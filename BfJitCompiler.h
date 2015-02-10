/*!
 * @file BfJitCompiler.h
 * @brief Brainfuck JIT-compiler
 * @author koturn
 */
#ifndef BF_JIT_COMPILER_H
#define BF_JIT_COMPILER_H


#ifdef USE_XBYAK
#include <xbyak/xbyak.h>
#include "BfIRCompiler.h"


namespace bf {


/*!
 * @brief Brainfuck JIT-compiler
 */
class BfJitCompiler : public Xbyak::CodeGenerator {
private:
  BfIR irCode;
public:
  static const std::size_t DEFAULT_GENERATOR_SIZE = 100000;
  BfJitCompiler(std::size_t size=DEFAULT_GENERATOR_SIZE) :
    CodeGenerator(size) {}
  BfJitCompiler(BfIR &irCode, std::size_t size=DEFAULT_GENERATOR_SIZE) :
    irCode(irCode), CodeGenerator(size) {}
  void setIRCode(BfIR &irCode) { this->irCode = irCode; }
  void compile(void);
};
#endif  // USE_XBYAK


}  // namespace bf
#endif  // BF_JIT_COMPILER_H
