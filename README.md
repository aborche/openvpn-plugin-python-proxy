# openvpn-plugin-python-proxy

OpenVPN plugin python proxy

# Description

This package proxies OpenVPN plugin functions requests to python module

# Attention!

Package builds and tested on FreeBSD.
Before build, check and change LIBDIR variable of OpenVPN plugin directory location in Makefile.
If you use this package on Linux - please check system libraries/includes paths in Makefile and setup.py files.
If you change any logic in Python module - before module install and compile always do "make clean"

# Build

just type "make"

# Install

just type "make install"

# Cleanup

just type "make clean"

# Python module

In directory python_module_src you can find template python module for OpenVPN.
By default module has name "openvpn". If you need more than one module in OpenVPN,
change their name to another.

# Activate plugin

Add to openvpn configuration file next line and restart openvpn
```
plugin <plugindir>openvpn-plugin-python-proxy.so "openvpn"
```

Where "openvpn" is the name of python module installed to system.

# Credentials

* Kaltashkin Eugene <zhecka@gmail.com>

