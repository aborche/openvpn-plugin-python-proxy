/*
 *
 *  Copyright (C) 2018 Kaltashkin Eugene <aborche.aborche@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * This file implements a python proxy for OpenVPN plugin module
 *
 * See the README file for build instructions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Python.h>

#include "openvpn-plugin.h"

/*
 * Our context, where we keep our state.
 */
struct plugin_context {
    const char *config_param;
};

int myStrLen(const char *envp[], int *envpsize)
{
    /*
     * Calculate envp array elements length and array size
     */
    int i = 0;
    int totalsize = 0;
    for(i = 0; envp[i] != '\0'; i++)
        {
            totalsize += strlen(envp[i]);
        }

    *envpsize = totalsize;
    return i;
}

OPENVPN_EXPORT int
openvpn_plugin_open_v3 (const int v3structver,
                        struct openvpn_plugin_args_open_in const *args,
                        struct openvpn_plugin_args_open_return *ret)
{
    struct plugin_context *context = NULL;

    /*
     * Allocate our context
     */
    context = (struct plugin_context *) calloc(1, sizeof(struct plugin_context));

    /*
     * Define plugin types which our script can serve
     */
    ret->type_mask = OPENVPN_PLUGIN_MASK (OPENVPN_PLUGIN_UP)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_DOWN)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_ROUTE_UP)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_IPCHANGE)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_TLS_VERIFY)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_CLIENT_CONNECT_V2)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_CLIENT_DISCONNECT)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_LEARN_ADDRESS)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_TLS_FINAL)
        |OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_ENABLE_PF);

    /*
     * Save parameters for plugin from openvpn config
     */
    if (args->argv[1])
        context->config_param = strdup(args->argv[1]);

    printf("openvpn-plugin-proxy: config_param=%s\n", context->config_param);

    /* Point the global context handle to our newly created context */
    ret->handle = (void *) context;

    return OPENVPN_PLUGIN_FUNC_SUCCESS;
}

OPENVPN_EXPORT int
openvpn_plugin_func_v3 (const int version,
                        struct openvpn_plugin_args_func_in const *args,
                        struct openvpn_plugin_args_func_return *retptr)
{
    struct plugin_context *context = (struct plugin_context *) args->handle;
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    int envpsize = 0;  // size of environment values
    int argvsize = 0;  // size of argument values
    char envptype[20]; // define environment variable with called plugin id
    char **envpexpanded; // expanded environment array
    char *PyDefName; // Python function name for calling 
    int PyReturn = OPENVPN_PLUGIN_FUNC_ERROR; 

    // Count size of envp
    int sizeof_envp = myStrLen(args->envp, &envpsize);
    sizeof_envp++;

    // make pluginid entry
    sprintf(envptype,"pluginid=%d",args->type);

    // make new array with environment values
    envpexpanded = malloc((sizeof_envp)*sizeof *envpexpanded);

    // Count size of args
    int sizeof_argv = myStrLen(args->argv, &argvsize);

    // Repack all values from envp to envpexpanded
    for(int i = 0; i < sizeof_envp-1; i++) {
      envpexpanded[i]=strdup(args->envp[i]);
      if (envpexpanded[i] == NULL) {
        printf("ENVPExpanded [%d] is null\n",i);
        return OPENVPN_PLUGIN_FUNC_ERROR;
      }
      // printf("envpexpanded[%d]=%s\n",i,envpexpanded[i]);
    }

    // add pluginid entry to new environment array
    envpexpanded[sizeof_envp-1] = strdup(envptype);

    /*
    for(int i = 0; i < sizeof_envp; i++) {
        printf("envpexpanded[%d]=%s\n",i,envpexpanded[i]);
    }
    */

