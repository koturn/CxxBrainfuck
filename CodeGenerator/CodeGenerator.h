#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#ifndef __UNUSED__
#  if defined(__GNUC__)
#    define __UNUSED__(x)  __UNUSED___ ## x __attribute__((unused))
#  elif defined(_MSC_VER)
#    define __UNUSED__(x)  __pragma(warning(suppress: 4100)) x
#  elif defined(__LCLINT__)
#    define __UNUSED__(x)  /*@unused@*/ x
#  elif defined(__cplusplus)
#    define __UNUSED__(x)
#  else
#    define __UNUSED__(x)  x
#  endif
#endif


class CodeGenerator {
protected:
  int indentLevel;
  const char *indent;

  inline void printIndent(void);
public:
  CodeGenerator(const char *indent="  ") :
    indentLevel(1), indent(indent) {}
  virtual void printHeader(void) {}
  virtual void printFooter(void) {}
  virtual void printPtrAdd(unsigned int __UNUSED__(value)) {}
  virtual void printPtrSub(unsigned int __UNUSED__(value)) {}
  virtual void printAdd(unsigned int __UNUSED__(value)) {}
  virtual void printSub(unsigned int __UNUSED__(value)) {}
  virtual void printPutchar(void) {}
  virtual void printGetchar(void) {}
  virtual void printLoopStart(void) {}
  virtual void printLoopEnd(void) {}
  virtual void printAssignZero(void) {
    printLoopStart();
    printSub(1);
    printLoopEnd();
  }
};


inline void 
CodeGenerator::printIndent(void)
{
  for (int i = 0; i < indentLevel; i++) {
    std::cout << indent;
  }
}


#endif  // CODE_GENERATOR
