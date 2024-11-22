![Modula's Garter compiler logo](./assets/garter.png?height=200)
# m
is a modular cross-compiler interface.
It does not contain any frontend or backends on its own,
but rather functions as a toolchain
that allows installing and interoperating compilers
follow `m`'s simple conventions.
The purpose of `m` is to function as the CLI for the
[`garter`](https://modula.dev/garter)
compiler and its components, but `m` itself should be simple enough to allow
cross-compatibility or incorporation into other projects.

## Quick-Use

1. Download or build `m`
2. Install modules
3. Compile by daisy-chaining source files between frontend

Running `m help` after installation should display usage information

## Example Use
If you installed a module for compiling C called c99
and you wanted to produce a Windows executable using the
standard [garter](https://modula.dev/garter) toolchain,
that command would look something like
```
m main.c main.gasm main.gyb main.exe // implicit
m -c99 main.c -a main.gasm -exe main.gyb main.exe // explicit
```
What happens under-the-hood is that the `main.c`
is loaded into the c99 module, and that frontend
outputs `main.gasm` which is loaded into the assembler.
The assembler output, `main.gyb`,
is then loaded into the Windows PE-EXE backend.

> [!NOTE]
> Build paths are a planned feature,
> wherein you can have an input file, an output file,
> and then the `m` toolchain will find the shortest
> compilation path between the input and output modules.
> It will look something like
> ```
> m --build main.c main.exe
> ```

## Module Conventions

Modules should accept the following special commands:
- `--version`, which prints version information
- `--contributors`, which prints the name of the module, and a list of all of its contributors
- `--license`, which prints the name of the module and the shorthand name its license.
For a list of such names, see [choosealicense.com](https://choosealicense.com/).
In the case of custom or other licenses,
the command should either print `custom:proprietary`,
or `custom:foss`. If you are unsure if you should
list your module as `custom:foss`, see [the FSF's four essential freedoms](https://en.wikipedia.org/wiki/The_Free_Software_Definition#The_Four_Essential_Freedoms_of_Free_Software)
- `--mconfig`, which prints the preferred module name, flag, and file extension it should be associated with

For normal use, `m` calls modules with
the `input` and `output` filenames to read and write.
If there are any errors during compilation,
the module should write the error to `output`
and exit with a non-zero status code.

## Licenses

`m` is made available under the copyleft [GPLv3 license](https://www.gnu.org/licenses/gpl-3.0.en.html).
Assets used by `m`, such as the
[Modula](https://modula.dev) and [garter](https://modula.dev/garter) branding,
are freely available but proprietary.
Modula permits redistribution of those assets alongside
the original or modified GPLv3 code so long as the copyright notice for those assets is still included.

> [!NOTE]
> The default `m license` command is sufficient
> for fufilling this requirement if left as-is