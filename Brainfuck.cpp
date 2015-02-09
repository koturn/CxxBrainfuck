#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#if __cplusplus >= 201103L
#  include <cstdint>
#else
#  include <stdint.h>
#endif

#ifdef USE_XBYAK
#  if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#    define  XBYAK_NO_OP_NAMES
#  endif
#  include <xbyak/xbyak.h>
#endif  // USE_XBYAK
#include "Brainfuck.h"
#include "CodeGenerator/_AllGenerator.h"
#include "winsubset.h"
#include "elfsubset.h"


static const char *
findLoopEnd(const char *srcptr);

static const char *
findLoopStart(const char *srcptr);

static unsigned int
countChar(const char *srcptr, char ch);


inline static void
writePEHeader(unsigned char *binCode, std::size_t codeSize);

inline static void
writeIData(unsigned char *binCode);

inline static void
writeElfHeader(unsigned char *binCode, std::size_t codeSize);

inline static void
writeElfFooter(unsigned char *binCode, std::size_t codeSize);


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
  delete[] binCode;
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
               "#ifndef WIN32_LEAN_AND_MEAN\n"
               "#  define WIN32_LEAN_AND_MEAN\n"
               "#  define WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
               "#endif\n"
               "#include <windows.h>\n"
               "#ifdef WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
               "#  undef WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
               "#  undef WIN32_LEAN_AND_MEAN\n"
               "#endif\n"
#elif defined(__linux__)
               "#include <unistd.h>\n"
               "#include <sys/mman.h>\n"
#endif
               "\n"
               "static int stack[128 * 1024];\n"
               "static unsigned char code[] = {\n"
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
               "  VirtualProtect((LPVOID) code, sizeof(code), PAGE_EXECUTE_READWRITE, &old_protect);\n"
#elif defined(__linux__)
               "  long page_size = sysconf(_SC_PAGESIZE) - 1;\n"
               "  mprotect((void *) code, (sizeof(code) + page_size) & ~page_size, PROT_READ | PROT_EXEC);\n"
#endif
               "  ((void (*)(int (*)(int), int (*)(), int *)) (unsigned char *) code)(putchar, getchar, stack);\n"
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


/*!
 * @brief Generate executable Windows binary
 * @param [in] wbt  Binary type
 */
