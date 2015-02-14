#ifndef GENERATOR_RUBY_H
#define GENERATOR_RUBY_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorRuby : public SourceGenerator {
protected:
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genNextN(unsigned int value);
  inline void genPrevN(unsigned int value);
  inline void genAdd(unsigned int value);
  inline void genSub(unsigned int value);
  inline void genPutchar(void);
  inline void genGetchar(void);
  inline void genLoopStart(void);
  inline void genLoopEnd(void);
  inline void genAssign(unsigned int value);
  inline void genAddVar(int value);
  inline void genSubVar(int value);
public:
  GeneratorRuby(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorRuby::genHeader(void)
{
  std::cout << "#!/usr/bin/env ruby\n\n"
               "if __FILE__ == $0\n"
            << indent << "memory = [0]\n"
            << indent << "idx = 0\n\n";
}


inline void
GeneratorRuby::genFooter(void)
{
  std::cout << "end" << std::endl;
}


inline void
GeneratorRuby::genNextN(unsigned int value)
{
  genIndent();
  std::cout << "idx += " << value << "\n";
  genIndent();
  std::cout << "if idx >= memory.size\n";
  genIndent();
  std::cout << indent << "memory += [0] * (idx - memory.size + 1)\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorRuby::genPrevN(unsigned int value)
{
  genIndent();
  std::cout << "idx -= " << value << "\n";
}


inline void
GeneratorRuby::genAdd(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << "\n";
}


inline void
GeneratorRuby::genSub(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << "\n";
}


inline void
GeneratorRuby::genPutchar(void)
{
  genIndent();
  std::cout << "print memory[idx].chr\n";
}


inline void
GeneratorRuby::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = gets.chomp[0].ord\n";
}


inline void
GeneratorRuby::genLoopStart(void)
{
  genIndent();
  std::cout << "until memory[idx] == 0\n";
  indentLevel++;
}


inline void
GeneratorRuby::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorRuby::genAssign(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << "\n";
}


inline void
GeneratorRuby::genAddVar(int value)
{
  genIndent();
  std::cout << "unless memory[idx] == 0\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "if idx + " << value << " >= memory.size\n";
    genIndent();
    std::cout << indent << indent
              << "memory += [0] * (idx + " << value << " - memory.size + 1)\n";
    genIndent();
    std::cout << indent << "end\n";
    genIndent();
    std::cout << indent << "memory[idx + " <<  value << "] += memory[idx]\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] += memory[idx]\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorRuby::genSubVar(int value)
{
  genIndent();
  std::cout << "unless memory[idx] == 0\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " <<  value << "] -= memory[idx]\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] -= memory[idx]\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


}  // namespace bf
#endif  // GENERATOR_RUBY_H
