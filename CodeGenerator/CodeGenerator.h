/*!
 * @file CodeGenerator.h
 * @brief Super class for source code generator and binary generator
 * @author koturn
 */
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <algorithm>
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
  void genMainCode(void);
  inline virtual void genHeader(void) = 0;
  inline virtual void genFooter(void) = 0;
  inline virtual void genNext(void);
  inline virtual void genPrev(void);
  inline virtual void genNextN(int value) = 0;
  inline virtual void genPrevN(int value) = 0;
  inline virtual void genInc(void);
  inline virtual void genDec(void);
  inline virtual void genAdd(int value) = 0;
  inline virtual void genSub(int value) = 0;
  inline virtual void genIncAt(int value);
  inline virtual void genDecAt(int value);
  inline virtual void genAddAt(int value1, int value2);
  inline virtual void genSubAt(int value1, int value2);
  inline virtual void genPutchar(void) = 0;
  inline virtual void genGetchar(void) = 0;
  inline virtual void genLoopStart(void) = 0;
  inline virtual void genLoopEnd(void) = 0;
  inline virtual void genAssign(int value);
  inline virtual void genAssignAt(int value1, int value2);
  inline virtual void genSearchZero(int value);
  inline virtual void genAddVar(int value);
  inline virtual void genSubVar(int value);
  inline virtual void genCmulVar(int value1, int value2);
  inline virtual void genInfLoop(void);
public:
  CodeGenerator(BfIR &irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    irCode(irCode), code(NULL), codePtr(NULL)
  {
    code = new unsigned char[codeSize];
    std::fill_n(code, 0, codeSize);
    codePtr = code;
  }
  CodeGenerator(std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    code(NULL),
    codePtr(NULL)
  {
    code = new unsigned char[codeSize];
    std::fill_n(code, 0, codeSize);
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


inline void
CodeGenerator::genMainCode(void)
{
  for (BfIR::const_iterator cmd = irCode.begin(), end = irCode.end(); cmd != end; cmd++) {
    switch (cmd->type) {
      case BfInstruction::NEXT:
        genNext();
        break;
      case BfInstruction::PREV:
        genPrev();
        break;
      case BfInstruction::NEXT_N:
        genNextN(cmd->value1);
        break;
      case BfInstruction::PREV_N:
        genPrevN(cmd->value1);
        break;
      case BfInstruction::INC:
        genInc();
        break;
      case BfInstruction::DEC:
        genDec();
        break;
      case BfInstruction::ADD:
        genAdd(cmd->value1);
        break;
      case BfInstruction::SUB:
        genSub(cmd->value1);
        break;
      case BfInstruction::INC_AT:
        genIncAt(cmd->value1);
        break;
      case BfInstruction::DEC_AT:
        genDecAt(cmd->value1);
        break;
      case BfInstruction::ADD_AT:
        genAddAt(cmd->value1, cmd->value2);
        break;
      case BfInstruction::SUB_AT:
        genSubAt(cmd->value1, cmd->value2);
        break;
      case BfInstruction::PUTCHAR:
        genPutchar();
        break;
      case BfInstruction::GETCHAR:
        genGetchar();
        break;
      case BfInstruction::LOOP_START:
        genLoopStart();
        break;
      case BfInstruction::LOOP_END:
        genLoopEnd();
        break;
      case BfInstruction::ASSIGN_ZERO:
        genAssign(0);
        break;
      case BfInstruction::ASSIGN:
        genAssign(cmd->value1);
        break;
      case BfInstruction::ASSIGN_AT:
        genAssignAt(cmd->value1, cmd->value2);
        break;
      case BfInstruction::SEARCH_ZERO:
        genSearchZero(cmd->value1);
        break;
      case BfInstruction::ADD_VAR:
        genAddVar(cmd->value1);
        break;
      case BfInstruction::SUB_VAR:
        genSubVar(cmd->value1);
        break;
      case BfInstruction::CMUL_VAR:
        genCmulVar(cmd->value1, cmd->value2);
        break;
      case BfInstruction::INF_LOOP:
        genInfLoop();
        break;
    }
  }
}


inline void
CodeGenerator::genNext(void)
{
  genNextN(1);
}


inline void
CodeGenerator::genPrev(void)
{
  genPrevN(1);
}


inline void
CodeGenerator::genInc(void)
{
  genAdd(1);
}


inline void
CodeGenerator::genDec(void)
{
  genSub(1);
}


inline void
CodeGenerator::genIncAt(int value)
{
  genAddAt(value, 1);
}


inline void
CodeGenerator::genDecAt(int value)
{
  genSubAt(value, 1);
}


inline void
CodeGenerator::genAddAt(int value1, int value2)
{
  if (value1 > 0) {
    genNextN(value1);
    genAdd(value2);
    genPrevN(value1);
  } else {
    genPrevN(-value1);
    genAdd(value2);
    genNextN(-value1);
  }
}


inline void
CodeGenerator::genSubAt(int value1, int value2)
{
  if (value1 > 0) {
    genNextN(value1);
    genSub(value2);
    genPrevN(value1);
  } else {
    genPrevN(-value1);
    genSub(value2);
    genNextN(-value1);
  }
}


inline void
CodeGenerator::genAssign(int value)
{
  genLoopStart();
  genDec();
  genLoopEnd();
  genAdd(value);
}


inline void
CodeGenerator::genAssignAt(int value1, int value2)
{
  if (value1 > 0) {
    genNextN(value1);
    genAssign(value2);
    genPrevN(value1);
  } else {
    genPrevN(-value1);
    genAssign(value2);
    genNextN(-value1);
  }
}


inline void
CodeGenerator::genSearchZero(int value)
{
  genLoopStart();
  if (value > 0) {
    value == 1 ? genNext() : genNextN(value);
  } else {
    value == -1 ? genPrev() : genPrevN(-value);
  }
  genLoopEnd();
}


inline void
CodeGenerator::genAddVar(int value)
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


inline void
CodeGenerator::genSubVar(int value)
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


inline void
CodeGenerator::genCmulVar(int value1, int value2)
{
  genLoopStart();
  genDec();
  if (value1 > 0) {
    if (value1 == 1) {
      genNext();
    } else {
      genNextN(value1);
    }
  } else {
    if (value1 == -1) {
      genPrev();
    } else {
      genPrevN(-value1);
    }
  }
  genAdd(value2);
  if (value1 > 0) {
    if (value1 == 1) {
      genPrev();
    } else {
      genPrevN(value1);
    }
  } else {
    if (value1 == -1) {
      genNext();
    } else {
      genNextN(-value1);
    }
  }
  genLoopEnd();
}


inline void
CodeGenerator::genInfLoop(void)
{
  genLoopStart();
  genLoopEnd();
}


}  // namespace bf
#endif  // CODE_GENERATOR_H