void
Brainfuck::generateWinBinary(BinType wbt)
{
  switch (wbt) {
    case WIN_BIN_X86:
      generateX86WinBinary();
      break;
    case ELF_BIN_X64:
      generateX64ElfBinary();
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
  generator.getCode<void (*)(int (*)(int), int (*)(), int *)>()
    (std::putchar, std::getchar, xbyakRtStack);
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

  delete[] binCode;
  binCode = new unsigned char[EXE_SIZE];
  std::memset(binCode, 0, EXE_SIZE);

  if (commands.size() == 0 && compileType != NO_COMPILE) {
    compile();
  }
  binCode[HEADER_OFFSET] = 0x31; binCode[HEADER_OFFSET + 1] = 0xc9;  // xor ecx, ecx
  binCode[HEADER_OFFSET + 2] = 0x57;  // push edi
  binCode[HEADER_OFFSET + 3] = 0xbf; *reinterpret_cast<int *>(&binCode[HEADER_OFFSET + 4]) = ADDR_BUF;  // mov edi, addr_buf

  int idx = HEADER_OFFSET + 8;
  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          binCode[idx] = 0x66; binCode[idx + 1] = 0x41;  // inc cx
          idx += 2;
        }
        break;
      case PTR_SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          binCode[idx] = 0x66; binCode[idx + 1] = 0x49;  // dec cx
          idx += 2;
        }
        break;
      case ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          binCode[idx] = 0xfe; binCode[idx + 1] = 0x04; binCode[idx + 2] = 0x0f;  // inc byte [edi+ecx]
          idx += 3;
        }
        break;
      case SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          binCode[idx] = 0xfe; binCode[idx + 1] = 0x0c; binCode[idx + 2] = 0x0f;  // dec byte [edi+ecx]
          idx += 3;
        }
        break;
      case PUTCHAR:
        binCode[idx] = 0x51;  // push ecx
        *reinterpret_cast<int *>(&binCode[idx + 1]) = 0x0f04b60f;  // movzx eax,byte [edi+ecx]
        binCode[idx + 5] = 0x50;  // push eax
        binCode[idx + 6] = 0xa1; *reinterpret_cast<int *>(&binCode[idx + 7]) = ADDR_PUTCHAR;  // mov eax, [addr_putchar]
        binCode[idx + 11] = 0xff; binCode[idx + 12] = 0xd0;  // call eax
        binCode[idx + 13] = 0x58;  // pop eax
        binCode[idx + 14] = 0x59;  // pop ecx
        idx += 15;
        break;
      case GETCHAR:
        binCode[idx] = 0x51;  // push ecx
        binCode[idx + 1] = 0xa1; *reinterpret_cast<int *>(&binCode[idx + 2]) = ADDR_GETCHAR;  // mov eax, [addr_getchar]
        binCode[idx + 6] = 0xff; binCode[idx + 7] = 0xd0;  // call eax
        binCode[idx + 8] = 0x59;  // pop ecx
        binCode[idx + 9] = 0x88; binCode[idx + 10] = 0x04; binCode[idx + 11] = 0x0f;  // mov [edi+ecx],al
        idx += 12;
        break;
      case LOOP_START:
        loopStack.push(idx);
        *reinterpret_cast<int *>(&binCode[idx]) = 0x000f3c80;  // cmp byte [edi+ecx], 0
        // jz Jump to just behind the corresponding ] (define address later)
        binCode[idx + 4] = 0x0f; binCode[idx + 5] = 0x84;
        idx += 10;
        break;
      case LOOP_END:
        {
          int idxLoop = loopStack.top();
          loopStack.pop();
          binCode[idx] = 0xe9; *reinterpret_cast<int *>(&binCode[idx + 1]) = idxLoop - (idx + 5);  // jmp idxLoop
          idx += 5;
          *reinterpret_cast<int *>(&binCode[idxLoop + 6]) = idx - (idxLoop + 10);  // Rewrites the top of the loop
        }
        break;
      case ASSIGN_ZERO:
        // LOOP_START
        pc += 2;
        loopStack.push(idx);
        *reinterpret_cast<int *>(&binCode[idx]) = 0x000f3c80;  // cmp byte [edi+ecx], 0
        binCode[idx + 4] = 0x0f; binCode[idx + 5] = 0x84;
        idx += 10;
        // SUB
        binCode[idx] = 0xfe; binCode[idx + 1] = 0x0c; binCode[idx + 2] = 0x0f;  // dec byte [edi+ecx]
        idx += 3;
        // LOOP_END
        {
          int idxLoop = loopStack.top();
          loopStack.pop();
          binCode[idx] = 0xe9; *reinterpret_cast<int *>(&binCode[idx + 1]) = idxLoop - (idx + 5);  // jmp idxLoop
          idx += 5;
          *reinterpret_cast<int *>(&binCode[idxLoop + 6]) = idx - (idxLoop + 10);  // Rewrites the top of the loop
        }
        break;
    }
    if (static_cast<std::size_t>(idx) > sizeof(binCode) - 32) {
      throw "Output size has been exceeded\n";
    }
  }
  binCode[idx] = 0x5f;  // pop edi
  binCode[idx + 1] = 0x31; binCode[idx + 2] = 0xc0;  // xor eax, eax
  binCode[idx + 3] = 0xc3;  // ret

  writePEHeader(binCode, EXE_SIZE - HEADER_OFFSET);
  writeIData(binCode);
  binCodeSize = EXE_SIZE;
}


/*!
 * @brief Generate x64 Executable Elf binary
 */
