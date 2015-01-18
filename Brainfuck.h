#ifndef BRAINFUCK_H
#define BRAINFUCK_H


#include <vector>


/*!
 * @brief Brainfuck interpreter
 */
class Brainfuck {
private:
  typedef enum {
    PTR_ADD,
    PTR_SUB,
    ADD,
    SUB,
    PUTCHAR,
    GETCHAR,
    LOOP_START,
    LOOP_END
  } Instruction;

  typedef enum {
     NO_COMPILE,
     NORMAL
  } CompileType;

  struct Command {
    Instruction  type;
    unsigned int value;
  };

  std::size_t memorySize;
  char *sourceBuffer;
  std::vector<Command> commands;
  CompileType compileType;

  void interpretExecute(void);
  void compileExecute(void);

public:
  Brainfuck(std::size_t memorySize=65536) :
    memorySize(memorySize),
    sourceBuffer(NULL),
    commands(0),
    compileType(NO_COMPILE) {}
  ~Brainfuck(void);

  void load(const char *filename);
  void trim(void);
  void execute(void);
  void compile(void);
};


#endif
