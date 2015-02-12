/*!
 * @file BfIRCompiler.cpp
 * @brief Brainfuck-IR compiler
 * @author koturn
 */
#include <stack>
#include <queue>
#include <iostream>
#include "BfIRCompiler.h"


static unsigned int
countChar(const char *srcptr, char ch);

template<char INST1, char INST2>
static int
compressInstruction(const char **_srcptr);


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
        {
          srcptr++;
          int value = compressInstruction<'>', '<'>(&srcptr) + 1;
          if (value > 0) {
            cmd.type = BfInstruction::PTR_ADD;
            cmd.value = value;
          } else if (value < 0) {
            cmd.type = BfInstruction::PTR_SUB;
            cmd.value = -value;
          } else {
            continue;
          }
        }
        break;
      case '<':
        {
          srcptr++;
          int value = compressInstruction<'<', '>'>(&srcptr) + 1;
          if (value > 0) {
            cmd.type = BfInstruction::PTR_SUB;
            cmd.value = value;
          } else if (value < 0) {
            cmd.type = BfInstruction::PTR_ADD;
            cmd.value = -value;
          } else {
            continue;
          }
        }
        break;
      case '+':
        {
          srcptr++;
          int value = compressInstruction<'+', '-'>(&srcptr) + 1;
          if (value > 0) {
            cmd.type = BfInstruction::ADD;
            cmd.value = value;
          } else if (value < 0) {
            cmd.type = BfInstruction::SUB;
            cmd.value = -value;
          } else {
            continue;
          }
        }
        break;
      case '-':
        {
          srcptr++;
          int value = compressInstruction<'-', '+'>(&srcptr) + 1;
          if (value > 0) {
            cmd.type = BfInstruction::SUB;
            cmd.value = value;
          } else if (value < 0) {
            cmd.type = BfInstruction::ADD;
            cmd.value = -value;
          } else {
            continue;
          }
        }
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
        switch (srcptr[1]) {
          case '-':
            if (srcptr[2] == ']') {
              srcptr += 3;
              int cnt = 0;
              for (; *srcptr == '+'; srcptr++) {
                cnt++;
              }
              srcptr--;
              cmd.type = BfInstruction::ASSIGN;
              cmd.value = cnt;
            } else {
              const char *_srcptr = srcptr;
              srcptr += 2;
              int cnt1 = compressInstruction<'>', '<'>(&srcptr);
              srcptr++;
              int cnt2 = compressInstruction<'+', '-'>(&srcptr);
              srcptr++;
              int cnt3 = compressInstruction<'<', '>'>(&srcptr);
              if (srcptr[1] != ']' || cnt2 == 0 || cnt1 != cnt3) {
                srcptr = _srcptr;
                cmd.type = BfInstruction::LOOP_START;
                loopStack.push(static_cast<unsigned int>(irCode.size()));
              } else {
                srcptr++;
                if (cnt2 == 1) {
                  cmd.type = BfInstruction::ADD_VAR;
                  cmd.value = cnt1;
                } else if (cnt2 == -1) {
                  cmd.type = BfInstruction::SUB_VAR;
                  cmd.value = cnt1;
                } else {
                  srcptr = _srcptr;
                  cmd.type = BfInstruction::LOOP_START;
                  loopStack.push(static_cast<unsigned int>(irCode.size()));
                }
              }
            }
            break;
          case '>':
          case '<':
            {
              const char *_srcptr = srcptr;
              srcptr++;
              int cnt1 = compressInstruction<'>', '<'>(&srcptr);
              srcptr++;
              int cnt2 = compressInstruction<'+', '-'>(&srcptr);
              srcptr++;
              int cnt3 = compressInstruction<'<', '>'>(&srcptr);
              if (srcptr[1] != '-' || srcptr[2] != ']' || cnt2 == 0 || cnt1 != cnt3) {
                srcptr = _srcptr;
                cmd.type = BfInstruction::LOOP_START;
                loopStack.push(static_cast<unsigned int>(irCode.size()));
              } else {
                srcptr += 2;
                if (cnt2 == 1) {
                  cmd.type = BfInstruction::ADD_VAR;
                  cmd.value = cnt1;
                } else if (cnt2 == -1) {
                  cmd.type = BfInstruction::SUB_VAR;
                  cmd.value = cnt1;
                } else {
                  srcptr = _srcptr;
                  cmd.type = BfInstruction::LOOP_START;
                  loopStack.push(static_cast<unsigned int>(irCode.size()));
                }
              }
            }
            break;
          default:
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
    // std::cout << dumpEnum(cmd.type) << "  :  " << static_cast<int>(cmd.value) << std::endl;
    irCode.push_back(cmd);
  }
  // exit(1);
  // std::cout << "END: parse" << std::endl;
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


/*!
 * @brief Compress continuous instructions which has same meanings
 * @tparam [in] INST1  Instruction '+', '-', '>', '<'
 * @tparam [in] INST2  Instruction correspond to INST1, which has reverse meaning
 * @param srcptr  Pointer to Brainfuck source code
 * @return Compressed value
 */
template<char INST1, char INST2>
static int
compressInstruction(const char **_srcptr)
{
  const char *srcptr = *_srcptr;
  int value = 0;
  for (;; srcptr++) {
    if (*srcptr == INST1) {
      value++;
    } else if (*srcptr == INST2) {
      value--;
    } else {
      break;
    }
  }
  *_srcptr = srcptr - 1;
  return value;
}
