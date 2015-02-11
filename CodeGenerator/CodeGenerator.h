/*!
 * @file CodeGenerator.h
 * @brief Super class for source code generator and binary generator
 * @author koturn
 */
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <cstdlib>
#include "../BfIRCompiler.h"


namespace bf {


/*!
 * @brief Super class for source code generator and binary generator
 */
class CodeGenerator {
private:
  static const std::size_t DEFAULT_MAX_CODE_SIZE = 1048576;
protected:
  BfIR irCode;
  unsigned char *code;
  unsigned char *codePtr;
  virtual void genHeader(void) = 0;
  virtual void genFooter(void) = 0;
  virtual void genPtrAdd(unsigned int value) = 0;
  virtual void genPtrSub(unsigned int value) = 0;
  virtual void genAdd(unsigned int value) = 0;
  virtual void genSub(unsigned int value) = 0;
  virtual void genPutchar(void) = 0;
  virtual void genGetchar(void) = 0;
  virtual void genLoopStart(void) = 0;
  virtual void genLoopEnd(void) = 0;
  virtual void genAssign(unsigned int value) {
    genLoopStart();
    genSub(1);
    genLoopEnd();
    genAdd(value);
  }
public:
  CodeGenerator(BfIR &irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    irCode(irCode), code(NULL), codePtr(NULL)
  {
    code = new unsigned char[codeSize];
    std::memset(code, 0, codeSize);
    codePtr = code;
  }
  CodeGenerator(std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    code(NULL), codePtr(NULL)
  {
    code = new unsigned char[codeSize];
    std::memset(code, 0, codeSize);
    codePtr = code;
  }
  ~CodeGenerator(void)
  {
    delete[] code;
  }

  void setIRCode(BfIR &irCode)
  {
    this->irCode = irCode;
  }
  virtual void genCode(void) = 0;
};


}  // namespace bf
#endif  // CODE_GENERATOR_H
