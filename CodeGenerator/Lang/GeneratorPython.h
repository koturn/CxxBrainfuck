#ifndef GENERATOR_PYTHON
#define GENERATOR_PYTHON


#include "../CodeGenerator.h"


class GeneratorPython : public CodeGenerator {
public:
  GeneratorPython(const char *indent="    ") :
    CodeGenerator(indent) {};
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
};




inline void
GeneratorPython::printHeader(void)
{
  std::cout << "#!/usr/bin/env python\n\n"
            << "import sys\n\n\n"
            << "if __name__ == '__main__':\n"
            << indent << "memory = [0]\n"
            << indent << "idx = 0\n\n";
}


inline void
GeneratorPython::printFooter(void)
{
  // Nothing to do
}


inline void
GeneratorPython::printPtrAdd(unsigned int value)
{
  printIndent();
  std::cout << "idx += " << value << "\n";
  printIndent();
  std::cout << "if idx >= len(memory):\n";
  indentLevel++;
  printIndent();
  std::cout << "memory += [0] * (idx - len(memory) + 1)\n";
  indentLevel--;
}


inline void
GeneratorPython::printPtrSub(unsigned int value)
{
  printIndent();
  std::cout << "idx -= " << value << "\n";
}


inline void
GeneratorPython::printAdd(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] += " << value << "\n";
}


inline void
GeneratorPython::printSub(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] -= " << value << "\n";
}


inline void
GeneratorPython::printPutchar(void)
{
  printIndent();
  std::cout << "sys.stdout.write(chr(memory[idx]))\n";
}


inline void
GeneratorPython::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = ord(sys.stdin.read(1))\n";
}


inline void
GeneratorPython::printLoopStart(void)
{
  printIndent();
  std::cout << "while memory[idx] != 0:\n";
  indentLevel++;
}


inline void
GeneratorPython::printLoopEnd(void)
{
  indentLevel--;
}


#endif  // GENERATOR_PYTHON
