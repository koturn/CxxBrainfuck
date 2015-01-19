#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>

#ifdef USE_XBYAK
#  if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#    define  XBYAK_NO_OP_NAMES
#  endif
#  include <xbyak/xbyak.h>
#endif  // USE_XBYAK
#include "Brainfuck.h"
#include "CodeGenerator/_AllGenerator.h"


static const char *
findLoopEnd(const char *srcptr);

static const char *
findLoopStart(const char *srcptr);

static unsigned int
countChar(const char *srcptr, char ch);

#ifdef USE_XBYAK
typedef enum {
  B, F
} Direction;

inline static std::string
toStr(int labelNo, Direction dir);
#endif  // USE_XBYAK




/* ========================================================================= *
 * Public members                                                            *
 * ========================================================================= */
/*!
 * @brief Destructor: Delete alocated memory
 */
Brainfuck::~Brainfuck(void)
{
  delete[] sourceBuffer;
#ifdef USE_XBYAK
  delete[] xbyakRtStack;
#endif  // USE_XBYAK
}


/*!
 * @brief Load Brainfuck source code
 * @param [in] filename  File path
 */
void
Brainfuck::load(const char *filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    throw "Cannot open file";
  }
  ifs.seekg(0, ifs.end);
  std::size_t fileSize = static_cast<std::size_t>(ifs.tellg());
  ifs.seekg(0, ifs.beg);

  sourceBuffer = new char[fileSize];
  sourceBuffer[fileSize - 1] = '\0';
  ifs.read(sourceBuffer, fileSize);
}


/*!
 * @brief [in] Delete not Brainfuck-characters
 */
void
Brainfuck::trim(void)
{
  const char *srcptr = sourceBuffer;
  char *dstptr = sourceBuffer;
  for (; *srcptr != '\0'; srcptr++) {
    switch (*srcptr) {
      case '>':
      case '<':
      case '+':
      case '-':
      case '.':
      case ',':
      case '[':
      case ']':
        *dstptr++ = *srcptr;
        break;
    }
  }
  *dstptr = '\0';
}


/*!
 * @brief Compile brainfuck source code
 * @param [in] compileType  Type of compile
 */
void
Brainfuck::compile(CompileType compileType)
{
  switch (compileType) {
    case NO_COMPILE:
      break;
    case NORMAL_COMPILE:
      normalCompile();
      break;
#ifdef USE_XBYAK
    case XBYAK_JIT_COMPILE:
      xbyakJitCompile();
      break;
#endif  // USE_XBYAK
  }
}


/*!
 * @brief Execute brainfuck source code
 */
void
Brainfuck::execute(void)
{
  switch (compileType) {
    case NO_COMPILE:
      interpretExecute();
      break;
    case NORMAL_COMPILE:
      compileExecute();
      break;
#ifdef USE_XBYAK
    case XBYAK_JIT_COMPILE:
      xbyakJitExecute();
      break;
#endif  // USE_XBYAK
  }
  std::cout.put('\n');
}


#ifdef USE_XBYAK
/*!
 * @brief Dump jit-compiled binaries in C-source code
 */
void
Brainfuck::xbyakDump(void)
{
  const Xbyak::uint8 *code = generator.getCode();
  std::size_t size = generator.getSize();
  std::cout << "#include <stdio.h>\n"
            << "#include <stdlib.h>\n"
#ifdef __linux__
            << "#include <unistd.h>\n"
            << "#include <sys/mman.h>\n"
#endif
            << "\n"
            << "static int stack[128 * 1024];\n"
            << "static const unsigned char code[] = {\n";
  std::cout << std::hex << " ";
  for (std::size_t i = 0; i < size; i++) {
    std::cout << " 0x" << std::setfill('0') << std::setw(2)
              << static_cast<unsigned int>(code[i]) << ",";
    if (i % 16 == 15) {
      std::cout << "\n ";
    }
  }
  std::cout << std::dec
            << "\n};\n\n\n"
            << "int\n"
            << "main(void)\n"
            << "{\n"
#ifdef __linux__
            << "  long pageSize = sysconf(_SC_PAGESIZE) - 1;\n"
            << "  mprotect((void *) code, (sizeof(code) + pageSize) & ~pageSize, PROT_READ | PROT_EXEC);\n"
#endif
            << "  ((void (*)(void *, void *, int *)) code)((void *) putchar, (void *) getchar, stack);\n"
            << "  return EXIT_SUCCESS;\n"
            << "}"
            << std::endl;
}
#endif  // USE_XBYAK


