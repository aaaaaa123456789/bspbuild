# bspbuild

Automatically generate BSP patches, without needing to write BSP code by hand.

This utility will generate a BSP patch for a set of files automatically, that validates both input and output files
and allows the user to generate multiple output files from the same input, without needing to write BSP code by hand.
The resulting patch can be applied by using a BSP patcher, such as the one found [here][patcher-website].

For more information, check the [documentation](docs.md). For additional information about BSP patches, check the
corresponding [repository][bsp-repo].

[patcher-website]: https://aaaaaa123456789.github.io/bsp
[bsp-repo]: https://github.com/aaaaaa123456789/bsp

### Installation instructions

The only prerequisite is a C99 compiler; bspbuild doesn't have any other dependencies.

To compile bspbuild, run `make`. This will use the C compiler specified by the `CC` environment variable; it will
default to `gcc` if the variable is unset. You can also pass `CC=<compiler name>` to make itself.  
Compiling the utility will generate an executable called `bspbuild` in the project's root directory.

To install bspbuild system-wide under Linux, run `sudo make install`. This will default to installing bspbuild in
`/usr/local/bin`; pass `INSTALL_PATH=<directory>` to override this default.

If you want to generate a debuggable binary, run `make debug` instead. This will compile with `-Og -ggdb`, as opposed
to `-O3` for the release binary.

If your compiler has warnings enabled by default (e.g., clang), compilation may produce a number of warnings. Any such
warnings should be ignored.

Native compilation under Windows should not be any different from Linux, although the `EXENAME` variable can be used
to ensure that the binary has the correct `.exe` extension (e.g., `make EXENAME=bspbuild.exe`).

Cross-compilation for Windows can be achieved by setting the `TOOLCHAIN_PREFIX` variable to the cross toolchain's
prefix. Two targets are provided in the Makefile, `w32` and `w64`, that set these two variables appropriately for
cross-compilation for 32 and 64-bit Windows targets. The toolchain prefixes they use are determined by the
`W32_PREFIX` and `W64_PREFIX` variables, which default to the default prefixes for the MinGW cross toolchain,
`i686-w64-mingw32-` and `x86_64-w64-mingw32-`.  
Also, `make windows` will generate both executables (32 and 64-bit) using the variables mentioned above to select the
individual toolchain prefixes, and overriding the executable name to be `bspbuild32.exe` and `bspbuild64.exe`. This is
used to build the published releases.
