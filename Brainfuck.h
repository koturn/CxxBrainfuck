#ifndef BRAINFUCK_H
#define BRAINFUCK_H


#include <vector>
#if __cplusplus >= 201103L
#  include <memory>
#endif  // __cplusplus >= 201103L
#ifdef USE_XBYAK
#  if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#    define  XBYAK_NO_OP_NAMES
#  endif
#  include <xbyak/xbyak.h>
#endif  // USE_XBYAK

#include "BfIRCompiler.h"
#include "BfJitCompiler.h"
#include "CodeGenerator/CodeGenerator.h"
#include "compat.h"


namespace bf {


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
    WIN_BIN_X86,
    ELF_BIN_X64
  } BinType;

  Brainfuck(std::size_t memorySize=65536) :
    memorySize(memorySize),
    binCodeSize(0),
    compileType(NO_COMPILE),
    sourceBuffer(nullptr),
    binCode(nullptr)
#ifdef USE_XBYAK
    , xbyakRtStackSize(XBYAK_RT_STACK_SIZE)
#endif  // USE_XBYAK
    {}
#if __cplusplus < 201103L
  ~Brainfuck(void);
#endif  // __cplusplus < 201103L

  void load(const char *filename);
  void trim(void);
  void compile(CompileType compileType=NORMAL_COMPILE);
  void execute(void);
  void translate(LANG lang=LANG_C);
  void generateWinBinary(BinType wbt=WIN_BIN_X86);
  inline const unsigned char *getWinBinary(void) const;
  inline std::size_t getWinBinarySize(void) const;
#ifdef USE_XBYAK
  void xbyakDump(void);
#endif  // USE_XBYAK

private:
  std::size_t memorySize;
  std::size_t binCodeSize;
  CompileType compileType;
#if __cplusplus >= 201103L
  std::unique_ptr<char[]> sourceBuffer;
  std::unique_ptr<unsigned char[]> binCode;
#else
  char* sourceBuffer;
  unsigned char* binCode;
#endif  // __cplusplus >= 201103L
  BfIRCompiler  irCompiler;
  BfJitCompiler jitCompiler;
#ifdef USE_XBYAK
  static const unsigned int XBYAK_RT_STACK_SIZE = 128 * 1024;
  std::size_t xbyakRtStackSize;
#endif  // USE_XBYAK

  void normalCompile(void);
  void interpretExecute(void) const;
  void compileExecute(void) const;

  template<class TCodeGenerator>
    void generateCode(TCodeGenerator& cg);
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
#if __cplusplus >= 201103L
  return binCode.get();
#else
  return binCode;
#endif  // __cplusplus >= 201103L
}


/*!
 * @brief Get size of executable Windows binary
 * @return Size of executable Windows binary
 */
inline std::size_t
Brainfuck::getWinBinarySize(void) const
{
  return binCodeSize;
}


}  // namespace bf
#endif  // BRAINFUCK_H
