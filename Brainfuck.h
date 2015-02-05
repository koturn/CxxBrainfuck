#ifndef BRAINFUCK_H
#define BRAINFUCK_H


#include <vector>

#ifdef USE_XBYAK
#  if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#    define  XBYAK_NO_OP_NAMES
#  endif
#  include <xbyak/xbyak.h>
#endif  // USE_XBYAK

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

  typedef enum {
    WIN_BIN_X86
  } WinBinType;

  Brainfuck(std::size_t memorySize=65536) :
    memorySize(memorySize), sourceBuffer(NULL), compileType(NO_COMPILE)
    , exeBin(NULL), exeBinSize(0)
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
  void generateWinBinary(WinBinType wbt=WIN_BIN_X86);
  inline const unsigned char *getWinBinary(void) const;
  inline std::size_t getWinBinarySize(void) const;
#ifdef USE_XBYAK
  void xbyakDump(void);
#endif  // USE_XBYAK

private:
  typedef enum {
    PTR_ADD, PTR_SUB,
    ADD, SUB,
    PUTCHAR, GETCHAR,
    LOOP_START, LOOP_END,
    ASSIGN_ZERO
  } Instruction;

  struct Command {
    Instruction  type;
    unsigned int value;
  };

  std::size_t memorySize;
  char *sourceBuffer;
  std::vector<Command> commands;
  CompileType compileType;
  unsigned char *exeBin;
  std::size_t exeBinSize;
#ifdef USE_XBYAK
  static const unsigned int GENERATOR_SIZE = 100000;
  static const unsigned int XBYAK_RT_STACK_SIZE = 128 * 1024;
  Xbyak::CodeGenerator generator;
  std::size_t xbyakRtStackSize;
  int *xbyakRtStack;
#endif  // USE_XBYAK

  void normalCompile(void);
  void interpretExecute(void) const;
  void compileExecute(void) const;

  template<class TCodeGenerator>
    void generateCode(TCodeGenerator &cg);
  void generateX86WinBinary(void);
#ifdef USE_XBYAK
  void xbyakJitCompile(void);
  void xbyakJitExecute(void);
#endif  // USE_XBYAK
};


/*!
 * @brief Get executable Windows binary
 * @return Pointer to executable Windows binary
 */
inline const unsigned char *
Brainfuck::getWinBinary(void) const
{
  return exeBin;
}


/*!
 * @brief Get size of executable Windows binary
 * @return Size of executable Windows binary
 */
inline std::size_t
Brainfuck::getWinBinarySize(void) const
{
  return exeBinSize;
}


#endif  // BRAINFUCK_H
