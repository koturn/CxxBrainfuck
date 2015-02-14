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




static const char *
findLoopEnd(const char *srcptr);

static const char *
findLoopStart(const char *srcptr);




/* ========================================================================= *
 * Public members                                                            *
 * ========================================================================= */
namespace bf {


/*!
 * @brief Destructor: Delete alocated memory
 */
Brainfuck::~Brainfuck(void)
{
  delete[] sourceBuffer;
  delete[] binCode;
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
  std::size_t size = jitCompiler.getSize();
  if (size == 0) {
    xbyakJitCompile();
    size = jitCompiler.getSize();
  }
  const Xbyak::uint8 *code = jitCompiler.getCode();

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
  irCompiler.compile();
  BfIR irCode = irCompiler.getCode();
  switch (lang) {
    case LANG_C:
      {
        GeneratorC cGenerator(irCode);
        cGenerator.genCode();
      }
      break;
    case LANG_CPP:
      {
        GeneratorCpp cppGenerator(irCode);
        cppGenerator.genCode();
      }
      break;
    case LANG_CSHARP:
      {
        GeneratorCSharp csharpGenerator(irCode);
        csharpGenerator.genCode();
      }
      break;
    case LANG_JAVA:
      {
        GeneratorJava javaGenerator(irCode);
        javaGenerator.genCode();
      }
      break;
    case LANG_LUA:
      {
        GeneratorLua luaGenerator(irCode);
        luaGenerator.genCode();
      }
      break;
    case LANG_PYTHON:
      {
        GeneratorPython pythonGenerator(irCode);
        pythonGenerator.genCode();
      }
      break;
    case LANG_RUBY:
      {
        GeneratorRuby rubyGenerator(irCode);
        rubyGenerator.genCode();
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
  irCompiler.compile();
  BfIR irCode = irCompiler.getCode();
  switch (wbt) {
    case WIN_BIN_X86:
      {
        GeneratorWinX86 g = GeneratorWinX86(irCode);
        g.genCode();
        binCodeSize = g.getSize();
        binCode = new unsigned char[binCodeSize];
        std::memcpy(binCode, g.getCode(), binCodeSize);
      }
      break;
    case ELF_BIN_X64:
      {
        GeneratorElfX64 g = GeneratorElfX64(irCode);
        g.genCode();
        binCodeSize = g.getSize();
        binCode = new unsigned char[binCodeSize];
        std::memcpy(binCode, g.getCode(), binCodeSize);
      }
      break;
  }
}




/* ========================================================================= *
 * Private members                                                           *
 * ========================================================================= */
void
Brainfuck::normalCompile(void)
{
  irCompiler.setSource(sourceBuffer);
  irCompiler.compile();
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

  BfIR irCode = irCompiler.getCode();
  BfIR::size_type size = irCompiler.getSize();
  for (unsigned int pc = 0; pc < size; pc++) {
    switch (irCode[pc].type) {
      case BfInstruction::NEXT:
        ptr++;
        break;
      case BfInstruction::PREV:
        ptr--;
        break;
      case BfInstruction::NEXT_N:
        ptr += irCode[pc].value1;
        break;
      case BfInstruction::PREV_N:
        ptr -= irCode[pc].value1;
        break;
      case BfInstruction::INC:
        (*ptr)++;
        break;
      case BfInstruction::DEC:
        (*ptr)--;
        break;
      case BfInstruction::ADD:
        *ptr += irCode[pc].value1;
        break;
      case BfInstruction::SUB:
        *ptr -= irCode[pc].value1;
        break;
      case BfInstruction::PUTCHAR:
        std::cout.put(static_cast<char>(*ptr));
        break;
      case BfInstruction::GETCHAR:
        *ptr = static_cast<unsigned char>(std::cin.get());
        break;
      case BfInstruction::LOOP_START:
        if (*ptr == 0) {
          pc = irCode[pc].value1;
        }
        break;
      case BfInstruction::LOOP_END:
        if (*ptr != 0) {
          pc = irCode[pc].value1;
        }
        break;
      case BfInstruction::ASSIGN:
        *ptr = irCode[pc].value1;
        break;
      case BfInstruction::ADD_VAR:
        if (*ptr) {
          *(ptr + irCode[pc].value1) += *ptr;
          *ptr = 0;
        }
        break;
      case BfInstruction::SUB_VAR:
        /*
         * Following code is slower. But I Don't know why.
         *   if (*ptr) {
         *     *(ptr + static_cast<int>(irCode[pc].value1)) -= *ptr;
         *     *ptr = 0;
         *   }
         */
        while (*ptr) {
          (*ptr)--;
          ptr += irCode[pc].value1;
          (*ptr)--;
          ptr -= irCode[pc].value1;
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
  normalCompile();
  BfIR irCode = irCompiler.getCode();
  jitCompiler.setIRCode(irCode);
  jitCompiler.compile();
  compileType = XBYAK_JIT_COMPILE;
}


/*!
 * @brief Execute jit compiled binary
 */
void
Brainfuck::xbyakJitExecute(void)
{
  int *xbyakRtStack = new int[xbyakRtStackSize];
  std::memset(xbyakRtStack, 0, xbyakRtStackSize);
  jitCompiler.getCode<void (*)(int (*)(int), int (*)(), int *)>()
    (std::putchar, std::getchar, xbyakRtStack);
  delete[] xbyakRtStack;
}
#endif  // USE_XBYAK


}  // namespace bf




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
