/*!
 * @file BfIRCompiler.cpp
 * @brief Brainfuck-IR compiler
 * @author koturn
 */
#include <stack>
#include <queue>
#include "BfIRCompiler.h"


template<char INST1, char INST2>
static int
compressInstruction(const char **_srcptr);

inline static bool
genOperationAt(bf::BfInstruction::Command &cmd, const bf::BfInstruction::Command &c1);

inline static bool
isPtrOperation(bf::BfInstruction::Instruction inst);

inline static bool
isOperationAt(bf::BfInstruction::Instruction inst);




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
          bool isNormalNext = true;
          srcptr++;
          int value = compressInstruction<'>', '<'>(&srcptr) + 1;
          if (irCode.size() > 1) {
            BfInstruction::Command c1 = irCode[irCode.size() - 1];  // (+*|-*) ?
            BfInstruction::Command c2 = irCode[irCode.size() - 2];  // <* ?
            if (value == 1 && c2.type == BfInstruction::PREV) {
              cmd.value1 = -1;
              isNormalNext = genOperationAt(cmd, c1);
            } else if (value == -1 && c2.type == BfInstruction::NEXT) {
              cmd.value1 = 1;
              isNormalNext = genOperationAt(cmd, c1);
            } else if (value > 1 && c2.type == BfInstruction::PREV_N && value == c2.value1) {
              cmd.value1 = -c2.value1;
              isNormalNext = genOperationAt(cmd, c1);
            } else if (value < 1 && c2.type == BfInstruction::NEXT_N && -value == c2.value1) {
              cmd.value1 = c2.value1;
              isNormalNext = genOperationAt(cmd, c1);
            }
          }
          if (isNormalNext) {
            if (value > 0) {
              if (value == 1) {
                cmd.type = BfInstruction::NEXT;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::NEXT_N;
                cmd.value1 = value;
                cmd.value2 = 0;
              }
            } else if (value < 0) {
              if (value == -1) {
                cmd.type = BfInstruction::PREV;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::PREV_N;
                cmd.value1 = -value;
                cmd.value2 = 0;
              }
            } else {
              continue;
            }
          } else {
            irCode.pop_back(); irCode.pop_back();
          }
        }
        break;
      case '<':
        {
          bool isNormalPrev = true;
          srcptr++;
          int value = compressInstruction<'<', '>'>(&srcptr) + 1;
          if (irCode.size() > 1) {
            BfInstruction::Command c1 = irCode[irCode.size() - 1];  // (+*|-*) ?
            BfInstruction::Command c2 = irCode[irCode.size() - 2];  // >* ?
            if (value == 1 && c2.type == BfInstruction::NEXT) {
              cmd.value1 = 1;
              isNormalPrev = genOperationAt(cmd, c1);
            } else if (value == -1 && c2.type == BfInstruction::PREV) {
              cmd.value1 = -1;
              isNormalPrev = genOperationAt(cmd, c1);
            } else if (value > 0 && c2.type == BfInstruction::NEXT_N && value == c2.value1) {
              cmd.value1 = c2.value1;
              isNormalPrev = genOperationAt(cmd, c1);
            } else if (value < 0 && c2.type == BfInstruction::PREV_N && -value == c2.value1) {
              cmd.value1 = -c2.value1;
              isNormalPrev = genOperationAt(cmd, c1);
            }
          }
          if (isNormalPrev) {
            if (value > 0) {
              if (value == 1) {
                cmd.type = BfInstruction::PREV;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::PREV_N;
                cmd.value1 = value;
                cmd.value2 = 0;
              }
            } else if (value < 0) {
              if (value == 1) {
                cmd.type = BfInstruction::NEXT;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::NEXT_N;
                cmd.value1 = -value;
                cmd.value2 = 0;
              }
            } else {
              continue;
            }
          } else {
            irCode.pop_back(); irCode.pop_back();
          }
        }
        break;
      case '+':
        {
          srcptr++;
          int value = compressInstruction<'+', '-'>(&srcptr) + 1;
          if (irCode.size() > 0 && irCode[irCode.size() - 1].type == BfInstruction::ASSIGN_ZERO) {
            cmd.type = BfInstruction::ASSIGN;
            cmd.value1 = value;
            cmd.value2 = 0;
            irCode.pop_back();
          } else {
            if (value > 0) {
              if (value == 1) {
                cmd.type = BfInstruction::INC;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::ADD;
                cmd.value1 = value;
                cmd.value2 = 0;
              }
            } else if (value < 0) {
              if (value == 1) {
                cmd.type = BfInstruction::DEC;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::SUB;
                cmd.value1 = -value;
                cmd.value2 = 0;
              }
            } else {
              continue;
            }
          }
        }
        break;
      case '-':
        {
          srcptr++;
          int value = compressInstruction<'-', '+'>(&srcptr) + 1;
          if (irCode.size() > 0 && irCode[irCode.size() - 1].type == BfInstruction::ASSIGN_ZERO) {
            cmd.type = BfInstruction::ASSIGN;
            cmd.value1 = -value;
            cmd.value2 = 0;
            irCode.pop_back();
          } else {
            if (value > 0) {
              if (value == 1) {
                cmd.type = BfInstruction::DEC;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::SUB;
                cmd.value1 = value;
                cmd.value2 = 0;
              }
            } else if (value < 0) {
              if (value == 1) {
                cmd.type = BfInstruction::INC;
                cmd.value1 = 0;
                cmd.value2 = 0;
              } else {
                cmd.type = BfInstruction::ADD;
                cmd.value1 = -value;
                cmd.value2 = 0;
              }
            } else {
              continue;
            }
          }
        }
        break;
      case '.':
        cmd.type = BfInstruction::PUTCHAR;
        cmd.value1 = 0;
        cmd.value2 = 0;
        break;
      case ',':
        cmd.type = BfInstruction::GETCHAR;
        cmd.value1 = 0;
        cmd.value2 = 0;
        break;
      case '[':
        cmd.type = BfInstruction::LOOP_START;
        loopStack.push(static_cast<unsigned int>(irCode.size()));
        break;
      case ']':
        {
          bool isNormalLoopEnd = true;
          BfIR::size_type size = irCode.size();
          if (size > 0 && irCode[size - 1].type == BfInstruction::LOOP_START) {
            cmd.type = BfInstruction::INF_LOOP;
            cmd.value1 = 0;
            cmd.value2 = 0;
            irCode.pop_back();
            isNormalLoopEnd = false;
          }
          if (size > 1 && irCode[size - 2].type == BfInstruction::LOOP_START) {
            BfInstruction::Command &c1 = irCode[size - 1];
            if (c1.type == BfInstruction::INC || c1.type == BfInstruction::DEC) {
              cmd.type = BfInstruction::ASSIGN_ZERO;
              cmd.value1 = 0;
              cmd.value2 = 0;
              irCode.pop_back(); irCode.pop_back();
              isNormalLoopEnd = false;
            } else if (isPtrOperation(c1.type)) {
              cmd.type = BfInstruction::SEARCH_ZERO;
              switch (c1.type) {
                case BfInstruction::NEXT:
                  cmd.value1 = 1;
                  break;
                case BfInstruction::PREV:
                  cmd.value1 = -1;
                  break;
                case BfInstruction::NEXT_N:
                  cmd.value1 = c1.value1;
                  break;
                case BfInstruction::PREV_N:
                  cmd.value1 = -c1.value1;
                  break;
              }
              cmd.value2 = 0;
              irCode.pop_back(); irCode.pop_back();
              isNormalLoopEnd = false;
            }
          }
          if (size > 2) {  // [->+<]
            BfInstruction::Command &c1 = irCode[size - 1];  // >*(+|-)*<* ? or - ?
            BfInstruction::Command &c2 = irCode[size - 2];  // - ? or >*(+|-)*<* ?
            BfInstruction::Command &c3 = irCode[size - 3];  // [ ?
            if (c3.type == BfInstruction::LOOP_START &&
                ((c2.type == BfInstruction::DEC && isOperationAt(c1.type)) ||
                 (isOperationAt(c2.type) && c1.type == BfInstruction::DEC))) {
              BfInstruction::Command &c = (c2.type == BfInstruction::DEC) ? c1 : c2;
              switch (c.type) {
                case BfInstruction::INC_AT:
                  cmd.type = BfInstruction::ADD_VAR;
                  cmd.value1 = c.value1;
                  cmd.value2 = 0;
                  irCode.pop_back(); irCode.pop_back(); irCode.pop_back();
                  isNormalLoopEnd = false;
                  break;
                case BfInstruction::DEC_AT:
                  cmd.type = BfInstruction::SUB_VAR;
                  cmd.value1 = c.value1;
                  cmd.value2 = 0;
                  irCode.pop_back(); irCode.pop_back(); irCode.pop_back();
                  isNormalLoopEnd = false;
                  break;
                case BfInstruction::ADD_AT:
                  cmd.type = BfInstruction::CMUL_VAR;
                  cmd.value1 = c.value1;
                  cmd.value2 = c.value2;
                  irCode.pop_back(); irCode.pop_back(); irCode.pop_back();
                  isNormalLoopEnd = false;
                  break;
              }
            }
          }
          if (isNormalLoopEnd) {
            cmd.type = BfInstruction::LOOP_END;
            cmd.value1 = loopStack.top();
            cmd.value2 = 0;
            irCode[loopStack.top()].value1 = static_cast<int>(irCode.size());
          }
          loopStack.pop();
        }
        break;
    }
    irCode.push_back(cmd);
  }
}


}  // namespace bf




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


