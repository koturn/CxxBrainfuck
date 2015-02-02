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
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#  include <cstdio>
#  ifdef NOMINMAX
#    define NOMINMAX
#    define BRAINFUCK_NOMONMAX_IS_NOT_DEFINED
#  endif
#  ifdef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define BRAINFUCK_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#  endif
#  include <windows.h>
#  ifdef BRAINFUCK_NOMONMAX_IS_NOT_DEFINED
#    undef NOMINMAX
#  endif
#  ifdef BRAINFUCK_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WIN32_LEAN_AND_MEAN
#  endif
#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)


static const char *
findLoopEnd(const char *srcptr);

static const char *
findLoopStart(const char *srcptr);

static unsigned int
countChar(const char *srcptr, char ch);


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
inline static void
writePEHeader(unsigned char *bin, std::size_t codeSize);

inline static void
writeIData(unsigned char *bin);
#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)


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
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
  delete[] exeBin;
#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
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
  std::size_t size = generator.getSize();
  if (size == 0) {
    xbyakJitCompile();
    size = generator.getSize();
  }
  const Xbyak::uint8 *code = generator.getCode();

  std::cout << "#include <stdio.h>\n"
               "#include <stdlib.h>\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
               "#ifndef NOMINMAX\n"
               "#  define NOMINMAX\n"
               "#  define NOMINMAX_IS_NOT_DEFINED\n"
               "#endif\n"
               "#ifdef WIN32_LEAN_AND_MEAN\n"
               "#  define WIN32_LEAN_AND_MEAN\n"
               "#  define WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
               "#endif\n"
               "#include <windows.h>\n"
               "#ifdef NOMONMAX_IS_NOT_DEFINED\n"
               "#  undef NOMINMAX\n"
               "#endif\n"
               "#undef NOMONMAX_IS_NOT_DEFINED\n"
               "#ifdef LEAN_AND_MEAN_IS_NOT_DEFINED\n"
               "#  undef WIN32_LEAN_AND_MEAN\n"
               "#endif\n"
               "#undef LEAN_AND_MEAN_IS_NOT_DEFINED\n"
#elif defined(__linux__)
               "#include <unistd.h>\n"
               "#include <sys/mman.h>\n"
#endif
               "\n"
               "static int stack[128 * 1024];\n"
               "static const unsigned char code[] = {\n"
            << std::hex << " ";
  for (std::size_t i = 0; i < size; i++) {
    std::cout << " 0x" << std::setfill('0') << std::setw(2)
              << static_cast<unsigned int>(code[i]) << ",";
    if (i % 16 == 15) {
      std::cout << "\n ";
    }
  }
  std::cout << std::dec
            << "\n};\n\n\n"
               "int\n"
               "main(void)\n"
               "{\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
               "  DWORD old_protect;\n"
               "  VirtualProtect((void *) code, sizeof(code), PAGE_EXECUTE_READWRITE, &old_protect);\n"
#elif defined(__linux__)
               "  long page_size = sysconf(_SC_PAGESIZE) - 1;\n"
               "  mprotect((void *) code, (sizeof(code) + page_size) & ~page_size, PROT_READ | PROT_EXEC);\n"
#endif
               "  ((void (*)(void *, void *, int *)) code)((void *) putchar, (void *) getchar, stack);\n"
               "  return EXIT_SUCCESS;\n"
               "}"
            << std::endl;
}
#endif  // USE_XBYAK


/*!
 * @brief Translate brainfuck to otehr languages
 * @param [in] lang  Constant of language
 */
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


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
/*!
 * @brief Generate executable Windows binary
 * @param [in] wbt  Binary type
 */
