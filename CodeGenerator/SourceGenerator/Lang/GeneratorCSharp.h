#ifndef GENERATOR_CSHARP_H
#define GENERATOR_CSHARP_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorCSharp : public SourceGenerator {
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
  GeneratorCSharp(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
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
GeneratorCSharp::genPtrAdd(unsigned int value)
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
GeneratorCSharp::genPtrSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "idx--;\n";
  } else {
    std::cout << "idx -= " << value << ";\n";
  }
}


inline void
GeneratorCSharp::genAdd(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]++;\n";
  } else {
    std::cout << "memory[idx] += " << value << ";\n";
  }
}


inline void
GeneratorCSharp::genSub(unsigned int value)
{
  genIndent();
  if (value == 1) {
    std::cout << "memory[idx]--;\n";
  } else {
    std::cout << "memory[idx] -= " << value << ";\n";
  }
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
GeneratorCSharp::genAssign(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << ";\n";
}


}  // namespace bf
#endif  // GENERATOR_CSHARP_H
