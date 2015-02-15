#ifndef GENERATOR_C_H
#define GENERATOR_C_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorC : public SourceGenerator {
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
  inline void genIncAt(int value);
  inline void genDecAt(int value1);
  inline void genAddAt(int value1, int value2);
  inline void genSubAt(int value1, int value2);
  inline void genPutchar(void);
  inline void genGetchar(void);
  inline void genLoopStart(void);
  inline void genLoopEnd(void);
  inline void genAssign(int value);
  inline void genAssignAt(int value1, int value2);
  inline void genSearchZero(int value);
  inline void genAddVar(int value);
  inline void genSubVar(int value);
  inline void genCmulVar(int value1, int value2);
  inline void genInfLoop(void);
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
GeneratorC::genNext(void)
{
  genIndent();
  std::cout << "ptr++;\n";
}


inline void
GeneratorC::genPrev(void)
{
  genIndent();
  std::cout << "ptr--;\n";
}


inline void
GeneratorC::genNextN(int value)
{
  genIndent();
  std::cout << "ptr += " << value << ";\n";
}


inline void
GeneratorC::genPrevN(int value)
{
  genIndent();
  std::cout << "ptr -= " << value << ";\n";
}


inline void
GeneratorC::genInc(void)
{
  genIndent();
  std::cout << "(*ptr)++;\n";
}


inline void
GeneratorC::genDec(void)
{
  genIndent();
  std::cout << "(*ptr)--;\n";
}


inline void
GeneratorC::genAdd(int value)
{
  genIndent();
  std::cout << "*ptr += " << value << ";\n";
}


inline void
GeneratorC::genSub(int value)
{
  genIndent();
  std::cout << "*ptr -= " << value << ";\n";
}


inline void
GeneratorC::genIncAt(int value)
{
  genIndent();
  if (value > 0) {
    std::cout << "(*(ptr + " << value;
  } else {
    std::cout << "(*(ptr - " << -value;
  }
  std::cout << "))++;\n";
}


inline void
GeneratorC::genDecAt(int value)
{
  genIndent();
  if (value > 0) {
    std::cout << "(*(ptr + " << value;
  } else {
    std::cout << "(*(ptr - " << -value;
  }
  std::cout << "))--;\n";
}


inline void
GeneratorC::genAddAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "*(ptr + " << value1 << ") ";
  } else {
    std::cout << "*(ptr - " << -value1 << ") ";
  }
  std::cout << "+= " << value2 << ";\n";
}


inline void
GeneratorC::genSubAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "*(ptr + " << value1 << ") ";
  } else {
    std::cout << "*(ptr - " << -value1 << ") ";
  }
  std::cout << "-= " << value2 << ";\n";
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
GeneratorC::genAssign(int value)
{
  genIndent();
  std::cout << "*ptr = " << value << ";\n";
}


inline void
GeneratorC::genAssignAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "*(ptr + " << value1 << ") ";
  } else {
    std::cout << "*(ptr - " << -value1 << ") ";
  }
  std::cout << "= " << value2 << ";\n";
}


inline void
GeneratorC::genSearchZero(int value)
{
  genIndent();
  std::cout << "while (*ptr) {\n";
  genIndent(); std::cout << indent;
  if (value == 1) {
    std::cout << "ptr++;\n";
  } else if (value == -1) {
    std::cout << "ptr--;\n";
  } else if (value > 0) {
    std::cout << "ptr += " << value << ";\n";
  } else if (value < 0) {
    std::cout << "ptr -= " << value << ";\n";
  }
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::genAddVar(int value)
{
  genIndent();
  std::cout << "if (*ptr) {\n";
  genIndent(); std::cout << indent;
  if (value >= 0) {
    std::cout << "*(ptr + " <<  value;
  } else {
    std::cout << "*(ptr - " << -value;
  }
  std::cout << ") += *ptr;\n";
  genIndent();
  std::cout << indent << "*ptr = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::genSubVar(int value)
{
  genIndent();
  std::cout << "if (*ptr) {\n";
  genIndent(); std::cout << indent;
  if (value >= 0) {
    std::cout << "*(ptr + " <<  value;
  } else {
    std::cout << "*(ptr - " << -value;
  }
  std::cout << ") -= *ptr;\n";;
  genIndent();
  std::cout << indent << "*ptr = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::genCmulVar(int value1, int value2)
{
  genIndent();
  std::cout << "if (*ptr) {\n";
  genIndent(); std::cout << indent;
  if (value1 >= 0) {
    std::cout << "*(ptr + " <<  value1;
  } else {
    std::cout << "*(ptr - " << -value1;
  }
  std::cout << ") += *ptr * " << value2 << ";\n";
  genIndent();
  std::cout << indent << "*ptr = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorC::genInfLoop(void)
{
  genIndent();
  std::cout << "if (*ptr) {\n";
  genIndent();
  std::cout << indent << "for (;;);\n";
  genIndent();
  std::cout << "}\n";
}


}  // namespace bf
#endif  // GENERATOR_C_H