void
Brainfuck::generateWinBinary(WinBinType wbt)
{
  switch (wbt) {
    case WIN_BIN_X86:
      generateX86WinBinary();
      break;
  }
}
#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)




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
        if (srcptr[1] == '-' && srcptr[2] == ']') {
          srcptr += 2;
          cmd.type = ASSIGN_ZERO;
        } else {
          cmd.type = LOOP_START;
          loopStack.push(static_cast<unsigned int>(commands.size()));
        }
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
Brainfuck::interpretExecute(void) const
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
Brainfuck::compileExecute(void) const
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
      case ASSIGN_ZERO:
        *ptr = 0;
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
  generator.mov(pPutchar, generator.ptr[generator.esp + P_ + 4]);  // putchar
  generator.mov(pGetchar, generator.ptr[generator.esp + P_ + 8]);  // getchar
  generator.mov(stack, generator.ptr[generator.esp + P_ + 12]);  // stack
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
        {
          int cnt = countChar(srcptr, *srcptr);
          if (cnt == 1) {
            generator.inc(cur);
          } else {
            generator.add(cur, cnt);
          }
          srcptr += cnt - 1;
        }
        break;
      case '-':
        {
          int cnt = countChar(srcptr, *srcptr);
          if (cnt == 1) {
            generator.dec(cur);
          } else {
            generator.sub(cur, cnt);
          }
          srcptr += cnt - 1;
        }
        break;
      case '>':
        {
          int cnt = countChar(srcptr, *srcptr);
          generator.add(stack, 4 * cnt);
          srcptr += cnt - 1;
        }
        break;
      case '<':
        {
          int cnt = countChar(srcptr, *srcptr);
          generator.sub(stack, 4 * cnt);
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
        if (srcptr[1] == '-' && srcptr[2] == ']') {
          srcptr += 2;
          generator.mov(cur, 0);
        } else {
          generator.L(toStr(labelNo, B));
          generator.mov(generator.eax, cur);
          generator.test(generator.eax, generator.eax);
          generator.jz(toStr(labelNo, F), Xbyak::CodeGenerator::T_NEAR);
          keepLabelNo.push(labelNo++);
        }
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
    (Xbyak::CastTo<void *>(std::putchar), Xbyak::CastTo<void *>(std::getchar), xbyakRtStack);
}
#endif  // USE_XBYAK


/*!
 * @brief Generate code of other programming language
 * @param [in] cg  Code generator of target language
 */
template<class TCodeGenerator>
void
Brainfuck::generateCode(TCodeGenerator &cg)
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
      case ASSIGN_ZERO:
        cg.printAssignZero();
        break;
    }
  }
  cg.printFooter();
}


#if defined(_WIN32) || defined(_WIN64) || (__CYGWIN__)
/*!
 * @brief Generate X86 Executable Windows binary
 */
