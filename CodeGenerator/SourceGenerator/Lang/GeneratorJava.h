#ifndef GENERATOR_JAVA_H
#define GENERATOR_JAVA_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorJava : public SourceGenerator {
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
public:
  GeneratorJava(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorJava::genHeader(void)
{
  std::cout << "public class TranslatedBrainfuck {\n"
            << indent << "public static void main(String[] args) {\n"
            << indent << indent << "final int MEMORY_SIZE = 65536;\n"
            << indent << indent << "char[] memory = new char[MEMORY_SIZE];\n"
            << indent << indent << "int idx = 0;\n\n";
}


inline void
GeneratorJava::genFooter(void)
{
  std::cout << indent << "}\n"
               "}"
            << std::endl;
}


inline void
GeneratorJava::genPtrAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "idx++;\n";
  } else {
    genIndent();
    std::cout << "idx += " << value << ";\n";
  }
}


inline void
GeneratorJava::genPtrSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorJava::genAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorJava::genSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
}


inline void
GeneratorJava::genPutchar(void)
{
  genIndent();
  std::cout << "System.out.print(memory[idx]);\n";
}


inline void
GeneratorJava::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = System.in.read();\n";
}


inline void
GeneratorJava::genLoopStart(void)
{
  genIndent();
  std::cout << "while (memory[idx] != 0) {\n";
  indentLevel++;
}


inline void
GeneratorJava::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorJava::genAssign(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << ";\n";
}


}  // namespace bf
#endif  // GENERATOR_JAVA_H