inline static bool
genOperationAt(bf::BfInstruction::Command &cmd, const bf::BfInstruction::Command &c1)
{
  switch (c1.type) {
    case bf::BfInstruction::INC:
      cmd.type = bf::BfInstruction::INC_AT;
      cmd.value2 = 0;
      return false;
    case bf::BfInstruction::DEC:
      cmd.type = bf::BfInstruction::DEC_AT;
      cmd.value2 = 0;
      return false;
    case bf::BfInstruction::ADD:
      cmd.type = bf::BfInstruction::ADD_AT;
      cmd.value2 = c1.value1;
      return false;
    case bf::BfInstruction::SUB:
      cmd.type = bf::BfInstruction::SUB_AT;
      cmd.value2 = c1.value1;
      return false;
    case bf::BfInstruction::ASSIGN_ZERO:
      cmd.type = bf::BfInstruction::ASSIGN_AT;
      cmd.value2 = 0;
      return false;
    case bf::BfInstruction::ASSIGN:
      cmd.type = bf::BfInstruction::ASSIGN_AT;
      cmd.value2 = c1.value1;
      return false;
    default:
      return true;
  }
}


inline static bool
isPtrOperation(bf::BfInstruction::Instruction inst)
{
  return (inst == bf::BfInstruction::NEXT || inst == bf::BfInstruction::NEXT_N ||
      inst == bf::BfInstruction::PREV || inst == bf::BfInstruction::PREV_N);
}


inline static bool
isOperationAt(bf::BfInstruction::Instruction inst)
{
  return (inst == bf::BfInstruction::INC_AT || inst == bf::BfInstruction::DEC_AT ||
      inst == bf::BfInstruction::ADD_AT || inst == bf::BfInstruction::SUB_AT);
}
