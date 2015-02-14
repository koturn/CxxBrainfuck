/*!
 * @file BfJitCompiler.cpp
 * @brief Brainfuck JIT-compiler
 * @author koturn
 */
#include <stack>
#include "BfJitCompiler.h"


#ifdef USE_XBYAK
typedef enum {
  B, F
} Direction;

inline static std::string
toStr(int labelNo, Direction dir);


namespace bf {


/*!
 * @brief Compile brainfuck IR code with Xbyak JIT-compile
 */
void
BfJitCompiler::compile(void)
{
#ifdef XBYAK32
  const Xbyak::Reg32 &pPutchar(esi);
  const Xbyak::Reg32 &pGetchar(edi);
  const Xbyak::Reg32 &stack(ebp);
  const Xbyak::Address cur = dword[stack];
  push(ebp);  // stack
  push(esi);
  push(edi);
  const int P_ = 4 * 3;
  mov(pPutchar, ptr[esp + P_ + 4]);  // putchar
  mov(pGetchar, ptr[esp + P_ + 8]);  // getchar
  mov(stack, ptr[esp + P_ + 12]);  // stack
#elif defined(XBYAK64_WIN)
  const Xbyak::Reg64 &pPutchar(rsi);
  const Xbyak::Reg64 &pGetchar(rdi);
  const Xbyak::Reg64 &stack(rbp);  // stack
  const Xbyak::Address cur = dword[stack];
  push(rsi);
  push(rdi);
  push(rbp);
  mov(pPutchar, rcx);  // putchar
  mov(pGetchar, rdx);  // getchar
  mov(stack, r8);  // stack
#else
  const Xbyak::Reg64& pPutchar(rbx);
  const Xbyak::Reg64& pGetchar(rbp);
  const Xbyak::Reg64& stack(r12);  // stack
  const Xbyak::Address cur = dword[stack];
  push(rbx);
  push(rbp);
  push(r12);
  mov(pPutchar, rdi);  // putchar
  mov(pGetchar, rsi);  // getchar
  mov(stack, rdx);  // stack
#endif  // XBYAK32
  int labelNo = 0;
  std::stack<int> keepLabelNo;
  for (BfIR::const_iterator cmd = irCode.begin(), end = irCode.end(); cmd != end; cmd++) {
    switch (cmd->type) {
      case BfInstruction::NEXT:
        add(stack, 4);
        break;
      case BfInstruction::PREV:
        sub(stack, 4);
        break;
      case BfInstruction::NEXT_N:
        add(stack, 4 * cmd->value);
        break;
      case BfInstruction::PREV_N:
        sub(stack, 4 * cmd->value);
        break;
      case BfInstruction::INC:
        inc(cur);
        break;
      case BfInstruction::DEC:
        dec(cur);
        break;
      case BfInstruction::ADD:
        add(cur, cmd->value);
        break;
      case BfInstruction::SUB:
        sub(cur, cmd->value);
        break;
      case BfInstruction::PUTCHAR:
#ifdef XBYAK32
        push(cur);
        call(pPutchar);
        pop(eax);
#elif defined(XBYAK64_WIN)
        mov(rcx, cur);
        sub(rsp, 32);
        call(pPutchar);
        add(rsp, 32);
#else
        mov(rdi, cur);
        call(pPutchar);
#endif  // XBYAK32
        break;
      case BfInstruction::GETCHAR:
#if defined(XBYAK32) || defined(XBYAK64_GCC)
        call(pGetchar);
        mov(cur, eax);
#elif defined(XBYAK64_WIN)
        sub(rsp, 32);
        call(pGetchar);
        add(rsp, 32);
        mov(cur, rax);
#endif  // defined(XBYAK32) || defined(XBYAK64_GCC)
        break;
      case BfInstruction::LOOP_START:
        L(toStr(labelNo, B));
        mov(eax, cur);
        test(eax, eax);
        jz(toStr(labelNo, F), Xbyak::CodeGenerator::T_NEAR);
        keepLabelNo.push(labelNo++);
        break;
      case BfInstruction::LOOP_END:
        {
          int no = keepLabelNo.top();
          keepLabelNo.pop();
          jmp(toStr(no, B));
          L(toStr(no, F));
        }
        break;
      case BfInstruction::ASSIGN:
        mov(cur, cmd->value);
        break;
      case BfInstruction::ADD_VAR:
        // LOOP_START
        L(toStr(labelNo, B));
        mov(eax, cur);
        test(eax, eax);
        jz(toStr(labelNo, F), Xbyak::CodeGenerator::T_NEAR);
        keepLabelNo.push(labelNo++);
        // SUB
        dec(cur);
        // PTR_ADD
        add(stack, 4 * static_cast<int>(cmd->value));
        // ADD
        inc(cur);
        // PTR_SUB
        sub(stack, 4 * static_cast<int>(cmd->value));
        // LOOP_END
        {
          int no = keepLabelNo.top();
          keepLabelNo.pop();
          jmp(toStr(no, B));
          L(toStr(no, F));
        }
        break;
      case BfInstruction::SUB_VAR:
        // LOOP_START
        L(toStr(labelNo, B));
        mov(eax, cur);
        test(eax, eax);
        jz(toStr(labelNo, F), Xbyak::CodeGenerator::T_NEAR);
        keepLabelNo.push(labelNo++);
        // SUB
        dec(cur);
        // PTR_ADD
        add(stack, 4 * static_cast<int>(cmd->value));
        // SUB
        dec(cur);
        // PTR_SUB
        sub(stack, 4 * static_cast<int>(cmd->value));
        // LOOP_END
        {
          int no = keepLabelNo.top();
          keepLabelNo.pop();
          jmp(toStr(no, B));
          L(toStr(no, F));
        }
        break;
      default:
        break;
    }
  }
#ifdef XBYAK32
  pop(edi);
  pop(esi);
  pop(ebp);
#elif defined(XBYAK64_WIN)
  pop(rbp);
  pop(rdi);
  pop(rsi);
#else
  pop(r12);
  pop(rbp);
  pop(rbx);
#endif  // XBYAK32
  ret();
}


}  // namespace bf




/*!
 * @brief Convert label to string
 * @param [in] labelNo  Label Number
 * @param [in] dir      Direction (Backword or Forward)
 * @return Label in string format
 */
inline static std::string
toStr(int labelNo, Direction dir)
{
  return Xbyak::Label::toStr(labelNo) + (dir == B ? 'B' : 'F');
}
#endif  // USE_XBYAK
