#ifndef GENERATOR_C_H
#define GENERATOR_C_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorC : public SourceGenerator {
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
  inline void genAssignZero(void);
public:
  GeneratorC(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorC::genHeader(void)
{
  std::cout << "#include <stdio.h>\n"
               "#include <stdlib.h>\n\n"
               "#define MEMORY_SIZE 65536\n\n"
               "int\n"
               "main(void)\n"
               "{\n"
            << indent << "static unsigned char memory[MEMORY_SIZE] = {0};\n"
            << indent << "unsigned char *ptr = memory;\n\n";
}


inline void
GeneratorC::genFooter(void)
{
  std::cout << "\n";
  std::cout << indent << "return EXIT_SUCCESS;\n"
               "}"
            << std::endl;
}


inline void
GeneratorC::genPtrAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "ptr++;\n";
  } else {
    std::cout << "ptr += " << value << ";\n";
  }
}


inline void
GeneratorC::genPtrSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "ptr--;\n";
  } else {
    std::cout << "ptr -= " << value << ";\n";
  }
}


inline void
GeneratorC::genAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "(*ptr)++;\n";
  } else {
    std::cout << "*ptr += " << value << ";\n";
  }
}


inline void
GeneratorC::genSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "(*ptr)--;\n";
  } else {
    std::cout << "*ptr -= " << value << ";\n";
  }
}


inline void
GeneratorC::genPutchar(void)
{
  genIndent();
  std::cout << "putchar(*ptr);\n";
}


inline void
GeneratorC::genGetchar(void)
{
  genIndent();
  std::cout << "*ptr = (unsigned char) getchar();";
}


inline void
GeneratorC::genLoopStart(void)
{
  genIndent();
  std::cout << "while (*ptr) {\n";
  indentLevel++;
}


inline void
GeneratorC::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::genAssignZero(void)
{
  genIndent();
  std::cout << "*ptr = 0;\n";
}


}  // namespace bf
#endif  // GENERATOR_C_H