void
Brainfuck::generateX86WinBinary(void)
{
  static const unsigned int HEADER_OFFSET = 0x400;
  static const unsigned int EXE_SIZE = 16 * 1024 + HEADER_OFFSET;
  const int ADDR_PUTCHAR = 0x00405044;
  const int ADDR_GETCHAR = ADDR_PUTCHAR + 4;
  const int ADDR_BUF = 0x00406000;
  std::stack<int> loopStack;

  delete[] exeBin;
  exeBin = new unsigned char[EXE_SIZE];
  std::memset(exeBin, 0, EXE_SIZE);

  if (commands.size() == 0 && compileType != NO_COMPILE) {
    compile();
  }
  exeBin[HEADER_OFFSET] = 0x31; exeBin[HEADER_OFFSET + 1] = 0xc9;  // xor ecx, ecx
  exeBin[HEADER_OFFSET + 2] = 0x57;  // push edi
  exeBin[HEADER_OFFSET + 3] = 0xbf; *reinterpret_cast<int *>(&exeBin[HEADER_OFFSET + 4]) = ADDR_BUF;  // mov edi, addr_buf

  int idx = HEADER_OFFSET + 8;
  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          exeBin[idx] = 0x66; exeBin[idx + 1] = 0x41;  // inc cx
          idx += 2;
        }
        break;
      case PTR_SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          exeBin[idx] = 0x66; exeBin[idx + 1] = 0x49;  // dec cx
          idx += 2;
        }
        break;
      case ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          exeBin[idx] = 0xfe; exeBin[idx + 1] = 0x04; exeBin[idx + 2] = 0x0f;  // inc byte [edi+ecx]
          idx += 3;
        }
        break;
      case SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          exeBin[idx] = 0xfe; exeBin[idx + 1] = 0x0c; exeBin[idx + 2] = 0x0f;  // dec byte [edi+ecx]
          idx += 3;
        }
        break;
      case PUTCHAR:
        exeBin[idx] = 0x51;  // push ecx
        *reinterpret_cast<int *>(&exeBin[idx + 1]) = 0x0f04b60f;  // movzx eax,byte [edi+ecx]
        exeBin[idx + 5] = 0x50;  // push eax
        exeBin[idx + 6] = 0xa1; *reinterpret_cast<int *>(&exeBin[idx + 7]) = ADDR_PUTCHAR;  // mov eax, [addr_putchar]
        exeBin[idx + 11] = 0xff; exeBin[idx + 12] = 0xd0;  // call eax
        exeBin[idx + 13] = 0x58;  // pop eax
        exeBin[idx + 14] = 0x59;  // pop ecx
        idx += 15;
        break;
      case GETCHAR:
        exeBin[idx] = 0x51;  // push ecx
        exeBin[idx + 1] = 0xa1; *reinterpret_cast<int *>(&exeBin[idx + 2]) = ADDR_GETCHAR;  // mov eax, [addr_getchar]
        exeBin[idx + 6] = 0xff; exeBin[idx + 7] = 0xd0;  // call eax
        exeBin[idx + 8] = 0x59;  // pop ecx
        exeBin[idx + 9] = 0x88; exeBin[idx + 10] = 0x04; exeBin[idx + 11] = 0x0f;  // mov [edi+ecx],al
        idx += 12;
        break;
      case LOOP_START:
        loopStack.push(idx);
        *reinterpret_cast<int *>(&exeBin[idx]) = 0x000f3c80;  // cmp byte [edi+ecx], 0
        // jz Jump to just behind the corresponding ] (define address later)
        exeBin[idx + 4] = 0x0f; exeBin[idx + 5] = 0x84;
        idx += 10;
        break;
      case LOOP_END:
        {
          int idxLoop = loopStack.top();
          loopStack.pop();
          exeBin[idx] = 0xe9; *reinterpret_cast<int *>(&exeBin[idx + 1]) = idxLoop - (idx + 5);  // jmp idxLoop
          idx += 5;
          *reinterpret_cast<int *>(&exeBin[idxLoop + 6]) = idx - (idxLoop + 10);  // Rewrites the top of the loop
        }
        break;
      case ASSIGN_ZERO:
        // LOOP_START
        pc += 2;
        loopStack.push(idx);
        *reinterpret_cast<int *>(&exeBin[idx]) = 0x000f3c80;  // cmp byte [edi+ecx], 0
        exeBin[idx + 4] = 0x0f; exeBin[idx + 5] = 0x84;
        idx += 10;
        // SUB
        exeBin[idx] = 0xfe; exeBin[idx + 1] = 0x0c; exeBin[idx + 2] = 0x0f;  // dec byte [edi+ecx]
        idx += 3;
        // LOOP_END
        {
          int idxLoop = loopStack.top();
          loopStack.pop();
          exeBin[idx] = 0xe9; *reinterpret_cast<int *>(&exeBin[idx + 1]) = idxLoop - (idx + 5);  // jmp idxLoop
          idx += 5;
          *reinterpret_cast<int *>(&exeBin[idxLoop + 6]) = idx - (idxLoop + 10);  // Rewrites the top of the loop
        }
        break;
    }
    if (static_cast<std::size_t>(idx) > sizeof(exeBin) - 32) {
      throw "Output size has been exceeded\n";
    }
  }
  exeBin[idx] = 0x5f;  // pop edi
  exeBin[idx + 1] = 0x31; exeBin[idx + 2] = 0xc0;  // xor eax, eax
  exeBin[idx + 3] = 0xc3;  // ret

  writePEHeader(exeBin, EXE_SIZE - HEADER_OFFSET);
  writeIData(exeBin);
  exeBinSize = EXE_SIZE;
}
#endif  // defined(_WIN32) || defined(_WIN64) || (__CYGWIN__)




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


#if defined(_WIN32) || defined(_WIN64) || (__CYGWIN__)
/*!
 * @brief Write PE Header to array
 * @param [out] exeBin    Destination executable binary array
 * @param [in]  codeSize  Size of code
 */
