#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>

#include "Brainfuck.h"


static const char *
findLoopEnd(const char *srcptr);

static const char *
findLoopStart(const char *srcptr);

static unsigned int
countChar(const char *srcptr, char ch);


/*!
 * @brief Destructor: Delete alocated memory
 */
Brainfuck::~Brainfuck(void)
{
  delete[] sourceBuffer;
}


/*!
 * @brief Load Brainfuck source code
 * @param [in] filename  File path
 */
void
Brainfuck::load(const char *filename)
{
  std::ifstream ifs(filename);
  if (ifs.fail()) {
    throw "Cannot open file";
  }
  ifs.seekg(0, ifs.end);
  std::size_t fileSize = static_cast<std::size_t>(ifs.tellg());
  ifs.seekg(0, ifs.beg);

  sourceBuffer = new char[fileSize];
  sourceBuffer[fileSize - 1] = '\0';
  ifs.read(sourceBuffer, fileSize);
}


/*!
 * @brief [in] Delete not Brainfuck-characters
 */
void
Brainfuck::trim(void)
{
  const char *srcptr = sourceBuffer;
  char *dstptr = sourceBuffer;
  for (; *srcptr != '\0'; srcptr++) {
    switch (*srcptr) {
      case '>':
      case '<':
      case '+':
      case '-':
      case '.':
      case ',':
      case '[':
      case ']':
        *dstptr++ = *srcptr;
        break;
    }
  }
  *dstptr = '\0';
}


/*!
 * @brief Execute brainfuck source code
 */
void
Brainfuck::execute(void)
{
  switch (compileType) {
    case NO_COMPILE:
      interpretExecute();
      break;
    case NORMAL:
      compileExecute();
      break;
  }
  std::cout.put('\n');
}


/*!
 * @brief Execute brainfuck without compile.
 */
void
Brainfuck::interpretExecute(void)
{
  unsigned char *memory = new unsigned char[memorySize];
  std::memset(memory, 0, memorySize);
  unsigned char *ptr = memory;
  for (const char *srcptr = sourceBuffer; *srcptr != '\0'; srcptr++) {
    switch (*srcptr) {
      case '>': ptr++;    break;
      case '<': ptr--;    break;
      case '+': (*ptr)++; break;
      case '-': (*ptr)--; break;
      case '.':
        std::cout.put(static_cast<char>(*ptr));
        break;
      case ',':
        *ptr = static_cast<unsigned char>(std::cin.get());
        break;
      case '[':
        if (*ptr != 0) break;
        srcptr = findLoopEnd(srcptr);
        break;
      case ']':
        if (*ptr == 0) break;
        srcptr = findLoopStart(srcptr);
        break;
    }
  }
  delete[] memory;
}


/*!
 * @brief Execute compiled brainfuck source code
 */
void
Brainfuck::compileExecute(void)
{
  unsigned char *memory = new unsigned char[memorySize];
  std::memset(memory, 0, memorySize);
  unsigned char *ptr = memory;
  for (std::vector<Command>::size_type pc = 0, size = commands.size(); pc < size; pc++) {
    switch (commands[pc].type) {
      case PTR_ADD:
        ptr += commands[pc].value;
        break;
      case PTR_SUB:
        ptr -= commands[pc].value;
        break;
      case ADD:
        *ptr += commands[pc].value;
        break;
      case SUB:
        *ptr -= commands[pc].value;
        break;
      case PUTCHAR:
        std::cout.put(static_cast<char>(*ptr));
        break;
      case GETCHAR:
        *ptr = static_cast<unsigned char>(std::cin.get());
        break;
      case LOOP_START:
        if (*ptr == 0) {
          pc = commands[pc].value;
        }
        break;
      case LOOP_END:
        if (*ptr != 0) {
          pc = commands[pc].value;
        }
        break;
    }
  }
  delete[] memory;
}


/*!
 * @brief Compile brainfuck code to Instruction set
 */
void
Brainfuck::compile(void)
{
  std::stack<unsigned int> loopStack;
  for (const char *srcptr = sourceBuffer; *srcptr != '\0'; srcptr++) {
    Command cmd;
    switch (*srcptr) {
      case '>':
        cmd.type = PTR_ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '<':
        cmd.type = PTR_SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '+':
        cmd.type = ADD;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '-':
        cmd.type = SUB;
        cmd.value = countChar(srcptr, *srcptr);
        srcptr += cmd.value - 1;
        break;
      case '.':
        cmd.type = PUTCHAR;
        cmd.value = 0;
        break;
      case ',':
        cmd.type = GETCHAR;
        cmd.value = 0;
        break;
      case '[':
        cmd.type = LOOP_START;
        loopStack.push(static_cast<unsigned int>(commands.size()));
        break;
      case ']':
        cmd.type = LOOP_END;
        cmd.value = loopStack.top();
        commands[loopStack.top()].value = static_cast<unsigned int>(commands.size() - 1);
        loopStack.pop();
        break;
    }
    commands.push_back(cmd);
  }
  compileType = NORMAL;
}


/*!
 * @brief Find the end of loop
 * @param [in] code  Pointer to the brainfuck source code
 * @return  Pointer to the end of loop of brainfuck source code
 */
static const char *
findLoopEnd(const char *srcptr)
{
  char ch;
  int depth = 1;
  while (ch = *++srcptr, depth > 0) {
    switch (ch) {
      case '[':  depth++; break;
      case ']':  depth--; break;
      case '\0': throw "Cannod find the end of loop";
    }
  }
  return srcptr - 1;
}


/*!
 * @brief Find the start of loop
 * @param [in] code  Pointer to the brainfuck source code
 * @return  Pointer to the start of loop of brainfuck source code
 */
static const char *
findLoopStart(const char *srcptr)
{
  char ch;
  int depth = 1;
  while (ch = *--srcptr, depth > 0) {
    switch (ch) {
      case '[': depth--; break;
      case ']': depth++; break;
    }
  }
  return srcptr;
}


/*!
 * @brief Count how many given character are continuous.
 * @return The number of consecutive characters
 */
static unsigned int
countChar(const char *srcptr, char ch)
{
  unsigned int cnt = 1;
  while (*++srcptr == ch) {
    cnt++;
  }
  return cnt;
}