void
Brainfuck::generateX64ElfBinary(void)
{
  static const unsigned int ELF_SIZE =
    64 * 1024 + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + 22 + sizeof(Elf64_Shdr) * 4;
  std::stack<unsigned char *> loopStack;

  delete[] binCode;
  binCode = new unsigned char[ELF_SIZE];
  std::memset(binCode, 0, ELF_SIZE);

  if (commands.size() == 0 && compileType != NO_COMPILE) {
    compile();
  }

  unsigned char *const base = binCode;
  unsigned char *b = base + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;

  // mov rbx, 0x0000000000000000  (set .bss address)
  *b++ = 0x48; *b++ = 0xbb;
  *reinterpret_cast<uint64_t *>(b) = 0x08248000; b += sizeof(uint64_t);

  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          *b++ = 0x48; *b++ = 0xff; *b++ = 0xc3;  // inc rbx
        }
        break;
      case PTR_SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          *b++ = 0x48; *b++ = 0xff; *b++ = 0xcb;  // dec rbx
        }
        break;
      case ADD:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          *b++ = 0xfe; *b++ = 0x03;  // inc byte ptr [rbx]
        }
        break;
      case SUB:
        for (unsigned int i = 0; i < commands[pc].value; i++) {
          *b++ = 0xfe; *b++ = 0x0b;  // dec byte ptr [rbx]
        }
        break;
      case PUTCHAR:
        *b++ = 0x48; *b++ = 0xc7; *b++ = 0xc0; *b++ = 0x01; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov rax, 1 (set system call to write)
        *b++ = 0xba; *b++ = 0x01; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov edx, 0x01 (3rd argument)
        *b++ = 0x48; *b++ = 0x89; *b++ = 0xde;  // mov rsi, rbx (2nd argument)
        *b++ = 0xbf; *b++ = 0x01; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov edi, 0x1 (1st argument)
        *b++ = 0x0f; *b++ = 0x05;  // syscall
        break;
      case GETCHAR:
        *b++ = 0x48; *b++ = 0xc7; *b++ = 0xc0; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov rax, 0 (set system call to write)
        *b++ = 0xba; *b++ = 0x01; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov edx, 0x01 (3rd argument)
        *b++ = 0x48; *b++ = 0x89; *b++ = 0xde;  // mov rsi, rbx (2nd argument)
        *b++ = 0xbf; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;  // mov edi, 0x0 (1st argument)
        *b++ = 0x0f; *b++ = 0x05;  // syscall
        break;
      case LOOP_START:
        loopStack.push(b);
        *b++ = 0x80; *b++ = 0x3b; *b++ = 0x00;  // cmp byte ptr [rbx], 0
        *b++ = 0x0f; *b++ = 0x84;
        *reinterpret_cast<uint32_t *>(b) = 0x00000000; b += sizeof(uint32_t);
        break;
      case LOOP_END:
        {
          unsigned char *_b = loopStack.top();
          loopStack.pop();
          *b++ = 0xe9;
          *reinterpret_cast<int32_t *>(b) = -static_cast<int32_t>(b + sizeof(int32_t) - _b); b += sizeof(int32_t);
          *reinterpret_cast<int32_t *>(_b + 5) = static_cast<int32_t>(b - (_b + 9));
        }
        break;
      case ASSIGN_ZERO:
        loopStack.push(b);
        *b++ = 0x80; *b++ = 0x3b; *b++ = 0x00;  // cmp byte ptr [rbx], 0
        *b++ = 0x0f; *b++ = 0x84;
        *reinterpret_cast<uint32_t *>(b) = 0x00000000; b += sizeof(uint32_t);

        *b++ = 0xfe; *b++ = 0x0b;  // dec byte ptr [rbx]

        {
          unsigned char *_b = loopStack.top();
          loopStack.pop();
          *b++ = 0xe9;
          *reinterpret_cast<int32_t *>(b) = -static_cast<int32_t>(b + sizeof(int32_t) - _b); b += sizeof(int32_t);
          *reinterpret_cast<int32_t *>(_b + 5) = static_cast<int32_t>(b - (_b + 9));
        }
        break;
    }
  }

  *b++ = 0xb8; *b++ = 0x3c; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;
  *b++ = 0xbf; *b++ = 0x2a; *b++ = 0x00; *b++ = 0x00; *b++ = 0x00;
  *b++ = 0x0f; *b++ = 0x05;

  std::size_t codeSize = b - (base + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2);

  writeElfHeader(base, codeSize);
  writeElfFooter(base, codeSize);
  binCodeSize = codeSize + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + 22 + sizeof(Elf64_Shdr) * 4;
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


