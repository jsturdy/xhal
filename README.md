# xhal
XHAL interface library

Developers documentation is generated with Doxygen and can be found under doc/

# Compilation Instructions

To compile xhal RPC modules execute:

```
make rpc
```

To compile CTP7 RPC libraries first compile `$BUILD_HOME/ctp7_modules` then execute:

```
make clean -f Makefile_ctp7
```
