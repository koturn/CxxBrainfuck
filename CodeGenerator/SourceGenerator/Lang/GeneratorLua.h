#ifndef GENERATOR_LUA_H
#define GENERATOR_LUA_H


#include "../SourceGenerator.h"


namespace bf {


class GeneratorLua : public SourceGenerator {
private:
  inline void genNilCheck(int value);
protected:
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genNextN(int value);
  inline void genPrevN(int value);
  inline void genAdd(int value);
  inline void genSub(int value);
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
  GeneratorLua(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE,
      const char *indent="  ") :
    SourceGenerator(irCode, indent, 0) {}
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
GeneratorLua::genNextN(int value)
{
  genIndent();
  std::cout << "idx = idx + " << value << "\n";
  genNilCheck(0);
}


inline void
GeneratorLua::genPrevN(int value)
{
  genIndent();
  std::cout << "idx = idx - " << value << "\n";
  genNilCheck(0);
}


inline void
GeneratorLua::genAdd(int value)
{
  genIndent();
  std::cout << "memory[idx] = memory[idx] + " << value << "\n";
}


inline void
GeneratorLua::genSub(int value)
{
  genIndent();
  std::cout << "memory[idx] = memory[idx] - " << value << "\n";
}


inline void
GeneratorLua::genAddAt(int value1, int value2)
{
  genNilCheck(value1);
  genIndent();
  std::cout << "memory[idx ";
  if (value1 > 0) {
    std::cout << "+ " << value1;
  } else {
    std::cout << "- " << -value1;
  }
  std::cout << "] = memory[idx ";
  if (value1 > 0) {
    std::cout << "+ " << value1;
  } else {
    std::cout << "- " << -value1;
  }
  std::cout << "] + " << value2 << "\n";
}


inline void
GeneratorLua::genSubAt(int value1, int value2)
{
  genNilCheck(value1);
  genIndent();
  std::cout << "memory[idx ";
  if (value1 > 0) {
    std::cout << "+ " << value1;
  } else {
    std::cout << "- " << -value1;
  }
  std::cout << "] = memory[idx ";
  if (value1 > 0) {
    std::cout << "+ " << value1;
  } else {
    std::cout << "- " << -value1;
  }
  std::cout << "] - " << value2 << "\n";
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
GeneratorLua::genAssign(int value)
{
  genIndent();
  std::cout << "memory[idx] = " << value << "\n";
}


inline void
GeneratorLua::genAssignAt(int value1, int value2)
{
  genNilCheck(value1);
  genIndent();
  if (value1 > 0) {
    std::cout << "memory[idx + " << value1;
  } else {
    std::cout << "memory[idx - " << -value1;
  }
  std::cout << "] = " << value2 << "\n";
}


inline void
GeneratorLua::genSearchZero(int value)
{
  genIndent();
  std::cout << "while memory[idx] ~= 0 do\n";
  indentLevel++;
  genIndent();
  if (value > 0) {
    std::cout << "idx = idx + " << value << "\n";
  } else if (value < 0) {
    std::cout << "idx = idx - " << -value << "\n";
  }
  genNilCheck(0);
  indentLevel--;
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genAddVar(int value)
{
  genIndent();
  std::cout << "if memory[idx] ~= 0 then\n";

  indentLevel++;
  genNilCheck(value);
  indentLevel--;

  // genIndent();
  // if (value >= 0) {
  //   std::cout << indent << "if memory[idx + " <<  value << "] == nil then\n";
  // } else {
  //   std::cout << indent << "if memory[idx - " << -value << "] == nil then\n";
  // }
  // genIndent();
  // if (value >= 0) {
  //   std::cout << indent << indent << "memory[idx + " <<  value << "] = 0\n";
  // } else {
  //   std::cout << indent << indent << "memory[idx - " << -value << "] = 0\n";
  // }
  // genIndent();
  // std::cout << indent << "end\n";

  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " << value
              << "] = memory[idx + " << value << "] + memory[idx]\n";
  } else {
    std::cout << indent << "memory[idx - " << -value
              << "] = memory[idx - " << -value << "] + memory[idx]\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genSubVar(int value)
{
  genIndent();
  std::cout << "if memory[idx] ~= 0 then\n";

  indentLevel++;
  genNilCheck(value);
  indentLevel--;

  // genIndent();
  // if (value >= 0) {
  //   std::cout << indent << "if memory[idx + " <<  value << "] == nil then\n";
  // } else {
  //   std::cout << indent << "if memory[idx - " << -value << "] == nil then\n";
  // }
  // genIndent();
  // if (value >= 0) {
  //   std::cout << indent << indent << "memory[idx + " <<  value << "] = 0\n";
  // } else {
  //   std::cout << indent << indent << "memory[idx - " << -value << "] = 0\n";
  // }
  // genIndent();
  // std::cout << indent << "end\n";

  genIndent();
  if (value >= 0) {
    std::cout << indent << "memory[idx + " << value
              << "] = memory[idx + " << value << "] - memory[idx]\n";
  } else {
    std::cout << indent << "memory[idx - " << -value
              << "] = memory[idx - " << -value << "] - memory[idx]\n";
  }
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genCmulVar(int value1, int value2)
{
  genIndent();
  std::cout << "if memory[idx] ~= 0 then\n";
  indentLevel++;
  genNilCheck(value1);
  indentLevel--;
  genIndent();
  if (value1 >= 0) {
    std::cout << indent << "memory[idx + " << value1
              << "] = memory[idx + " << value1;
  } else {
    std::cout << indent << "memory[idx - " << -value1
              << "] = memory[idx - " << -value1;
  }
  std::cout << "] + memory[idx] * " << value2;
  genIndent();
  std::cout << indent << "memory[idx] = 0\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genInfLoop(void)
{
  genIndent();
  std::cout << "if memory[idx] ~= 0 then\n";
  genIndent();
  std::cout << indent << "while true do end\n";
  genIndent();
  std::cout << "end\n";
}


inline void
GeneratorLua::genNilCheck(int value)
{
  genIndent();
  std::cout << "if memory[idx";
  if (value > 0) {
    std::cout << " + " << value;
  } else if (value < 0) {
    std::cout << " - " << -value;
  }
  std::cout << "] == nil then\n";
  genIndent();
  std::cout << indent << "memory[idx";
  if (value > 0) {
    std::cout << " + " << value;
  } else if (value < 0) {
    std::cout << " - " << -value;
  }
  std::cout << "] = 0\n";
  genIndent();
  std::cout << "end\n";
}


}  // namespace bf
#endif  // GENERATOR_LUA_H
