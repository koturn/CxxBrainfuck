#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Brainfuck.h"


static const std::size_t DEFAULT_MEMORY_SIZE = 65536;


/*!
 * @brief Print the header of translated C-source code
 * @param [in,out] fp  Output file pointer
 */
int
main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Invalid argument" << std::endl;
    return EXIT_FAILURE;
  }
  try {
    Brainfuck bf(DEFAULT_MEMORY_SIZE);
    bf.load(argv[1]);
    bf.trim();
#ifdef _MSC_VER
    bf.compile(Brainfuck::NORMAL_COMPILE);
    bf.generateWinBinary();
    std::ofstream fout("output.exe", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!fout) {
      throw "Cannot open file";
    }
    fout.write(reinterpret_cast<const char *>(bf.getWinBinary()), bf.getWinBinarySize());

    bf.compile(Brainfuck::XBYAK_JIT_COMPILE);
    bf.execute();
#else
    bf.compile();
    bf.execute();
#endif  // _MSC_VER
  } catch (const char *errmsg) {
    std::cerr << errmsg << std::endl;
    return EXIT_FAILURE;
  } catch (std::bad_alloc) {
    std::cerr << "Cannot allocate memory" << std::endl;
    return EXIT_FAILURE;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
