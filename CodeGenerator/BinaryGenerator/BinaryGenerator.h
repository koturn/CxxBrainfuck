/*!
 * @file BinaryGenerator.h
 * @brief Super class for executable binary generator
 * @author koturn
 */
#ifndef BINARY_GENERATOR_H
#define BINARY_GENERATOR_H

#include <stack>
#include "../CodeGenerator.h"


namespace bf {


/*!
 * @brief Super class for executable binary generator
 */
class BinaryGenerator : public CodeGenerator {
protected:
  virtual void genPlorogue(void) = 0;
  virtual void genEpirogue(void) = 0;
  static const std::size_t DEFAULT_MAX_CODE_SIZE = 1048576;
  std::size_t codeSize;
  std::size_t binSize;
  std::stack<unsigned char *> loopStack;
public:
  BinaryGenerator(BfIR irCode) :
    CodeGenerator(irCode) {}
  inline void genCode(void);
  inline unsigned char *getCode(void) const { return code; }
  inline std::size_t getSize(void) const { return binSize; }
};


/*!
 * @brief Generate binary code
 */
inline void
BinaryGenerator::genCode(void)
{
  genPlorogue();
  genMainCode();
  genEpirogue();
  genHeader();
  genFooter();
}


}  // namespace bf
#endif  // BINARY_GENERATOR_H