/*!
 * @brief Write PE Header to array
 * @param [out] binCode   Destination executable binary array
 * @param [in]  codeSize  Size of code
 */
inline static void
writePEHeader(unsigned char *binCode, std::size_t codeSize)
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
    'T',  'h',  'i',  's',  ' ',  'p',  'r',  'o',  'g',  'r',  'a',  'm',  ' ',  'c',  'a',  'n',
    'n',  'o',  't',  ' ',  'b',  'e',  ' ',  'r',  'u',  'n',  ' ',  'i',  'n',  ' ',  'D',  'O',
    'S',  ' ',  'm',  'o',  'd',  'e',  '.',  '\r', '\n', '$',  0,    0,    0,    0,    0,    0,
    // 80-83: PE Signature
    'P',  'E',  0,    0
  };

  unsigned char *ptr = binCode;
  std::memcpy(ptr, STUB, sizeof(STUB));
  ptr += sizeof(STUB);

  IMAGE_FILE_HEADER *ifh = reinterpret_cast<IMAGE_FILE_HEADER *>(ptr);
  ifh->Machine = 0x014c;
  ifh->NumberOfSections = 3;
  ifh->TimeDateStamp = 0;
  ifh->PointerToSymbolTable = 0;
  ifh->NumberOfSymbols = 0;
  ifh->SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
  ifh->Characteristics = 0x030f;
  ptr += sizeof(IMAGE_FILE_HEADER);

  IMAGE_OPTIONAL_HEADER32 *ioh = reinterpret_cast<IMAGE_OPTIONAL_HEADER32 *>(ptr);
  ioh->Magic = 0x010b;
  ioh->MajorLinkerVersion = 6;
  ioh->MinorLinkerVersion = 0;
  ioh->SizeOfCode = static_cast<DWORD>(codeSize);
  ioh->SizeOfInitializedData = 0;
  ioh->SizeOfUninitializedData = 65536;
  ioh->AddressOfEntryPoint = 0x1000;
  ioh->BaseOfCode = 0x1000;
  ioh->BaseOfData = 0x6000;
  ioh->ImageBase = 0x00400000;
  ioh->SectionAlignment = 0x1000;
  ioh->FileAlignment = 0x0200;
  ioh->MajorOperatingSystemVersion = 4;
  ioh->MinorOperatingSystemVersion = 0;
  ioh->MajorImageVersion = 0;
  ioh->MinorImageVersion = 0;
  ioh->MajorSubsystemVersion = 4;
  ioh->MinorSubsystemVersion = 0;
  ioh->Win32VersionValue = 0;
  ioh->SizeOfImage = 0x16000;
  ioh->SizeOfHeaders = 0x200;
  ioh->CheckSum = 0;
  ioh->Subsystem = 3;
  ioh->DllCharacteristics = 0;
  ioh->SizeOfStackReserve = 1024 * 1024;
  ioh->SizeOfStackCommit = 8 * 1024;
  ioh->SizeOfHeapReserve = 1024 * 1024;
  ioh->SizeOfHeapCommit = 4 * 1024;
  ioh->LoaderFlags = 0;
  ioh->NumberOfRvaAndSizes = 16;
  ioh->DataDirectory[1].VirtualAddress = 0x5000;  // import table
  ioh->DataDirectory[1].Size = 100;
  ptr += sizeof(IMAGE_OPTIONAL_HEADER32);

  // .text section
  IMAGE_SECTION_HEADER *ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".text");
  ish->Misc.VirtualSize = static_cast<DWORD>(codeSize);
  ish->VirtualAddress = 0x1000;
  ish->SizeOfRawData = static_cast<DWORD>(codeSize);
  ish->PointerToRawData = 0x400;
  ish->Characteristics = 0x60500060;
  ptr += sizeof(IMAGE_SECTION_HEADER);

  // .idata section
  ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".idata");
  ish->Misc.VirtualSize = 100;
  ish->VirtualAddress = 0x5000;
  ish->SizeOfRawData = 512;
  ish->PointerToRawData = 0x200;
  ish->Characteristics = 0xc0300040;
  ptr += sizeof(IMAGE_SECTION_HEADER);

  // .bss section
  ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".bss");
  ish->Misc.VirtualSize = 65536;
  ish->VirtualAddress = 0x6000;
  ish->Characteristics = 0xc0400080;
}


