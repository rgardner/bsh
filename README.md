# bsh
[![Build Status](https://travis-ci.org/rgardner/bsh.svg?branch=master)](https://travis-ci.org/rgardner/bsh)

Bob's finally coming out of his shell.

```
bsh             __
    ___        |  "---.
  ."   ". -o)  |      |
<|_______|/`   |__,---;
  UU   UU ====|_______|
Welcome to Bob shell.
```

## Installation
OS X and Ubuntu Linux are currently supported and tested. This project
requires [CMake](https://cmake.org/) version 2.8 or higher to build.

For OS X, you need the following development dependencies:
```sh
$ brew install cmake
```

For Ubuntu, you need additional dependencies:
```sh
$ sudo apt-get install cmake libbsd-dev libreadline-dev pkg-config
```

After these depenencies are installed:
```bash
$ git clone https://github.com/rgardner/bsh
$ cd bsh
$ mkdir build && cd build
$ cmake .. && make
```

After building `bsh`, you can run the executable: `build/bin/bsh`.

### Documentation
[Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) is required to
build documentation:

```sh
$ # macOS
$ brew install doxygen
$ # Ubuntu
$ sudo apt-get install doxygen
```

To generate the documentation:

```sh
$ mkdir -p build && cd build
$ cmake ..
$ make doc
```

### Testing
To run the test suite, you need the [Check](http://check.sourceforge.net/) unit
testing framework. You can find more info on downloading / setting it up on
their [install page](http://check.sourceforge.net/web/install.html), but the
short of it is:
```bash
$ brew install check  # OS X
$ sudo apt-get install check  # Ubuntu
```

You can run the test suite by:
```sh
$ mkdir -p build && cd build
$ cmake .. && make
$ bin/check_bsh
```

### Dependencies Explained
**CMake** is the build system used for this project. Originally, this started as
a simple Makefile, but it grew more complicated until eventually `cmake` became
the more viable option.

**libbsd-dev** is required on Ubuntu to get access to `strlcpy` and `strlcat`,
two functions that make working with strings much easier. [This
paper](https://www.sudo.ws/todd/papers/strlcpy.html) does a good job of
explaining the design intentions of these functions.

**check** is the unit-testing framework chosen for this project. It is one of
the most popular C unit-testing frameworks and works well with `cmake`.


## Feature Roadmap
1. ~~write the parser~~
  - ~~http://www.cs.cornell.edu/Courses/cs414/2004su/homework/shell/parse.html~~
2. ~~basic shell support~~
  - ~~http://www.cs.cornell.edu/Courses/cs414/2004su/homework/shell/shell.html~~
3. ~~Extra history features~~
  - ~~`history -s num` and `history num`~~
4. ~~`which`~~
  - ~~http://www.opensource.apple.com/source/shell_cmds/shell_cmds-170/which/which.c~~
5. ~~`dirs, pushd, popd`~~
6. ~~`alias`~~
7. `pipe`
8. `advanced I/O`
9. `fg/bg`
10. `printenv/setenv`
11. shell scripting: `if/then,while,for`
12. tab completion and command prompt editing
13. up and down arrows to scroll through history list
14. relax constrains (allow `prog>outfile`)
15. terminal support
16. port to Windows

## Resources
These resources have been invaluable to me while working on this project:

1. Cornell CS414 Summer 2004 by Jeanna Matthews
  - ["Parse.c" assignment](http://www.cs.cornell.edu/Courses/cs414/2004su/homework/shell/parse.html)
  - ["Writing Your Own Shell" assignment](http://www.cs.cornell.edu/Courses/cs414/2004su/homework/shell/shell.html)
2. [GNU History Library](http://cnswww.cns.cwru.edu/php/chet/readline/history.html)
3. [Bash source code](https://ftp.gnu.org/gnu/bash/)
4. [Implementing a Job Control Shell](http://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html#Implementing-a-Shell)
5. [The C Programming Language, 2nd
   Edition](http://smile.amazon.com/dp/0131103628/ref=cm_sw_r_tw_dp_.lqevb1B3CJ24)

## Credit
The awesome "geek tortoise" ascii art was found on
[Retrojunkie](http://www.retrojunkie.com/asciiart/animals/turtles.htm).
