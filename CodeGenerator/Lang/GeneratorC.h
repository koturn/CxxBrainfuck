#ifndef GENERATOR_C
#define GENERATOR_C


#include "../CodeGenerator.h"


class GeneratorC : public CodeGenerator {
public:
  GeneratorC(const char *indent="  ") :
    CodeGenerator(indent) {}
  inline void printHeader(void);
  inline void printFooter(void);
  inline void printPtrAdd(unsigned int value);
  inline void printPtrSub(unsigned int value);
  inline void printAdd(unsigned int value);
  inline void printSub(unsigned int value);
  inline void printPutchar(void);
  inline void printGetchar(void);
  inline void printLoopStart(void);
  inline void printLoopEnd(void);
  inline void printAssignZero(void);
};




inline void
GeneratorC::printHeader(void)
{
  std::cout << "#include <stdio.h>\n"
            << "#include <stdlib.h>\n\n"
            << "#define MEMORY_SIZE 65536\n\n"
            << "int\n"
            << "main(void)\n"
            << "{\n";
  printIndent();
  std::cout << "static unsigned char memory[MEMORY_SIZE] = {0};\n";
  printIndent();
  std::cout << "unsigned char *ptr = memory;\n\n";
}


inline void
GeneratorC::printFooter(void)
{
  std::cout << "\n";
  printIndent();
  std::cout << "return EXIT_SUCCESS;\n"
    << "}"
    << std::endl;
}


inline void
GeneratorC::printPtrAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "ptr++;\n";
  } else {
    std::cout << "ptr += " << value << ";\n";
  }
}


inline void
GeneratorC::printPtrSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "ptr--;\n";
  } else {
    std::cout << "ptr -= " << value << ";\n";
  }
}


inline void
GeneratorC::printAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "(*ptr)++;\n";
  } else {
    std::cout << "*ptr += " << value << ";\n";
  }
}


inline void
GeneratorC::printSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "(*ptr)--;\n";
  } else {
    std::cout << "*ptr -= " << value << ";\n";
  }
}


inline void
GeneratorC::printPutchar(void)
{
  printIndent();
  std::cout << "putchar(*ptr);\n";
}


inline void
GeneratorC::printGetchar(void)
{
  printIndent();
  std::cout << "*ptr = (unsigned char) getchar();";
}


inline void
GeneratorC::printLoopStart(void)
{
  printIndent();
  std::cout << "while (*ptr) {\n";
  indentLevel++;
}


inline void
GeneratorC::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::printAssignZero(void)
{
  printIndent();
  std::cout << "*ptr = 0;\n";
}


#endif  // GENERATOR_C
