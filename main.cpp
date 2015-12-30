#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <getopt.h>
#include "Brainfuck.h"


class OptionParser {
public:
  typedef enum {
    STATUS_OK,
    STATUS_EXIT,
    STATUS_ERROR
  } Status;

  OptionParser(int argc, char *argv[]) :
    argc(argc), optLevel(1), memorySize(DEFAULT_MEMORY_SIZE),
    status(STATUS_OK), argv(argv), programName(argv[0]), inFilename(NULL),
    target(NULL) {}
  void parse(void);
  void help(void) const;
  int getOptLevel(void) const { return optLevel; }
  std::size_t getMemorySize(void) const { return memorySize; }
  Status getStatus(void) const { return status; }
  const char *getInFilename(void) const { return inFilename; }
  const char *getTarget(void) const { return target; }

private:
  static const std::size_t DEFAULT_MEMORY_SIZE = 65536;
  int argc;
  int optLevel;
  std::size_t memorySize;
  Status status;
  char **argv;
  const char *programName;
  const char *inFilename;
  const char *target;
};

static bool
convertTarget(bf::Brainfuck::LANG *lang, char *target);

static void
toLowerCase(char *str);


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
static const char *DEFAULT_OUTPUT_FILE_NAME = "a.exe";
#else
static const char *DEFAULT_OUTPUT_FILE_NAME = "a.out";
#endif



/*!
 * @brief Print the header of translated C-source code
 * @param [in,out] fp  Output file pointer
 */