/*!
 * @brief Write IData to array
 * @param [out] binCode  Destination executable binary array
 */
inline static void
writeIData(unsigned char *binCode)
{
  static const char CRT_DLL[] = "msvcrt.dll\0\0\0\0\0";
  static const char STR_PUTCHAR[] = "putchar";
  static const char STR_GETCHAR[] = "getchar";
  static const unsigned int PE_OFFSET = 0x200;
  unsigned char *ptr = binCode + PE_OFFSET;

  int *idt = reinterpret_cast<int *>(ptr);
  // IDT 1
  idt[0] = 0x5028;
  idt[1] = 0;
  idt[2] = 0;
  idt[3] = 0x5034;
  idt[4] = 0x5044;
  // IDT (End)
  idt[5] = 0;
  idt[6] = 0;
  idt[7] = 0;
  idt[8] = 0;
  idt[9] = 0;
  ptr += sizeof(int) * 10;

  int *ilt = reinterpret_cast<int *>(ptr);
  ilt[0] = 0x5050;
  ilt[1] = 0x505a;
  ilt[2] = 0;
  ptr += sizeof(int) * 3;

  std::memcpy(ptr, CRT_DLL, sizeof(CRT_DLL));
  ptr += sizeof(CRT_DLL);

  int *iat = reinterpret_cast<int *>(ptr);
  iat[0] = 0x5050;
  iat[1] = 0x505a;
  iat[2] = 0;
  ptr += sizeof(int) * 3;

  *reinterpret_cast<short *>(ptr) = 0x0000;
  ptr += sizeof(short);
  std::memcpy(ptr, STR_PUTCHAR, sizeof(STR_PUTCHAR));
  ptr += sizeof(STR_PUTCHAR);

  *reinterpret_cast<short *>(ptr) = 0x0000;
  ptr += sizeof(short);
  std::memcpy(ptr, STR_GETCHAR, sizeof(STR_GETCHAR));
}


/*!
 * @brief Write header of ELF binary
 * @param [out] binCode   Pointer to ELF binary
 * @param [in]  codeSize  Size of code
 */
