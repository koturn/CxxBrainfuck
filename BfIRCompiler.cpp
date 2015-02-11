/*!
 * @file BfIRCompiler.cpp
 * @brief Brainfuck-IR compiler
 * @author koturn
 */
#include <stack>
#include "BfIRCompiler.h"


static unsigned int
countChar(const char *srcptr, char ch);


namespace bf {


/*!
 * @brief Compile brainfuck code to Instruction set
 */
void
BfIRCompiler::compile(void)
{
  std::stack<unsigned int> loopStack;
  irCode.clear();
  for (const char *srcptr = bfSource; *srcptr != '\0'; srcptr++) {
    BfInstruction::Command cmd;
    switch (*srcptr) {
      case '>':
        cmd.type = BfInstruction::PTR_ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '<':
        cmd.type = BfInstruction::PTR_SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '+':
        cmd.type = BfInstruction::ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '-':
        cmd.type = BfInstruction::SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '.':
        cmd.type = BfInstruction::PUTCHAR;
        cmd.value = 0;
        break;
      case ',':
        cmd.type = BfInstruction::GETCHAR;
        cmd.value = 0;
        break;
      case '[':
        if ((srcptr[1] == '-' || srcptr[1] == '+') && srcptr[2] == ']') {
          srcptr += 3;
          int cnt = 0;
          for (; *srcptr == '+'; srcptr++) {
            cnt++;
          }
          srcptr--;
          cmd.type = BfInstruction::ASSIGN;
          cmd.value = cnt;
        } else {
          cmd.type = BfInstruction::LOOP_START;
          loopStack.push(static_cast<unsigned int>(irCode.size()));
        }
        break;
      case ']':
        cmd.type = BfInstruction::LOOP_END;
        cmd.value = loopStack.top();
        irCode[loopStack.top()].value = static_cast<unsigned int>(irCode.size() - 1);
        loopStack.pop();
        break;
    }
    irCode.push_back(cmd);
  }
}


}  // namespace bf


/*!
 * @brief Count how many given character are continuous.
 * @return The number of consecutive characters
 */
static unsigned int
countChar(const char *srcptr, char ch)
{
  unsigned int cnt = 1;
  while (*++srcptr == ch) {
    cnt++;
  }
  return cnt;
}
