CxxBrainfuck
============

[![Build Status](https://travis-ci.org/koturn/CxxBrainfuck.png)](https://travis-ci.org/koturn/CxxBrainfuck)

Brainfuck interpreter, translator and compiler.

This program provides Brainfuck interpreter, translator and compiler.

Interpreter works in three mode.

- Simple Interpreter
- Interpreter with simple compile
- Interpreter with JIT-compile

Simple Interpreter seeks one character.
In other words, it cannot skip.
So it works very slow, especially at loop.

Interpreter with simple compile works with decent speed.
It compile Brainfuck source code before execution as following.

- Combine continuous increment / decrement pointer-increment /
  pointer-decrement instruction
- Generate jump table for loop instruction
- Generate an instruction to assign zero

Interpreter with JIT-compile works very fast.
Brainfuck JIT-compiler was implemented with [Xbyak](https://github.com/herumi/xbyak).

Translator can translate Brainfuck into following seven languages.

- C
- C++
- C#
- Java
- Lua
- Python
- Ruby

Compiler can compile Brainfuck source code into x86 Windows executable binary.
(This function works at Windows only).


## Usage

### Run Brainfuck program

```sh
$ ./brainfuck.out [Brainfuck source file]
```

### Options

- ```-c TARGET, --compile=TARGET```
  - Specify output type
    - ```c```:      Compile to C source code
    - ```cpp```:    Compile to C++ source code
    - ```csharp```: Compile to C# source code
    - ```java```:   Compile to Java source code
    - ```lua```:    Compile to Lua source code
    - ```python```: Compile to Python source code
    - ```ruby```:   Compile to Python source code
    - ```xbyakc```: Compile to C source code dumped from Xbyak Code generator
    - ```winx86```: Compile to x86 Windows executable binary
    - ```elfx64```: Compile to x64 ELF binary
- ```-h```, ```--help```
  - Show help and exit this program
- ```-O OPT_LEVEL```, ```--optimize=OPT_LEVEL```
  - Specify optimize level
    - 0: Execute with No compile
    - 1: Execute with simple compile
    - 2: Execute with JIT compile
  - Default value: ```OPT_LEVEL = 1```
- ```-s MEMORY_SIZE```, ```--size=MEMORY_SIZE```
  - Specify memory size
  - Default value: ```MEMORY_SIZE = 65536```


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

- [Brainfuck](http://en.wikipedia.org/wiki/Brainfuck)
- [hayamiz/dbtimes-vol01](https://github.com/hayamiz/dbtimes-vol01)
- [7shi/bfexe](https://github.com/7shi/bfexe)
- [簡易brainf*ckコンパイラを作る part3 - わらばんし仄聞記](http://warabanshi.hatenablog.com/entry/2013/01/05/131602)
- [実用Brainf*ckプログラミング](http://www.slideshare.net/KMC_JP/brainfck)


## Dependent libraries

- [herumi/Xbyak](https://github.com/herumi/xbyak)

#### MSVC only

- [koturn/getopt](https://github.com/koturn/getopt)
- [koturn/msvcdbg](https://github.com/koturn/msvcdbg)


## LICENSE

This software is released under the MIT License, see [LICENSE](LICENSE).
