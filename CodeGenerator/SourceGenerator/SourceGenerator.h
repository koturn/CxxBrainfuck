/*!
 * @file SourceGenerator.h
 * @brief Super class for source code generator
 * @author koturn
 */
#ifndef SOURCE_GENERATOR_H
#define SOURCE_GENERATOR_H

#include "../CodeGenerator.h"


namespace bf {


/*!
 * @brief Super class for source code generator
 */
class SourceGenerator : public CodeGenerator {
private:
  static const int DEFAULT_INDENT_LEVEL = 1;
protected:
  static const std::size_t DEFAULT_MAX_CODE_SIZE = 1048576;
  int indentLevel;
  const char *indent;
  inline void genIndent(void);
public:
  SourceGenerator(BfIR irCode, const char *indent="  ", int indentLevel=DEFAULT_INDENT_LEVEL) :
    CodeGenerator(irCode), indentLevel(indentLevel), indent(indent) {}
  inline void genCode(void);
};


/*!
 * @brief Generate source code
 */
inline void
SourceGenerator::genCode(void)
{
  genHeader();
  for (BfIR::const_iterator cmd = irCode.begin(), end = irCode.end(); cmd != end; cmd++) {
    switch (cmd->type) {
      case BfInstruction::PTR_ADD:
        genPtrAdd(cmd->value);
        break;
      case BfInstruction::PTR_SUB:
        genPtrSub(cmd->value);
        break;
      case BfInstruction::ADD:
        genAdd(cmd->value);
        break;
      case BfInstruction::SUB:
        genSub(cmd->value);
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
        genAssignZero();
        break;
    }
  }
  genFooter();
}


/*!
 * @brief Generate indent
 */
inline void
SourceGenerator::genIndent(void)
{
  for (int i = 0; i < indentLevel; i++) {
    std::cout << indent;
  }
}


}  // namespace bf
#endif  // SOURCE_GENERATOR_H
