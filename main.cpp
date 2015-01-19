#include <cstdlib>
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
#ifdef __CYGWIN__
    bf.compile();
#else
    bf.compile(Brainfuck::XBYAK_JIT_COMPILE);
#endif
    bf.translate(Brainfuck::LANG_CPP);
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
