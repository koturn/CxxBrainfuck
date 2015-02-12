#ifndef GENERATOR_PYTHON_H
#define GENERATOR_PYTHON_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorPython : public SourceGenerator {
protected:
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genPtrAdd(unsigned int value);
  inline void genPtrSub(unsigned int value);
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
  GeneratorPython(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="    ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorPython::genHeader(void)
{
  std::cout << "#!/usr/bin/env python\n\n"
               "import sys\n\n\n"
               "if __name__ == '__main__':\n"
            << indent << "memory = [0]\n"
            << indent << "idx = 0\n\n";
}


inline void
GeneratorPython::genFooter(void)
{
  // Nothing to do
}


inline void
GeneratorPython::genPtrAdd(unsigned int value)
{
  genIndent();
  std::cout << "idx += " << value << "\n";
  genIndent();
  std::cout << "if idx >= len(memory):\n";
  genIndent();
  std::cout << indent << "memory += [0] * (idx - len(memory) + 1)\n";
}


inline void
GeneratorPython::genPtrSub(unsigned int value)
{
  genIndent();
  std::cout << "idx -= " << value << "\n";
}


inline void
GeneratorPython::genAdd(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << "\n";
}


inline void
GeneratorPython::genSub(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << "\n";
}


inline void
GeneratorPython::genPutchar(void)
{
  genIndent();
  std::cout << "sys.stdout.write(chr(memory[idx]))\n";
}


inline void
GeneratorPython::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = ord(sys.stdin.read(1))\n";
}


inline void
GeneratorPython::genLoopStart(void)
{
  genIndent();
  std::cout << "while memory[idx] != 0:\n";
  indentLevel++;
}


inline void
GeneratorPython::genLoopEnd(void)
{
  indentLevel--;
}


inline void
GeneratorPython::genAssign(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << "\n";
}


inline void
GeneratorPython::genAddVar(int value)
{
  genIndent();
  std::cout << "if memory[idx] != 0:\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "if idx + " << value << " >= len(memory):\n";
    genIndent();
    std::cout << indent << indent
              << "memory += [0] * (idx + " << value << " - len(memory) + 1)\n";
    genIndent();
    std::cout << indent << "memory[idx + " <<  value << "] += memory[idx];\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] += memory[idx];\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
}


inline void
GeneratorPython::genSubVar(int value)
{
  genIndent();
  std::cout << "if memory[idx] != 0:\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " <<  value << "] -= memory[idx];\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] -= memory[idx];\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
}


}  // namespace bf
#endif  // GENERATOR_PYTHON_H