    // Define Python function name for calling from python module
    switch (args->type) {
        case OPENVPN_PLUGIN_UP:
            PyDefName = strdup("OPENVPN_PLUGIN_UP");
            break;
        case OPENVPN_PLUGIN_DOWN:
            PyDefName = strdup("OPENVPN_PLUGIN_DOWN");
            break;
        case OPENVPN_PLUGIN_ROUTE_UP:
            PyDefName = strdup("OPENVPN_PLUGIN_ROUTE_UP");
            break;
        case OPENVPN_PLUGIN_IPCHANGE:
            PyDefName = strdup("OPENVPN_PLUGIN_IPCHANGE");
            break;
        case OPENVPN_PLUGIN_TLS_VERIFY:
            PyDefName = strdup("OPENVPN_PLUGIN_TLS_VERIFY");
            break;
        case OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY:
            PyDefName = strdup("OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY");
            break;
        case OPENVPN_PLUGIN_CLIENT_CONNECT:
            PyDefName = strdup("OPENVPN_PLUGIN_CLIENT_CONNECT");
            break;
        case OPENVPN_PLUGIN_CLIENT_DISCONNECT:
            PyDefName = strdup("OPENVPN_PLUGIN_CLIENT_DISCONNECT");
            break;
        case OPENVPN_PLUGIN_LEARN_ADDRESS:
            PyDefName = strdup("OPENVPN_PLUGIN_LEARN_ADDRESS");
            break;
        case OPENVPN_PLUGIN_CLIENT_CONNECT_V2:
            PyDefName = strdup("OPENVPN_PLUGIN_CLIENT_CONNECT_V2");
            break;
        case OPENVPN_PLUGIN_TLS_FINAL:
            PyDefName = strdup("OPENVPN_PLUGIN_TLS_FINAL");
            break;
        case OPENVPN_PLUGIN_ENABLE_PF:
            PyDefName = strdup("OPENVPN_PLUGIN_ENABLE_PF");
            break;
        case OPENVPN_PLUGIN_ROUTE_PREDOWN:
            PyDefName = strdup("OPENVPN_PLUGIN_ROUTE_PREDOWN");
            break;
        case OPENVPN_PLUGIN_N:
            PyDefName = strdup("OPENVPN_PLUGIN_N");
            break;
        default:
            PyDefName = strdup("OPENVPN_UNKNOWN_PLUGIN_TYPE");
            break;
    }

    Py_SetProgramName("openvpn-python-proxy");
    Py_Initialize();
    // Move plugin environment from envp to python argv
    PySys_SetArgv(sizeof_envp, envpexpanded);

    // cleanup memory
    for(int i = 0; i < sizeof_envp; i++) {
        free(envpexpanded[i]);
    }
    free(envpexpanded);

    // set module name for call python code
    pName = PyString_FromString(context->config_param); // "openvpn");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule != NULL) {
        // check function exists in python module
        pFunc = PyObject_GetAttrString(pModule, PyDefName);
        if (pFunc && PyCallable_Check(pFunc)) {
            // build args list
            pArgs = PyTuple_New(sizeof_argv);
            for (int i = 0; i < sizeof_argv; ++i) {
                pValue = PyString_FromString(args->argv[i]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return OPENVPN_PLUGIN_FUNC_ERROR;
                }
                // pValue reference stolen here: 
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                switch (PyInt_AsLong(pValue)) {
                    case 0:
                        PyReturn = OPENVPN_PLUGIN_FUNC_SUCCESS;
                        break;
                    case 1:
                        PyReturn = OPENVPN_PLUGIN_FUNC_ERROR;
                        break;
                    case 2:
                        PyReturn = OPENVPN_PLUGIN_FUNC_DEFERRED;
                        break;
                    default:
                        PyReturn = OPENVPN_PLUGIN_FUNC_ERROR;
                }
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return OPENVPN_PLUGIN_FUNC_ERROR;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", PyDefName);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module\n");
        return OPENVPN_PLUGIN_FUNC_ERROR;
    }                    

    Py_Finalize();

    return PyReturn;
}

OPENVPN_EXPORT void
openvpn_plugin_close_v1(openvpn_plugin_handle_t handle)
{
    struct plugin_context *context = (struct plugin_context *) handle;
    free(context);
}

