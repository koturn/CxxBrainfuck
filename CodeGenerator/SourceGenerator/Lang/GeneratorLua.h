#ifndef GENERATOR_LUA_H
#define GENERATOR_LUA_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorLua : public SourceGenerator {
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
  inline void genAssignZero(void);
public:
  GeneratorLua(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 1) {}
};




inline void
GeneratorLua::genHeader(void)
{
  std::cout << "#!/usr/bin/env lua\n\n"
               "local memory = {0}\n"
               "local idx = 1\n\n";
}


inline void
GeneratorLua::genFooter(void)
{
  // Nothing to do
}


inline void
GeneratorLua::genPtrAdd(unsigned int value)
{
  genIndent();
  std::cout << "idx = idx + " << value << "\n";
  genIndent();
  std::cout << "if memory[idx] == nil then\n";
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genPtrSub(unsigned int value)
{
  genIndent();
  std::cout << "idx = idx - " << value << "\n";
  genIndent();
  std::cout << "if memory[idx] == nil then\n";
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genAdd(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = memory[idx] + " << value << "\n";
}


inline void
GeneratorLua::genSub(unsigned int value)
{
  genIndent();
  std::cout << "memory[idx] = memory[idx] - " << value << "\n";
}


inline void
GeneratorLua::genPutchar(void)
{
  genIndent();
  std::cout << "io.write(string.char(memory[idx]))\n";
}


inline void
GeneratorLua::genGetchar(void)
{
  genIndent();
  std::cout << "memory[idx] = string.byte(io.read(1))\n";
}


inline void
GeneratorLua::genLoopStart(void)
{
  genIndent();
  std::cout << "while memory[idx] ~= 0 do\n";
  indentLevel++;
}


inline void
GeneratorLua::genLoopEnd(void)
{
  indentLevel--;
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genAssignZero(void)
{
  genIndent();
  std::cout << "memory[idx] = 0\n";
}


}  // namespace bf
#endif  // GENERATOR_LUA_H
