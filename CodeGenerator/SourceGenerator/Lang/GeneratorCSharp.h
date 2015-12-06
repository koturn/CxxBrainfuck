#ifndef GENERATOR_CSHARP_H
#define GENERATOR_CSHARP_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorCSharp : public SourceGenerator {
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
  GeneratorCSharp(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="    ") :
    SourceGenerator(irCode, indent, 2) {}
};




inline void
GeneratorCSharp::genHeader(void)
{
  std::cout << "using System;\n\n"
               "public class TranslatedBrainfuck\n"
               "{\n"
            << indent << "public static void Main()\n"
            << indent << "{\n"
            << indent << indent << "const int MEMORY_SIZE = 65536;\n"
            << indent << indent << "byte[] memory = new byte[MEMORY_SIZE];\n"
            << indent << indent << "int idx = 0;\n\n";
}


inline void
GeneratorCSharp::genFooter(void)
{
  std::cout << indent << "}\n"
               "}"
            << std::endl;
}


inline void
GeneratorCSharp::genNext(void)
{
  genIndent();
  std::cout << "idx++;\n";
}


inline void
GeneratorCSharp::genPrev(void)
{
  genIndent();
  std::cout << "idx--;\n";
}


inline void
GeneratorCSharp::genNextN(int value)
{
  genIndent();
  std::cout << "idx += " << value << ";\n";
}


inline void
GeneratorCSharp::genPrevN(int value)
{
  genIndent();
  std::cout << "idx -= " << value << ";\n";
}


inline void
GeneratorCSharp::genInc(void)
{
  genIndent();
  std::cout << "memory[idx]++;\n";
}


inline void
GeneratorCSharp::genDec(void)
{
  genIndent();
  std::cout << "memory[idx]--;\n";
}


inline void
GeneratorCSharp::genAdd(int value)
{
  genIndent();
  std::cout << "memory[idx] += " << value << ";\n";
}


inline void
GeneratorCSharp::genSub(int value)
{
  genIndent();
  std::cout << "memory[idx] -= " << value << ";\n";
}


inline void
GeneratorCSharp::genIncAt(int value)
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
GeneratorCSharp::genDecAt(int value)
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
GeneratorCSharp::genAddAt(int value1, int value2)
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
GeneratorCSharp::genSubAt(int value1, int value2)
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
GeneratorCSharp::genPutchar(void)
{
  genIndent();
  std::cout << "Console.Write((char) memory[idx]);\n";
}


inline void
GeneratorCSharp::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = Console.Read();\n";
}


inline void
GeneratorCSharp::genLoopStart(void)
{
  genIndent();
  std::cout << "while (memory[idx] != 0)\n";
  genIndent();
  std::cout << "{\n";
  indentLevel++;
}


inline void
GeneratorCSharp::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCSharp::genAssign(int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << ";\n";
}


inline void
GeneratorCSharp::genAssignAt(int value1, int value2)
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
GeneratorCSharp::genSearchZero(int value)
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
GeneratorCSharp::genAddVar(int value)
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
GeneratorCSharp::genSubVar(int value)
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
GeneratorCSharp::genCmulVar(int value1, int value2)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
  genIndent(); std::cout << indent;
  if (value1 >= 0) {
    std::cout << "memory[idx + " <<  value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] += (byte) (memory[idx] * " << value2 << ");\n";
  genIndent();
  std::cout << indent << "memory[idx] = 0;\n";
  genIndent();
  std::cout << "}\n";
}


inline void
GeneratorCSharp::genInfLoop(void)
{
  genIndent();
  std::cout << "if (memory[idx] != 0) {\n";
  genIndent();
  std::cout << indent << "for (;;);\n";
  genIndent();
  std::cout << "}\n";
}


}  // namespace bf
#endif  // GENERATOR_CSHARP_H
