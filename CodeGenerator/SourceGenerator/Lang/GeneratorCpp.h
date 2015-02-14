#ifndef GENERATOR_CPP_H
#define GENERATOR_CPP_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorCpp : public SourceGenerator {
private:
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genNext(void);
  inline void genPrev(void);
  inline void genNextN(int value);
  inline void genPrevN(int value);
  inline void genInc(void);
  inline void genDec(void);
  inline void genAdd(int value);
  inline void genSub(int value);
  inline void genPutchar(void);
  inline void genGetchar(void);
  inline void genLoopStart(void);
  inline void genLoopEnd(void);
  inline void genAssign(int value);
  inline void genAddVar(int value);
  inline void genSubVar(int value);
public:
  GeneratorCpp(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorCpp::genHeader(void)
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
GeneratorCpp::genFooter(void)
{
  std::cout << "\n";
  genIndent();
  std::cout << "return EXIT_SUCCESS;\n"
               "}"
            << std::endl;
}


inline void
GeneratorCpp::genNext(void)
{
  genIndent();
  std::cout << "idx++;\n";
  genIndent();
  std::cout << "while (idx >= memory.size()) {\n";
  genIndent();
  std::cout << indent << "memory.resize(memory.size() * 2);\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::genPrev(void)
{
  genIndent();
  std::cout << "idx--;\n";
}


inline void
GeneratorCpp::genNextN(int value)
{
  genIndent();
  std::cout << "idx += " << value << ";\n";
  genIndent();
  std::cout << "while (idx >= memory.size()) {\n";
  genIndent();
  std::cout << indent << "memory.resize(memory.size() * 2);\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::genPrevN(int value)
{
  genIndent();
  std::cout << "idx -= " << value << ";\n";
}


inline void
GeneratorCpp::genInc(void)
{
  genIndent();
  std::cout << "memory[idx]++;\n";
}


inline void
GeneratorCpp::genDec(void)
{
  genIndent();
  std::cout << "memory[idx]--;\n";
}


inline void
GeneratorCpp::genAdd(int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << ";\n";
}


inline void
GeneratorCpp::genSub(int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << ";\n";
}


inline void
GeneratorCpp::genPutchar(void)
{
  genIndent();
  std::cout << "std::cout.put(memory[idx]);\n";
}


inline void
GeneratorCpp::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = static_cast<unsigned char>(std::cin.get());\n";
}


inline void
GeneratorCpp::genLoopStart(void)
{
  genIndent();
  std::cout << "while (memory[idx]) {\n";
  indentLevel++;
}


inline void
GeneratorCpp::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::genAssign(int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << ";\n";
}


inline void
GeneratorCpp::genAddVar(int value)
{
  genIndent();
  std::cout << "if (memory[idx]) {\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " <<  value << "] += memory[idx];\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] += memory[idx];\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::genSubVar(int value)
{
  genIndent();
  std::cout << "if (memory[idx]) {\n";
  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " <<  value << "] -= memory[idx];\n";
  } else {
    std::cout << indent << "memory[idx - " << -value << "] -= memory[idx];\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
  genIndent();
  std::cout << "}\n";
}


}  // namespace bf
#endif  // GENERATOR_CPP_H
