#ifndef GENERATOR_RUBY
#define GENERATOR_RUBY


#include "../CodeGenerator.h"


class GeneratorRuby : public CodeGenerator {
public:
  GeneratorRuby(const char *indent="  ") :
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
GeneratorRuby::printHeader(void)
{
  std::cout << "#!/usr/bin/env ruby\n\n"
            << "if __FILE__ == $0\n"
            << indent << "memory = [0]\n"
            << indent << "idx = 0\n\n";
}


inline void
GeneratorRuby::printFooter(void)
{
  std::cout << "end" << std::endl;
}


inline void
GeneratorRuby::printPtrAdd(unsigned int value)
{
  printIndent();
  std::cout << "idx += " << value << "\n";
  printIndent();
  std::cout << "if idx >= memory.size\n";
  indentLevel++;
  printIndent();
  std::cout << "memory += [0] * (idx - memory.size + 1)\n";
  indentLevel--;
  printIndent();
  std::cout << "end\n";
}


inline void
GeneratorRuby::printPtrSub(unsigned int value)
{
  printIndent();
  std::cout << "idx -= " << value << "\n";
}


inline void
GeneratorRuby::printAdd(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] += " << value << "\n";
}


inline void
GeneratorRuby::printSub(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] -= " << value << "\n";
}


inline void
GeneratorRuby::printPutchar(void)
{
  printIndent();
  std::cout << "print memory[idx].chr\n";
}


inline void
GeneratorRuby::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = gets.chomp[0].ord\n";
}


inline void
GeneratorRuby::printLoopStart(void)
{
  printIndent();
  std::cout << "until memory[idx] == 0\n";
  indentLevel++;
}


inline void
GeneratorRuby::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "end\n";
}


#endif  // GENERATOR_RUBY
