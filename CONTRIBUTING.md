# Contributing

This document serves as a style-guide and reference to help ensure consistency
in the project.


## Testing

To run the test suite, you need the [Check](http://check.sourceforge.net/) unit
testing framework. You can find more info on downloading / setting it up on
their [install page](http://check.sourceforge.net/web/install.html), but the
short of it is:

```sh
$ # macOS
$ brew install check
$ # Ubuntu
$ sudo apt-get install check
```

You can run the test suite by:

```sh
$ mkdir -p build && cd build
$ cmake .. && make
$ bin/check_bsh
```


## Making Changes

- no compiler warnings
- contants are in all caps and are prefixed with `BSH_`
- include guards are fully qualified, swapping / and . for _
- initialize pointers to NULL
- check for null pointer with `if (ptr)` instead of `if (ptr == NULL)`
- global variables should be prefixed with `g_`


## Documentation

- javadoc style comment
- use @ style
- don't use @file or @brief (they're both redundant)
- int var; `/*!< Detailed description after the member */`
- use @todo and preferably link to GH issue
- use @bug and preferably link to GH issue
- add examples with @code
- use @param, @param[in], @param[out], @param[in, out]


## References

- [C/C++ Mozilla Coding Style page](https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Coding_Style)
