* Right now it links in libz3.so at build time. This should be handled as a static lib beforehand.
    * Temporarily solved with: `sudo ln -s /usr/lib/libz3.so /usr/local/lib/libz3.so`

* Fix `target_compile_options(Parser PRIVATE --system-header-prefix=clang/)` so it works on all compilers
