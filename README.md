Brainfuck
=========

Brainfuck interpreter.


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

#### MSVC only

- [msvcdbg](https://github.com/koturn/msvcdbg)


## LICENSE

This software is released under the MIT License, see [LICENSE](LICENSE).