void
Brainfuck::translate(LANG lang)
{
  switch (lang) {
    case LANG_C:
      {
        GeneratorC cg("  ");
        generateCode(cg);
      }
      break;
    case LANG_CPP:
      {
        GeneratorCpp cg("  ");
        generateCode(cg);
      }
      break;
    case LANG_CSHARP:
      {
        GeneratorCSharp cg("    ");
        generateCode(cg);
      }
      break;
    case LANG_JAVA:
      {
        GeneratorJava cg("    ");
        generateCode(cg);
      }
      break;
    case LANG_LUA:
      {
        GeneratorLua cg("    ");
        generateCode(cg);
      }
      break;
    case LANG_PYTHON:
      {
        GeneratorPython cg("    ");
        generateCode(cg);
      }
      break;
    case LANG_RUBY:
      {
        GeneratorRuby cg("  ");
        generateCode(cg);
      }
      break;
  }
}




/* ========================================================================= *
 * Private members                                                           *
 * ========================================================================= */
/*!
 * @brief Compile brainfuck code to Instruction set
 */
void
Brainfuck::normalCompile(void)
{
  std::stack<unsigned int> loopStack;
  for (const char *srcptr = sourceBuffer; *srcptr != '\0'; srcptr++) {
    Command cmd;
    switch (*srcptr) {
      case '>':
        cmd.type = PTR_ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '<':
        cmd.type = PTR_SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '+':
        cmd.type = ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '-':
        cmd.type = SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '.':
        cmd.type = PUTCHAR;
        cmd.value = 0;
        break;
      case ',':
        cmd.type = GETCHAR;
        cmd.value = 0;
        break;
      case '[':
        cmd.type = LOOP_START;
        loopStack.push(static_cast<unsigned int>(commands.size()));
        break;
      case ']':
        cmd.type = LOOP_END;
        cmd.value = loopStack.top();
        commands[loopStack.top()].value = static_cast<unsigned int>(commands.size() - 1);
        loopStack.pop();
        break;
    }
    commands.push_back(cmd);
  }
  compileType = NORMAL_COMPILE;
}


/*!
 * @brief Execute brainfuck without compile.
 */
void
Brainfuck::interpretExecute(void)
{
  unsigned char *memory = new unsigned char[memorySize];
  std::memset(memory, 0, memorySize);
  unsigned char *ptr = memory;
  for (const char *srcptr = sourceBuffer; *srcptr != '\0'; srcptr++) {
    switch (*srcptr) {
      case '>': ptr++;    break;
      case '<': ptr--;    break;
      case '+': (*ptr)++; break;
      case '-': (*ptr)--; break;
      case '.':
        std::cout.put(static_cast<char>(*ptr));
        break;
      case ',':
        *ptr = static_cast<unsigned char>(std::cin.get());
        break;
      case '[':
        if (*ptr != 0) break;
        srcptr = findLoopEnd(srcptr);
        break;
      case ']':
        if (*ptr == 0) break;
        srcptr = findLoopStart(srcptr);
        break;
    }
  }
  delete[] memory;
}


/*!
 * @brief Execute compiled brainfuck source code
 */
