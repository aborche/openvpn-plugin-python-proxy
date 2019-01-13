#!/usr/local/bin/python2

'''
openvpn plugin proxy module

Copyright (C) 2018 Kaltashkin Eugene <aborche.aborche@gmail.com>

For detailed description of plugin functions read 
https://github.com/OpenVPN/openvpn/blob/master/include/openvpn-plugin.h.in

'''
import sys, os

OPENVPN_PLUGIN_FUNC_SUCCESS = 0
OPENVPN_PLUGIN_FUNC_ERROR = 1
OPENVPN_PLUGIN_FUNC_DEFERRED = 2

def test(*kwargs):
    for i in kwargs:
        print 'kwargs: %s'%i
    print "%s"%sys.argv
    print os.environ.keys()

def unpack_argv():
    return dict((k,v) for k,v in [ x.split('=',1) for x in sys.argv ])

def OPENVPN_PLUGIN_UP(*args):
    """
    Function OPENVPN_PLUGIN_UP called when plugin started first time from OpenVPN main process
    """
    print "-" * 30
    print "OPENVPN_PLUGIN_UP"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_DOWN(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_DOWN"
    """
    Function OPENVPN_PLUGIN_DOWN called when OpenVPN main process is shutting down
    """
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_ROUTE_UP(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_ROUTE_UP"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_IPCHANGE(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_IPCHANGE"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_TLS_VERIFY(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_TLS_VERIFY"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY(*args):
    print "-" * 30
    """
    Function OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY used for checking username/password pair.
    in OpenVPN debug log password field is not included due security purposes, but exists in argv array
    """
    print "OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_CLIENT_CONNECT(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_CLIENT_CONNECT\n"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_CLIENT_DISCONNECT(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_CLIENT_DISCONNECT\n"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_LEARN_ADDRESS(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_LEARN_ADDRESS\n"
    envp = unpack_argv()
    '''
    # Uncomment this block for activate packet filter file creation
    if 'pf_file' in envp:
        print 'PF_File is %s'%(envp['pf_file'])
        rules = ["[CLIENTS DROP]",
                "+fa56bf61-90da-11e8-bf33-005056a12a82-1234567",
                "+12345678-90da-11e8-bf33-005056a12a82-1234567",
                "[SUBNETS DROP]",
                "+10.150.0.1",
                "[END]"]
        with open(envp['pf_file'], 'w') as f:
            f.write('\n'.join(rules))
    '''
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS
    

def OPENVPN_PLUGIN_CLIENT_CONNECT_V2(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_CLIENT_CONNECT_V2"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_TLS_FINAL(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_TLS_FINAL\n"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_ENABLE_PF(*args):
    """
    Function OPENVPN_ENABLE_PF used for enable personal firewall rules for each client.
    If OPENVPN_ENABLE_PF is enabled, each called plugins part checks pf_file environment file
    """
    print "-" * 30
    print "OPENVPN_PLUGIN_ENABLE_PF"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_ROUTE_PREDOWN(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_ROUTE_PREDOWN"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_PLUGIN_N(*args):
    print "-" * 30
    print "OPENVPN_PLUGIN_N"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

def OPENVPN_UNKNOWN_PLUGIN_TYPE(*args):
    print "-" * 30
    print "OPENVPN_UNKNOWN_PLUGIN_TYPE"
    envp = unpack_argv()
    print envp
    print "=" * 30
    return OPENVPN_PLUGIN_FUNC_SUCCESS

