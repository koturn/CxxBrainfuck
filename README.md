Brainfuck
=========

Brainfuck interpreter.

This program provides normal Brainfuck interpreter, Brainfuck interpreter with
simple compile and Brainfuck interpreter with JIt-compile of Brainfuck.


## Usage

### Run Brainfuck program

```sh
$ ./brainfuck [Brainfuck source file]
```


## Build

#### With GCC

Use [Makefile](Makefile).

###### Release version

```sh
$ make
```

###### Debug version

```sh
$ make DEBUG=true
```

#### With MSVC

Use [msvc.mk](msvc.mk).
[msvc.mk](msvc.mk) is written for nmake.

###### Release version

```sh
> nmake /f msvc.mk
```

###### Debug version

```sh
> nmake /f msvc.mk DEBUG=true
```


## References

- [http://en.wikipedia.org/wiki/Brainfuck](http://en.wikipedia.org/wiki/Brainfuck)


## Dependent libraries

- [Xbyak](https://github.com/herumi/xbyak)

#### MSVC only

- [msvcdbg](https://github.com/koturn/msvcdbg)


## LICENSE

This software is released under the MIT License, see [LICENSE](LICENSE).
