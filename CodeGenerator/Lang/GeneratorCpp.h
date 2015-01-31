#ifndef GENERATOR_CPP
#define GENERATOR_CPP


#include "../CodeGenerator.h"


class GeneratorCpp : public CodeGenerator {
public:
  GeneratorCpp(const char *indent="  ") :
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
GeneratorCpp::printHeader(void)
{
  std::cout << "#include <cstdlib>\n"
               "#include <iostream>\n"
               "#include <vector>\n\n"
               "static const std::size_t MEMORY_SIZE = 256;\n\n"
               "int\n"
               "main(void)\n"
               "{\n"
            << indent << "std::vector<unsigned char> memory(MEMORY_SIZE, 0);\n"
            << indent << "std::vector<unsigned char>::size_type idx = 0;\n\n";
}


inline void
GeneratorCpp::printFooter(void)
{
  std::cout << "\n";
  printIndent();
  std::cout << "return EXIT_SUCCESS;\n"
               "}"
            << std::endl;
}


inline void
GeneratorCpp::printPtrAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "idx++;\n";
  } else {
    printIndent();
    std::cout << "idx += " << value << ";\n";
  }
  printIndent();
  std::cout << "while (idx >= memory.size()) {\n";
  printIndent();
  std::cout << indent << "memory.resize(memory.size() * 2);\n";
  printIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::printPtrSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorCpp::printAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorCpp::printSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
}


inline void
GeneratorCpp::printPutchar(void)
{
  printIndent();
  std::cout << "std::cout.put(memory[idx]);\n";
}


inline void
GeneratorCpp::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = static_cast<unsigned char>(std::cin.get());\n";
}


inline void
GeneratorCpp::printLoopStart(void)
{
  printIndent();
  std::cout << "while (memory[idx]) {\n";
  indentLevel++;
}


inline void
GeneratorCpp::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::printAssignZero(void)
{
  printIndent();
  std::cout << "*ptr = 0;\n";
}


#endif  // GENERATOR_CPP