inline static void
writeElfHeader(unsigned char *binCode, std::size_t codeSize)
{
  static const uint64_t ADDR = 0x08048000;
  unsigned char *ptr = binCode;

  // ELF header
  Elf64_Ehdr *ehdr = reinterpret_cast<Elf64_Ehdr *>(ptr);
  ehdr->e_ident[EI_MAG0] = ELFMAG0;
  ehdr->e_ident[EI_MAG1] = ELFMAG1;
  ehdr->e_ident[EI_MAG2] = ELFMAG2;
  ehdr->e_ident[EI_MAG3] = ELFMAG3;
  ehdr->e_ident[EI_CLASS] = ELFCLASS64;
  ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr->e_ident[EI_VERSION] = EV_CURRENT;
  ehdr->e_ident[EI_OSABI] = 0x00;
  ehdr->e_ident[EI_ABIVERSION] = 0x00;
  ehdr->e_ident[EI_PAD] = 0x00;
  ehdr->e_type = ET_EXEC;
  ehdr->e_machine = EM_X86_64;
  ehdr->e_version = EV_CURRENT;
  ehdr->e_entry = ADDR + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  ehdr->e_phoff = sizeof(Elf64_Ehdr);
  ehdr->e_shoff = sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * 2) + codeSize + 22;
  ehdr->e_flags = 0x00000000;
  ehdr->e_ehsize = sizeof(Elf64_Ehdr);
  ehdr->e_phentsize = sizeof(Elf64_Phdr);
  ehdr->e_phnum = 2;
  ehdr->e_shentsize = sizeof(Elf64_Shdr);
  ehdr->e_shnum = 4;
  ehdr->e_shstrndx = 1;
  ptr += sizeof(Elf64_Ehdr);

  // Program header
  Elf64_Phdr *phdr = reinterpret_cast<Elf64_Phdr *>(ptr);
  phdr->p_type = PT_LOAD;
  phdr->p_flags = PF_R | PF_X;
  phdr->p_offset = 0x0000000000000000;
  phdr->p_vaddr = ADDR;
  phdr->p_paddr = ADDR;
  phdr->p_filesz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize + 22 + sizeof(Elf64_Shdr) * 4;
  phdr->p_memsz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize + 22 + sizeof(Elf64_Shdr) * 4;
  phdr->p_align = 0x0000000000000100;
  ptr += sizeof(Elf64_Phdr);

  // program header for .bss (56 bytes)
  phdr = reinterpret_cast<Elf64_Phdr *>(ptr);
  phdr->p_type = PT_LOAD;
  phdr->p_flags = PF_R | PF_W;
  phdr->p_offset = 0x0000000000001000;
  phdr->p_vaddr = ADDR + 0x200000;
  phdr->p_paddr = ADDR + 0x200000;
  phdr->p_filesz = 0x0000000000000000;
  phdr->p_memsz = 0x0000000000007530;
  phdr->p_align = 0x0000000000200000;
}


/*!
 * @brief Write footer of ELF binary
 * @param [out] binCode   Pointer to ELF binary
 * @param [in]  codeSize  Size of code
 */
inline static void
writeElfFooter(unsigned char *binCode, std::size_t codeSize)
{
  static const uint64_t ADDR = 0x08048000;
  static const char SHSTRTBL[] = "\0.text\0.shstrtbl\0.bss";
  unsigned char *ptr = binCode + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize;

  // section string table (22bytes)
  std::memcpy(ptr, SHSTRTBL, sizeof(SHSTRTBL));
  ptr += sizeof(SHSTRTBL);

  // section header
  // first section header
  Elf64_Shdr *shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 0;
  shdr->sh_type = SHT_NULL;
  shdr->sh_flags = 0x0000000000000000;
  shdr->sh_addr = 0x0000000000000000;
  shdr->sh_offset = 0x0000000000000000;
  shdr->sh_size = 0x0000000000000000;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000000;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // second section header (.shstrtbl)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 7;
  shdr->sh_type = SHT_STRTAB;
  shdr->sh_flags = 0x0000000000000000;
  shdr->sh_addr = 0x0000000000000000;
  shdr->sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize;
  shdr->sh_size = sizeof(SHSTRTBL);
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000001;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // third section header (.text)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 1;
  shdr->sh_type = SHT_PROGBITS;
  shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
  shdr->sh_addr = ADDR + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  shdr->sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  shdr->sh_size = codeSize;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000004;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // fourth section header (.bss)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 17;
  shdr->sh_type = SHT_NOBITS;
  shdr->sh_flags = SHF_ALLOC | SHF_WRITE;
  shdr->sh_addr = ADDR + 0x200000;
  shdr->sh_offset = 0x0000000000001000;
  shdr->sh_size = 0x0000000000007530;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000010;
  shdr->sh_entsize = 0x0000000000000000;
}
