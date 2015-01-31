#ifndef GENERATOR_LUA
#define GENERATOR_LUA


#include "../CodeGenerator.h"


class GeneratorLua : public CodeGenerator {
public:
  GeneratorLua(const char *indent="  ") :
    CodeGenerator(indent) {}
  inline void printHeader(void);
  inline void printFooter(void);
  inline void printPtrAdd(unsigned int value);
  inline void printPtrSub(unsigned int value);
  inline void printAdd(unsigned int value);
  inline void printSub(unsigned int value);
  inline void printPutchar(void);
  inline void printGetchar(void);
  inline void printLoopStart(void);
  inline void printLoopEnd(void);
  inline void printAssignZero(void);
};




inline void
GeneratorLua::printHeader(void)
{
  std::cout << "#!/usr/bin/env lua\n\n"
               "local memory = {0}\n"
               "local idx = 1\n\n";
  indentLevel = 0;
}


inline void
GeneratorLua::printFooter(void)
{
  // Nothing to do
}


inline void
GeneratorLua::printPtrAdd(unsigned int value)
{
  printIndent();
  std::cout << "idx = idx + " << value << "\n";
  printIndent();
  std::cout << "if memory[idx] == nil then\n";
  printIndent();
  std::cout << indent << "memory[idx] = 0\n";
  printIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::printPtrSub(unsigned int value)
{
  printIndent();
  std::cout << "idx = idx - " << value << "\n";
  printIndent();
  std::cout << "if memory[idx] == nil then\n";
  printIndent();
  std::cout << indent << "memory[idx] = 0\n";
  printIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::printAdd(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] = memory[idx] + " << value << "\n";
}


inline void
GeneratorLua::printSub(unsigned int value)
{
  printIndent();
  std::cout << "memory[idx] = memory[idx] - " << value << "\n";
}


inline void
GeneratorLua::printPutchar(void)
{
  printIndent();
  std::cout << "io.write(string.char(memory[idx]))\n";
}


inline void
GeneratorLua::printGetchar(void)
{
  printIndent();
  std::cout << "memory[idx] = string.byte(io.read(1))\n";
}


inline void
GeneratorLua::printLoopStart(void)
{
  printIndent();
  std::cout << "while memory[idx] ~= 0 do\n";
  indentLevel++;
}


inline void
GeneratorLua::printLoopEnd(void)
{
  indentLevel--;
  printIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::printAssignZero(void)
{
  printIndent();
  std::cout << "memory[idx] = 0\n";
}


#endif  // GENERATOR_LUA