void
Brainfuck::compileExecute(void)
{
  unsigned char *memory = new unsigned char[memorySize];
  std::memset(memory, 0, memorySize);
  unsigned char *ptr = memory;
  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        ptr += commands[pc].value;
        break;
      case PTR_SUB:
        ptr -= commands[pc].value;
        break;
      case ADD:
        *ptr += commands[pc].value;
        break;
      case SUB:
        *ptr -= commands[pc].value;
        break;
      case PUTCHAR:
        std::cout.put(static_cast<char>(*ptr));
        break;
      case GETCHAR:
        *ptr = static_cast<unsigned char>(std::cin.get());
        break;
      case LOOP_START:
        if (*ptr == 0) {
          pc = commands[pc].value;
        }
        break;
      case LOOP_END:
        if (*ptr != 0) {
          pc = commands[pc].value;
        }
        break;
    }
  }
  delete[] memory;
}


#ifdef USE_XBYAK
/*!
 * @brief Compile brainfuck source code with Xbyak JIT-compile
 */
void
Brainfuck::xbyakJitCompile(void)
{
#ifdef XBYAK32
  const Xbyak::Reg32 &pPutchar(generator.esi);
  const Xbyak::Reg32 &pGetchar(generator.edi);
  const Xbyak::Reg32 &stack(generator.ebp);
  const Xbyak::Address cur = generator.dword[stack];
  generator.push(generator.ebp);  // stack
  generator.push(generator.esi);
  generator.push(generator.edi);

  const int P_ = 4 * 3;
  generator.mov(pPutchar, ptr[esp + P_ + 4]);  // putchar
  generator.mov(pGetchar, ptr[esp + P_ + 8]);  // getchar
  generator.mov(stack, ptr[esp + P_ + 12]);  // stack
#elif defined(XBYAK64_WIN)
  const Xbyak::Reg64 &pPutchar(generator.rsi);
  const Xbyak::Reg64 &pGetchar(generator.rdi);
  const Xbyak::Reg64 &stack(generator.rbp);  // stack
  const Xbyak::Address cur = generator.dword[stack];
  generator.push(generator.rsi);
  generator.push(generator.rdi);
  generator.push(generator.rbp);
  generator.mov(pPutchar, generator.rcx);  // putchar
  generator.mov(pGetchar, generator.rdx);  // getchar
  generator.mov(stack, generator.r8);  // stack
#else
  const Xbyak::Reg64& pPutchar(generator.rbx);
  const Xbyak::Reg64& pGetchar(generator.rbp);
  const Xbyak::Reg64& stack(generator.r12);  // stack
  const Xbyak::Address cur = generator.dword[stack];
  generator.push(generator.rbx);
  generator.push(generator.rbp);
  generator.push(generator.r12);
  generator.mov(pPutchar, generator.rdi);  // putchar
  generator.mov(pGetchar, generator.rsi);  // getchar
  generator.mov(stack, generator.rdx);  // stack
#endif  // XBYAK32
  int labelNo = 0;
  std::stack<int> keepLabelNo;
  for (const char *srcptr = sourceBuffer; *srcptr != '\0'; srcptr++) {
    switch (*srcptr) {
      case '+':
      case '-':
        {
          int cnt = countChar(srcptr, *srcptr);
          if (cnt == 1) {
            *srcptr == '+' ? generator.inc(cur) : generator.dec(cur);
          } else {
            generator.add(cur, (*srcptr == '+' ? cnt : -cnt));
          }
          srcptr += cnt - 1;
        }
        break;
      case '>':
      case '<':
        {
          int cnt = countChar(srcptr, *srcptr);
          generator.add(stack, 4 * (*srcptr == '>' ? cnt : -cnt));
          srcptr += cnt - 1;
        }
        break;
      case '.':
#ifdef XBYAK32
        generator.push(cur);
        generator.call(pPutchar);
        generator.pop(generator.eax);
#elif defined(XBYAK64_WIN)
        generator.mov(generator.rcx, cur);
        generator.sub(generator.rsp, 32);
        generator.call(pPutchar);
        generator.add(generator.rsp, 32);
#else
        generator.mov(generator.rdi, cur);
        generator.call(pPutchar);
#endif  // XBYAK32
        break;
      case ',':
#if defined(XBYAK32) || defined(XBYAK64_GCC)
        generator.call(pGetchar);
        generator.mov(cur, generator.eax);
#elif defined(XBYAK64_WIN)
        generator.sub(generator.rsp, 32);
        generator.call(pGetchar);
        generator.add(generator.rsp, 32);
        generator.mov(cur, generator.rax);
#endif  // defined(XBYAK32) || defined(XBYAK64_GCC)
        break;
      case '[':
        generator.L(toStr(labelNo, B));
        generator.mov(generator.eax, cur);
        generator.test(generator.eax, generator.eax);
        generator.jz(toStr(labelNo, F), Xbyak::CodeGenerator::T_NEAR);
        keepLabelNo.push(labelNo++);
        break;
      case ']':
        {
          int no = keepLabelNo.top();
          keepLabelNo.pop();
          generator.jmp(toStr(no, B));
          generator.L(toStr(no, F));
        }
        break;
      default:
        break;
    }
  }
#ifdef XBYAK32
  generator.pop(generator.edi);
  generator.pop(generator.esi);
  generator.pop(generator.ebp);
#elif defined(XBYAK64_WIN)
  generator.pop(generator.rbp);
  generator.pop(generator.rdi);
  generator.pop(generator.rsi);
#else
  generator.pop(generator.r12);
  generator.pop(generator.rbp);
  generator.pop(generator.rbx);
#endif  // XBYAK32
  generator.ret();

  compileType = XBYAK_JIT_COMPILE;
}


