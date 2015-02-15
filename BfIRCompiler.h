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
    NEXT, PREV, NEXT_N, PREV_N,
    INC, DEC, ADD, SUB,
    INC_AT, DEC_AT, ADD_AT, SUB_AT,
    PUTCHAR, GETCHAR,
    LOOP_START, LOOP_END,
    ASSIGN_ZERO, ASSIGN, ASSIGN_AT, SEARCH_ZERO,
    ADD_VAR, SUB_VAR, CMUL_VAR,
    INF_LOOP
  } Instruction;

  struct Command {
    Instruction type;
    int value1;
    int value2;
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


}  // namespace bf
#endif  // BF_IR_COMPILER_H
