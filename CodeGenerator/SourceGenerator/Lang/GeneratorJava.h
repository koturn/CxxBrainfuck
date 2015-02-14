#ifndef GENERATOR_JAVA_H
#define GENERATOR_JAVA_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorJava : public SourceGenerator {
protected:
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genNext(void);
  inline void genPrev(void);
  inline void genNextN(unsigned int value);
  inline void genPrevN(unsigned int value);
  inline void genInc(void);
  inline void genDec(void);
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
  GeneratorJava(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="    ") :
    SourceGenerator(irCode, indent, 2) {}
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
GeneratorJava::genNext(void)
{
  genIndent();
  std::cout << "idx++;\n";
}


inline void
GeneratorJava::genPrev(void)
{
  genIndent();
  std::cout << "idx--;\n";
}


inline void
GeneratorJava::genNextN(unsigned int value)
{
  genIndent();
  std::cout << "idx += " << value << ";\n";
}


inline void
GeneratorJava::genPrevN(unsigned int value)
{
  genIndent();
  std::cout << "idx -= " << value << ";\n";
}


inline void
GeneratorJava::genInc(void)
{
  genIndent();
  std::cout << "memory[idx]++;\n";
}


inline void
GeneratorJava::genDec(void)
{
  genIndent();
  std::cout << "memory[idx]--;\n";
}


inline void
GeneratorJava::genAdd(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << ";\n";
}


inline void
GeneratorJava::genSub(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << ";\n";
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


inline void
GeneratorJava::genAddVar(int value)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
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
GeneratorJava::genSubVar(int value)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
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
#endif  // GENERATOR_JAVA_H