inline static void
writePEHeader(unsigned char *exeBin, std::size_t codeSize)
{
  static const unsigned char STUB[] = {
    // 00-3b: DOS Header
    'M', 'Z',   0x50, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00,
    0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 3c-3f: Pointer to PE Header (=80)
    0x80, 0x00, 0x00, 0x00,
    // 40-7f: DOS Stub
    0xba, 0x10, 0x00, 0x0e, 0x1f, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x90, 0x90,
    'T', 'h', 'i', 's', ' ', 'p', 'r', 'o', 'g', 'r', 'a', 'm', ' ', 'c', 'a', 'n',
    'n', 'o', 't', ' ', 'b', 'e', ' ', 'r', 'u', 'n', ' ', 'i', 'n', ' ', 'D', 'O',
    'S', ' ', 'm', 'o', 'd', 'e', '.', '\r', '\n', '$', 0, 0, 0, 0, 0, 0,
    // 80-83: PE Signature
    'P', 'E', 0, 0
  };
  const static IMAGE_FILE_HEADER COFF = {
    0x014c, 3, 0, 0, 0, sizeof(IMAGE_OPTIONAL_HEADER32), 0x030f
  };
  unsigned char *ptr = exeBin;
  std::memcpy(ptr, STUB, sizeof(STUB));
  ptr += sizeof(STUB);
  std::memcpy(ptr, &COFF, sizeof(COFF));
  ptr += sizeof(COFF);

  IMAGE_OPTIONAL_HEADER32 opt = {
    0x010b,  // Magic
    6, 0,  // MajorLinkerVersion, MinorLinkerVersion
    static_cast<DWORD>(codeSize),  // SizeOfCode
    0,  // SizeOfInitializedData
    65536,  // SizeOfUninitializedData
    0x1000,  // AddressOfEntryPoint
    0x1000,  // BaseOfCode
    0x6000,  // BaseOfData
    0x00400000,  // ImageBase
    0x1000,  // SectionAlignment
    0x0200,  // FileAlignment
    4, 0,  // MajorOperatingSystemVersion, MinorOperatingSystemVersion
    0, 0,  // MajorImageVersion, MinorImageVersion
    4, 0,  // MajorSubsystemVersion, MinorSubsystemVersion
    0,  // Win32VersionValue
    0x16000,  // SizeOfImage
    0x200,  // SizeOfHeaders
    0,  // CheckSum
    3,  // Subsystem
    0,  // DllCharacteristics
    1024 * 1024,  // SizeOfStackReserve
    8 * 1024,  // SizeOfStackCommit
    1024 * 1024,  // SizeOfHeapReserve
    4 * 1024,  // SizeOfHeapCommit
    0,  // LoaderFlags
    16  // NumberOfRvaAndSizes
  };
  std::memset(opt.DataDirectory, 0, sizeof(opt.DataDirectory));
  opt.DataDirectory[1].VirtualAddress = 0x5000;  // import table
  opt.DataDirectory[1].Size = 100;
  std::memcpy(ptr, &opt, sizeof(opt));
  ptr += sizeof(opt);

  IMAGE_SECTION_HEADER sects[3];
  std::memset(sects, 0, sizeof(sects));
  std::strcpy(reinterpret_cast<char *>(sects[0].Name), ".text");
  sects[0].Misc.VirtualSize = static_cast<DWORD>(codeSize);
  sects[0].VirtualAddress = 0x1000;
  sects[0].SizeOfRawData = static_cast<DWORD>(codeSize);
  sects[0].PointerToRawData = 0x400;
  sects[0].Characteristics = 0x60500060;

  std::strcpy(reinterpret_cast<char *>(sects[1].Name), ".idata");
  sects[1].Misc.VirtualSize = 100;
  sects[1].VirtualAddress = 0x5000;
  sects[1].SizeOfRawData = 512;
  sects[1].PointerToRawData = 0x200;
  sects[1].Characteristics = 0xc0300040;

  std::strcpy(reinterpret_cast<char *>(sects[2].Name), ".bss");
  sects[2].Misc.VirtualSize = 65536;
  sects[2].VirtualAddress = 0x6000;
  sects[2].Characteristics = 0xc0400080;
  std::memcpy(ptr, sects, sizeof(sects));
}


/*!
 * @brief Write IData to array
 * @param [out] exeBin    Destination executable binary array
 */
inline static void
writeIData(unsigned char *exeBin)
{
  static const int IDT[] = {
    // IDT 1
    0x5028, 0, 0, 0x5034, 0x5044,
    // IDT (End)
    0, 0, 0, 0, 0
  };
  static const int ILT_IAT[] = {
    0x5050, 0x505a, 0
  };
  unsigned char *ptr = exeBin + 0x200;
  short s = 0x0000;

  std::memcpy(ptr, IDT, sizeof(IDT));
  ptr += sizeof(IDT);
  std::memcpy(ptr, ILT_IAT, sizeof(ILT_IAT));
  ptr += sizeof(ILT_IAT);
  std::memcpy(ptr, "msvcrt.dll\0\0\0\0\0", 16);
  ptr += 16;
  std::memcpy(ptr, ILT_IAT, sizeof(ILT_IAT));
  ptr += sizeof(ILT_IAT);
  std::memcpy(ptr, &s, sizeof(s));
  ptr += sizeof(short);
  std::memcpy(ptr, "putchar", 8);
  ptr += 8;
  std::memcpy(ptr, &s, sizeof(s));
  ptr += sizeof(short);
  std::memcpy(ptr, "getchar", 8);
}
#endif  // defined(_WIN32) || defined(_WIN64) || (__CYGWIN__)
