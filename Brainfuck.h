#ifndef BRAINFUCK_H
#define BRAINFUCK_H


#include <vector>

#ifdef USE_XBYAK
#  if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#    define  XBYAK_NO_OP_NAMES
#  endif
#  include <xbyak/xbyak.h>
#endif

#include "CodeGenerator/CodeGenerator.h"


class Brainfuck {
public:
  typedef enum {
    NO_COMPILE, NORMAL_COMPILE
#ifdef USE_XBYAK
    , XBYAK_JIT_COMPILE
#endif  // USE_XBYAK
  } CompileType;

  typedef enum {
    LANG_C,
    LANG_CPP,
    LANG_CSHARP,
    LANG_JAVA,
    LANG_LUA,
    LANG_PYTHON,
    LANG_RUBY
  } LANG;

  Brainfuck(std::size_t memorySize=65536) :
    memorySize(memorySize), sourceBuffer(NULL), compileType(NO_COMPILE)
#ifdef USE_XBYAK
    , generator(GENERATOR_SIZE), xbyakRtStackSize(XBYAK_RT_STACK_SIZE), xbyakRtStack(NULL)
#endif  // USE_XBYAK
    {}
  ~Brainfuck(void);

  void load(const char *filename);
  void trim(void);
  void compile(CompileType compileType=NORMAL_COMPILE);
  void execute(void);
  void translate(LANG lang=LANG_C);
#ifdef USE_XBYAK
  void xbyakDump(void);
#endif  // USE_XBYAK

private:
  typedef enum {
    PTR_ADD, PTR_SUB,
    ADD, SUB,
    PUTCHAR, GETCHAR,
    LOOP_START, LOOP_END
  } Instruction;

  struct Command {
    Instruction  type;
    unsigned int value;
  };

  std::size_t memorySize;
  char *sourceBuffer;
  std::vector<Command> commands;
  CompileType compileType;
#ifdef USE_XBYAK
  static const unsigned int GENERATOR_SIZE = 100000;
  static const unsigned int XBYAK_RT_STACK_SIZE = 128 * 1024;
  Xbyak::CodeGenerator generator;
  std::size_t xbyakRtStackSize;
  int *xbyakRtStack;
#endif  // USE_XBYAK

  void normalCompile(void);
  void interpretExecute(void);
  void compileExecute(void);
  void generateCode(CodeGenerator &tmpl);
#ifdef USE_XBYAK
  void xbyakJitCompile(void);
  void xbyakJitExecute(void);
#endif  // USE_XBYAK
};


#endif  // BRAINFUCK_H
