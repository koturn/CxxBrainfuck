#ifndef GENERATOR_CSHARP
#define GENERATOR_CSHARP


#include "../CodeGenerator.h"


class GeneratorCSharp : public CodeGenerator {
public:
  GeneratorCSharp(const char *indent="    ") :
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
GeneratorCSharp::printHeader(void)
{
  std::cout << "using System;\n\n"
            << "public class TranslatedBrainfuck\n"
            << "{\n"
            << indent << "public static void Main()\n"
            << indent << "{\n"
            << indent << indent << "const int MEMORY_SIZE = 65536;\n"
            << indent << indent << "byte[] memory = new byte[MEMORY_SIZE];\n"
            << indent << indent << "int idx = 0;\n\n";
  indentLevel = 2;
}


inline void
GeneratorCSharp::printFooter(void)
{
  std::cout << indent << "}\n"
            << "}"
            << std::endl;
}


inline void
GeneratorCSharp::printPtrAdd(unsigned int value)
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
GeneratorCSharp::printPtrSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorCSharp::printAdd(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorCSharp::printSub(unsigned int value)
{
  printIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
}


inline void
GeneratorCSharp::printPutchar(void)
{
  printIndent();
  std::cout << "Console.Write((char) memory[idx]);\n";
}


inline void
GeneratorCSharp::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = Console.Read();\n";
}


inline void
GeneratorCSharp::printLoopStart(void)
{
  printIndent();
  std::cout << "while (memory[idx] != 0)\n";
  printIndent();
  std::cout << "{\n";
  indentLevel++;
}


inline void
GeneratorCSharp::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "}\n";
}


inline void
GeneratorCSharp::printAssignZero(void)
{
  printIndent();
  std::cout << "memory[idx] = 0;\n";
}


#endif  // GENERATOR_CSHARP
