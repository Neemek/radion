# radion
This project is a programming language built in C++. It is a tree 
walking interpreter with a recursive-descent parser.

## Building
Firstly, make sure CMake is installed on your system.

To build the project, first make a folder called `debug`
or `release`, or something similar. `cd` into it, then
use one of the following commands:

```sh
# debug
cmake -DCMAKE_BUILD_TYPE=Debug ..

# release
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Then, you can build an executable with:

```sh
cmake --build .
```

Afterwards, an executable with the name `radion` or `radiond`,
depending on your os, should be in the build directory.

## Using
For examples of syntax, see the `./examples/` folder.

Using the executable without arguments opens a REPL.
Here you can write simple statements and the result
will be echoed back.

Using the exectuable *with* arguments causes it to
read and run those files, assuming they are source
files. 

Invalid syntax and tokens will be reported with
error messages.
