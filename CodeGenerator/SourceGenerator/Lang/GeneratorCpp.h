#ifndef GENERATOR_CPP_H
#define GENERATOR_CPP_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorCpp : public SourceGenerator {
private:
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
GeneratorCpp::genPtrAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "idx++;\n";
  } else {
    genIndent();
    std::cout << "idx += " << value << ";\n";
  }
  genIndent();
  std::cout << "while (idx >= memory.size()) {\n";
  genIndent();
  std::cout << indent << "memory.resize(memory.size() * 2);\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCpp::genPtrSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorCpp::genAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorCpp::genSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
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
GeneratorCpp::genAssign(unsigned int value)
{
  genIndent();
  std::cout << "*ptr = " << value << ";\n";
}


}  // namespace bf
#endif  // GENERATOR_CPP_H