/*!
 * @brief Execute jit compiled binary
 */
void
Brainfuck::xbyakJitExecute(void)
{
  xbyakRtStack = new int[xbyakRtStackSize];
  std::memset(xbyakRtStack, 0, xbyakRtStackSize);
  generator.getCode<void (*)(void *, void *, int *)>()
    (Xbyak::CastTo<void *>(putchar), Xbyak::CastTo<void *>(getchar), xbyakRtStack);
}
#endif  // USE_XBYAK


void
Brainfuck::generateCode(CodeGenerator &cg)
{
  if (commands.size() == 0 && compileType != NO_COMPILE) {
    compile();
  }
  cg.printHeader();
  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        cg.printPtrAdd(commands[pc].value);
        break;
      case PTR_SUB:
        cg.printPtrSub(commands[pc].value);
        break;
      case ADD:
        cg.printAdd(commands[pc].value);
        break;
      case SUB:
        cg.printSub(commands[pc].value);
        break;
      case PUTCHAR:
        cg.printPutchar();
        break;
      case GETCHAR:
        cg.printGetchar();
        break;
      case LOOP_START:
        cg.printLoopStart();
        break;
      case LOOP_END:
        cg.printLoopEnd();
        break;
    }
  }
  cg.printFooter();
}




/* ========================================================================= *
 * Local functions                                                           *
 * ========================================================================= */
/*!
 * @brief Find the end of loop
 * @param [in] code  Pointer to the brainfuck source code
 * @return  Pointer to the end of loop of brainfuck source code
 */
static const char *
findLoopEnd(const char *srcptr)
{
  char ch;
  int depth = 1;
  while (ch = *++srcptr, depth > 0) {
    switch (ch) {
      case '[':  depth++; break;
      case ']':  depth--; break;
      case '\0': throw "Cannod find the end of loop";
    }
  }
  return srcptr - 1;
}


/*!
 * @brief Find the start of loop
 * @param [in] code  Pointer to the brainfuck source code
 * @return  Pointer to the start of loop of brainfuck source code
 */
static const char *
findLoopStart(const char *srcptr)
{
  char ch;
  int depth = 1;
  while (ch = *--srcptr, depth > 0) {
    switch (ch) {
      case '[': depth--; break;
      case ']': depth++; break;
    }
  }
  return srcptr;
}


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


#ifdef USE_XBYAK
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