int
main(int argc, char *argv[])
{
  try {
    OptionParser op(argc, argv);
    op.parse();
    OptionParser::Status status = op.getStatus();
    if (status == OptionParser::STATUS_EXIT) return EXIT_SUCCESS;
    if (status == OptionParser::STATUS_ERROR) return EXIT_FAILURE;

    bf::Brainfuck bf(op.getMemorySize());
    bf.load(op.getInFilename());
    bf.trim();

    int optLevel = op.getOptLevel();
#ifdef USE_XBYAK
    if (optLevel >= 2) {
      bf.compile(bf::Brainfuck::XBYAK_JIT_COMPILE);
    } else if (optLevel == 1) {
      bf.compile(bf::Brainfuck::NORMAL_COMPILE);
    }
#else
    if (optLevel >= 1) {
      bf.compile(bf::Brainfuck::NORMAL_COMPILE);
    }
#endif

    char *target = const_cast<char *>(op.getTarget());
    bf::Brainfuck::LANG lang;
    if (target == NULL) {
      bf.execute();
    } else if (convertTarget(&lang, target)) {
      bf.translate(lang);
    } else {
#ifdef USE_XBYAK
      if (!std::strcmp(target, "xbyakc")) {
        bf.xbyakDump();
      }
#endif
      if (!std::strcmp(target, "winx86")) {
        bf.generateWinBinary(bf::Brainfuck::WIN_BIN_X86);
      } else if (!std::strcmp(target, "elfx64")) {
        bf.generateWinBinary(bf::Brainfuck::ELF_BIN_X64);
      }
      std::ofstream fout(DEFAULT_OUTPUT_FILE_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
      if (!fout) {
        std::cerr << "Cannot open file: output.exe" << std::endl;
      }
      fout.write(reinterpret_cast<const char *>(bf.getWinBinary()), bf.getWinBinarySize());
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


/*!
 * @brief Parse arguments
 */
void
OptionParser::parse(void)
{
  static const struct option opts[] = {
    {"compile",  required_argument, NULL, 'c'},
    {"help",     no_argument,       NULL, 'h'},
    {"optimize", required_argument, NULL, 'O'},
    {"size",     required_argument, NULL, 's'},
    {0, 0, 0, 0}  // must be filled with zero
  };
  int ret;
  int optidx = 0;
  std::stringstream ss;
  while ((ret = getopt_long(argc, argv, "c:ho:O:s:", opts, &optidx)) != EOF) {
    switch (ret) {
      case 'c':  // -c, --compile
        target = optarg;
        break;
      case 'h':  // -h, --help
        help();
        status = STATUS_EXIT;
        return;
      case 'O':  // -O, --optimize
        ss << optarg;
        ss >> optLevel;
        ss.clear();
        ss.str("");
        break;
      case 's':  // -s, --size
        ss << optarg;
        ss >> memorySize;
        ss.clear();
        ss.str("");
        break;
      case '?':  // unknown option
        help();
        status = STATUS_ERROR;
        return;
    }
  }
  if (optind != argc - 1) {
    std::cerr << "Please specify one brainfuck source code" << std::endl;
    help();
    status = STATUS_ERROR;
    return;
  }
  inFilename = argv[optind];
}


/*!
 * @brief Show usage of this program
 */
void
OptionParser::help(void) const
{
  std::cout << "[Usage]\n"
            << "  $ " << programName << " FILE [options]\n\n"
               "[Options]\n"
               "  -c TARGET, --compile=TARGET\n"
               "    Specify output type\n"
               "      - c:      Compile to C source code\n"
               "      - cpp:    Compile to C++ source code\n"
               "      - csharp: Compile to C# source code\n"
               "      - java:   Compile to Java source code\n"
               "      - lua:    Compile to Lua source code\n"
               "      - python: Compile to Python source code\n"
               "      - ruby:   Compile to Python source code\n"
#ifdef USE_XBYAK
               "      - xbyakc: Compile to C source code dumped from Xbyak Code generator\n"
#endif
               "      - winx86: Compile to x86 Windows executable binary\n"
               "      - elfx64: Compile to x64 Elf binary\n"
               "  -h, --help\n"
               "    Show help and exit this program\n"
               "  -O OPT_LEVEL, --optimize=OPT_LEVEL\n"
               "    Specify optimize level\n"
               "      - 0: Execute with No compile\n"
               "      - 1: Execute with simple compile\n"
#ifdef USE_XBYAK
               "      - 2: Execute with JIT compile\n"
#endif
               "    Default value: OPT_LEVEL = 1\n"
               "  -s MEMORY_SIZE, --size=MEMORY_SIZE\n"
               "    Specify memory size\n"
               "    Default value: MEMORY_SIZE = " << DEFAULT_MEMORY_SIZE << "\n"
            << std::endl;
}


/*!
 * @brief Get constant og target language
 * @param [out] lang    Variable to store constant
 * @param [in]  target  Target language
 * @return Return true if constant of target language is exist, otherwise
 *         false
 */
static bool
convertTarget(bf::Brainfuck::LANG *lang, char *target)
{
  toLowerCase(target);
  if (!std::strcmp(target, "c")) {
    *lang = bf::Brainfuck::LANG_C;
  } else if (!std::strcmp(target, "cpp")) {
    *lang = bf::Brainfuck::LANG_CPP;
  } else if (!std::strcmp(target, "csharp")) {
    *lang = bf::Brainfuck::LANG_CSHARP;
  } else if (!std::strcmp(target, "java")) {
    *lang = bf::Brainfuck::LANG_JAVA;
  } else if (!std::strcmp(target, "lua")) {
    *lang = bf::Brainfuck::LANG_LUA;
  } else if (!std::strcmp(target, "ruby")) {
    *lang = bf::Brainfuck::LANG_RUBY;
  } else if (!std::strcmp(target, "python")) {
    *lang = bf::Brainfuck::LANG_PYTHON;
  } else {
    return false;
  }
  return true;
}


/*!
 * @brief Convert C-string to lower case
 * @param [out] str  C-string
 */
static void
toLowerCase(char *str)
{
  std::size_t size = std::strlen(str);
  for (std::size_t i = 0; i < size; i++) {
    str[i] = static_cast<char>(tolower(str[i]));
  }
}
