#ifndef GENERATOR_JAVA
#define GENERATOR_JAVA


#include "../CodeGenerator.h"


class GeneratorJava : public CodeGenerator {
public:
  GeneratorJava(const char *indent="    ") :
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
GeneratorJava::printHeader(void)
{
  std::cout << "public class TranslatedBrainfuck {\n"
            << indent << "public static void main(String[] args) {\n"
            << indent << indent << "final int MEMORY_SIZE = 65536;\n"
            << indent << indent << "char[] memory = new char[MEMORY_SIZE];\n"
            << indent << indent << "int idx = 0;\n\n";
  indentLevel = 2;
}


inline void
GeneratorJava::printFooter(void)
{
  std::cout << indent << "}\n"
            << "}"
            << std::endl;
}


inline void
GeneratorJava::printPtrAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "idx++;\n";
  } else {
    printIndent();
    std::cout << "idx += " << value << ";\n";
  }
}


inline void
GeneratorJava::printPtrSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorJava::printAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorJava::printSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
}


inline void
GeneratorJava::printPutchar(void)
{
  printIndent();
  std::cout << "System.out.print(memory[idx]);\n";
}


inline void
GeneratorJava::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = System.in.read();\n";
}


inline void
GeneratorJava::printLoopStart(void)
{
  printIndent();
  std::cout << "while (memory[idx] != 0) {\n";
  indentLevel++;
}


inline void
GeneratorJava::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "}\n";
}


inline void
GeneratorJava::printAssignZero(void)
{
  printIndent();
  std::cout << "memory[idx] = 0;\n";
}


#endif  // GENERATOR_JAVA
