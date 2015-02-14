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
  virtual void genNext(void) {
    genNextN(1);
  }
  virtual void genPrev(void) {
    genPrevN(1);
  }
  virtual void genNextN(int value) = 0;
  virtual void genPrevN(int value) = 0;
  virtual void genInc(void) {
    genAdd(1);
  }
  virtual void genDec(void) {
    genSub(1);
  }
  virtual void genAdd(int value) = 0;
  virtual void genSub(int value) = 0;
  virtual void genPutchar(void) = 0;
  virtual void genGetchar(void) = 0;
  virtual void genLoopStart(void) = 0;
  virtual void genLoopEnd(void) = 0;
  virtual void genAssign(int value)
  {
    genLoopStart();
    genDec();
    genLoopEnd();
    genAdd(value);
  }
  virtual void genAddVar(int value)
  {
    genLoopStart();
    genDec();
    if (value > 0) {
      if (value == 1) {
        genNext();
      } else {
        genNextN(value);
      }
    } else {
      if (value == -1) {
        genPrev();
      } else {
        genPrevN(-value);
      }
    }
    genInc();
    if (value > 0) {
      if (value == 1) {
        genPrev();
      } else {
        genPrevN(value);
      }
    } else {
      if (value == -1) {
        genNext();
      } else {
        genNextN(-value);
      }
    }
    genLoopEnd();
  }
  virtual void genSubVar(int value)
  {
    genLoopStart();
    genDec();
    if (value > 0) {
      if (value == 1) {
        genNext();
      } else {
        genNextN(value);
      }
    } else {
      if (value == -1) {
        genPrev();
      } else {
        genPrevN(-value);
      }
    }
    genDec();
    if (value > 0) {
      if (value == 1) {
        genPrev();
      } else {
        genPrevN(value);
      }
    } else {
      if (value == -1) {
        genNext();
      } else {
        genNextN(-value);
      }
    }
    genLoopEnd();
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
