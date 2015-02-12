/*!
 * @file BfIRCompiler.h
 * @brief Brainfuck-IR compiler
 * @author koturn
 */
#ifndef BF_IR_COMPILER_H
#define BF_IR_COMPILER_H

#include <cstdlib>
#include <vector>


namespace bf {


/*!
 * @brief Brainfuck instruction set
 */
class BfInstruction {
public:
  typedef enum {
    PTR_ADD, PTR_SUB,
    ADD, SUB,
    PUTCHAR, GETCHAR,
    LOOP_START, LOOP_END,
    ASSIGN, ADD_VAR, SUB_VAR, CMUL_VAR
  } Instruction;

  struct Command {
    Instruction  type;
    unsigned int value;
  };
};

typedef std::vector<BfInstruction::Command> BfIR;


/*!
 * @brief Brainfuck to Brainfuck-IR compiler
 */
class BfIRCompiler {
public:
  BfIRCompiler(const char *bfSource=NULL) :
    bfSource(bfSource) {}

  inline void setSource(const char *bfSource) {
    this->bfSource = bfSource;
  }
  void compile(void);
  inline BfIR getCode(void) const { return irCode; };
  inline BfIR::size_type getSize(void) const { return irCode.size(); };

private:
  const char *bfSource;
  BfIR irCode;
};


static inline const char *
dumpEnum(BfInstruction::Instruction inst)
{
  switch (inst) {
    case BfInstruction::PTR_ADD: return "PTR_ADD";
    case BfInstruction::PTR_SUB: return "PTR_SUB";
    case BfInstruction::ADD: return "ADD";
    case BfInstruction::SUB: return "SUB";
    case BfInstruction::PUTCHAR: return "PUTCHAR";
    case BfInstruction::GETCHAR: return "GETCHAR";
    case BfInstruction::LOOP_START: return "LOOP_START";
    case BfInstruction::LOOP_END: return "LOOP_END";
    case BfInstruction::ASSIGN: return "ASSIGN";
    case BfInstruction::ADD_VAR: return "ADD_VAR";
    case BfInstruction::SUB_VAR: return "SUB_VAR";
    case BfInstruction::CMUL_VAR: return "CMUL_VAR";
  }
  return "NONE";
}



}  // namespace bf
#endif  // BF_IR_COMPILER_H
