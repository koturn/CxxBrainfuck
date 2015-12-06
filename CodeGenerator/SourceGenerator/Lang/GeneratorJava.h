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
GeneratorJava::genNextN(int value)
{
  genIndent();
  std::cout << "idx += " << value << ";\n";
}


inline void
GeneratorJava::genPrevN(int value)
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
GeneratorJava::genAdd(int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << ";\n";
}


inline void
GeneratorJava::genSub(int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << ";\n";
}


inline void
GeneratorJava::genIncAt(int value)
{
  genIndent();
  if (value > 0) {
    std::cout << "memory[idx + " << value;
  } else {
    std::cout << "memory[idx - " << -value;
  }
  std::cout << "]++;\n";
}


inline void
GeneratorJava::genDecAt(int value)
{
  genIndent();
  if (value > 0) {
    std::cout << "memory[idx + " << value;
  } else {
    std::cout << "memory[idx - " << -value;
  }
  std::cout << "]--;\n";
}


inline void
GeneratorJava::genAddAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "memory[idx + " << value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] += " << value2 << ";\n";
}


inline void
GeneratorJava::genSubAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "memory[idx + " << value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] -= " << value2 << ";\n";
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
GeneratorJava::genAssign(int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << ";\n";
}


inline void
GeneratorJava::genAssignAt(int value1, int value2)
{
  genIndent();
  if (value1 > 0) {
    std::cout << "memory[idx + " << value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] = " << value2 << ";\n";
}


inline void
GeneratorJava::genSearchZero(int value)
{
  genIndent();
  std::cout << "for (; memory[idx] != 0; idx";
  if (value == 1) {
    std::cout << "++);\n";
  } else if (value == -1) {
    std::cout << "--);\n";
  } else if (value > 1) {
    std::cout << " += " << value << ");\n";
  } else if (value < 1) {
    std::cout << " -= " << -value << ");\n";
  }
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


inline void
GeneratorJava::genCmulVar(int value1, int value2)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
  genIndent(); std::cout << indent;
  if (value1 >= 0) {
    std::cout << "memory[idx + " <<  value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] += memory[idx] * " << value2 << ";\n";
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorJava::genInfLoop(void)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
  genIndent();
  std::cout << indent << "for (;;);\n";
  genIndent();
  std::cout << "}\n";
}


}  // namespace bf
#endif  // GENERATOR_JAVA_H
