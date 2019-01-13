#!/usr/bin/env python2
#" -W default
import warnings; warnings.simplefilter('default')

import distutils.sysconfig
import os 
import sys

try:
  from setuptools import setup, Extension
except ImportError:
  from distutils.core import setup, Extension

long_description = """\
Embeds the Python interpreter into Openvpn plugins"""

classifiers = [
  "Topic :: System :: Systems Administration :: "]

if not os.environ.has_key("Py_DEBUG"):
  Py_DEBUG = []
else:
  Py_DEBUG = [('Py_DEBUG',1)]

libpython_so = distutils.sysconfig.get_config_var('INSTSONAME')
ext_modules = [
    Extension(
      "openvpn-plugin-python-proxy",
      sources=["openvpn-plugin-python-proxy.c"],
      include_dirs = ["/usr/local/include","/usr/include/openvpn"],
      library_dirs=["/usr/local/lib","/usr/lib"],
      define_macros=[('LIBPYTHON_SO','"'+libpython_so+'"')] + Py_DEBUG,
      libraries=["python%d.%d" % sys.version_info[:2]],
    ), ]

setup(
  name="openvpn-plugin-python-proxy",
  version="0.0.2",
  description="Enable python modules can be called from OpenVPN as Plugin",
  keywords="embed,authentication,security",
  platforms="Unix",
  long_description=long_description,
  author="Kaltashkin Eugene",
  author_email="aborche.aborche@gmail.com",
  url="http://aborche.com/",
  license="",
  classifiers=classifiers,
  ext_modules=ext_modules,
  packages=['openvpn'],
  package_dir={'openvpn': 'python_module_src/openvpn/'},
)
