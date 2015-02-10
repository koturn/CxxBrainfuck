#ifndef GENERATOR_ELF_X64
#define GENERATOR_ELF_X64


#include "../BinaryGenerator.h"
#include "elfsubset.h"


namespace bf {


class GeneratorElfX64 : public BinaryGenerator {
private:
  static const unsigned int HEADER_SIZE = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  static const unsigned int ADDR = 0x08048000;
  inline void genPlorogue(void);
  inline void genEpirogue(void);
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
public:
  GeneratorElfX64(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    BinaryGenerator(irCode) {}
};




inline void
GeneratorElfX64::genPlorogue(void)
{
  codePtr += HEADER_SIZE;
  *codePtr++ = 0x48; *codePtr++ = 0xbb;
  *reinterpret_cast<uint64_t *>(codePtr) = ADDR + 0x200000; codePtr += sizeof(uint64_t);
}


inline void
GeneratorElfX64::genEpirogue(void)
{
  *codePtr++ = 0xb8; *codePtr++ = 0x3c; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;
  *codePtr++ = 0xbf; *codePtr++ = 0x2a; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;
  *codePtr++ = 0x0f; *codePtr++ = 0x05;
  codeSize = codePtr - (code + HEADER_SIZE);
}


inline void
GeneratorElfX64::genHeader(void)
{
  unsigned char *ptr = code;

  // ELF header
  Elf64_Ehdr *ehdr = reinterpret_cast<Elf64_Ehdr *>(ptr);
  ehdr->e_ident[EI_MAG0] = ELFMAG0;
  ehdr->e_ident[EI_MAG1] = ELFMAG1;
  ehdr->e_ident[EI_MAG2] = ELFMAG2;
  ehdr->e_ident[EI_MAG3] = ELFMAG3;
  ehdr->e_ident[EI_CLASS] = ELFCLASS64;
  ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
  ehdr->e_ident[EI_VERSION] = EV_CURRENT;
  ehdr->e_ident[EI_OSABI] = 0x00;
  ehdr->e_ident[EI_ABIVERSION] = 0x00;
  ehdr->e_ident[EI_PAD] = 0x00;
  ehdr->e_type = ET_EXEC;
  ehdr->e_machine = EM_X86_64;
  ehdr->e_version = EV_CURRENT;
  ehdr->e_entry = ADDR + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  ehdr->e_phoff = sizeof(Elf64_Ehdr);
  ehdr->e_shoff = sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * 2) + codeSize + 22;
  ehdr->e_flags = 0x00000000;
  ehdr->e_ehsize = sizeof(Elf64_Ehdr);
  ehdr->e_phentsize = sizeof(Elf64_Phdr);
  ehdr->e_phnum = 2;
  ehdr->e_shentsize = sizeof(Elf64_Shdr);
  ehdr->e_shnum = 4;
  ehdr->e_shstrndx = 1;
  ptr += sizeof(Elf64_Ehdr);

  // Program header
  Elf64_Phdr *phdr = reinterpret_cast<Elf64_Phdr *>(ptr);
  phdr->p_type = PT_LOAD;
  phdr->p_flags = PF_R | PF_X;
  phdr->p_offset = 0x0000000000000000;
  phdr->p_vaddr = ADDR;
  phdr->p_paddr = ADDR;
  phdr->p_filesz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize + 22 + sizeof(Elf64_Shdr) * 4;
  phdr->p_memsz = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize + 22 + sizeof(Elf64_Shdr) * 4;
  phdr->p_align = 0x0000000000000100;
  ptr += sizeof(Elf64_Phdr);

  // program header for .bss (56 bytes)
  phdr = reinterpret_cast<Elf64_Phdr *>(ptr);
  phdr->p_type = PT_LOAD;
  phdr->p_flags = PF_R | PF_W;
  phdr->p_offset = 0x0000000000001000;
  phdr->p_vaddr = ADDR + 0x200000;
  phdr->p_paddr = ADDR + 0x200000;
  phdr->p_filesz = 0x0000000000000000;
  phdr->p_memsz = 0x0000000000007530;
  phdr->p_align = 0x0000000000200000;
  ptr += sizeof(Elf64_Phdr);
}


inline void
GeneratorElfX64::genFooter(void)
{
  static const char SHSTRTBL[] = "\0.text\0.shstrtbl\0.bss";
  unsigned char *ptr = codePtr;

  // section string table (22bytes)
  std::memcpy(ptr, SHSTRTBL, sizeof(SHSTRTBL));
  ptr += sizeof(SHSTRTBL);

  // section header
  // first section header
  Elf64_Shdr *shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 0;
  shdr->sh_type = SHT_NULL;
  shdr->sh_flags = 0x0000000000000000;
  shdr->sh_addr = 0x0000000000000000;
  shdr->sh_offset = 0x0000000000000000;
  shdr->sh_size = 0x0000000000000000;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000000;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // second section header (.shstrtbl)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 7;
  shdr->sh_type = SHT_STRTAB;
  shdr->sh_flags = 0x0000000000000000;
  shdr->sh_addr = 0x0000000000000000;
  shdr->sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2 + codeSize;
  shdr->sh_size = sizeof(SHSTRTBL);
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000001;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // third section header (.text)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 1;
  shdr->sh_type = SHT_PROGBITS;
  shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
  shdr->sh_addr = ADDR + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  shdr->sh_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * 2;
  shdr->sh_size = codeSize;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000004;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  // fourth section header (.bss)
  shdr = reinterpret_cast<Elf64_Shdr *>(ptr);
  shdr->sh_name = 17;
  shdr->sh_type = SHT_NOBITS;
  shdr->sh_flags = SHF_ALLOC | SHF_WRITE;
  shdr->sh_addr = ADDR + 0x200000;
  shdr->sh_offset = 0x0000000000001000;
  shdr->sh_size = 0x0000000000007530;
  shdr->sh_link = 0x00000000;
  shdr->sh_info = 0x00000000;
  shdr->sh_addralign = 0x0000000000000010;
  shdr->sh_entsize = 0x0000000000000000;
  ptr += sizeof(Elf64_Shdr);

  codePtr = ptr;
  binSize = codePtr - code;
}


inline void
GeneratorElfX64::genPtrAdd(unsigned int value)
{
  for (unsigned int i = 0; i < value; i++) {
    *codePtr++ = 0x48; *codePtr++ = 0xff; *codePtr++ = 0xc3;  // inc rbx
  }
}


inline void
GeneratorElfX64::genPtrSub(unsigned int value)
{
  for (unsigned int i = 0; i < value; i++) {
    *codePtr++ = 0x48; *codePtr++ = 0xff; *codePtr++ = 0xcb;  // dec rbx
  }
}


inline void
GeneratorElfX64::genAdd(unsigned int value)
{
  for (unsigned int i = 0; i < value; i++) {
    *codePtr++ = 0xfe; *codePtr++ = 0x03;  // inc byte ptr [rbx]
  }
}


inline void
GeneratorElfX64::genSub(unsigned int value)
{
  for (unsigned int i = 0; i < value; i++) {
    *codePtr++ = 0xfe; *codePtr++ = 0x0b;  // dec byte ptr [rbx]
  }
}


inline void
GeneratorElfX64::genPutchar(void)
{
  *codePtr++ = 0x48; *codePtr++ = 0xc7; *codePtr++ = 0xc0; *codePtr++ = 0x01; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov rax, 1 (set system call to write)
  *codePtr++ = 0xba; *codePtr++ = 0x01; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov edx, 0x01 (3rd argument)
  *codePtr++ = 0x48; *codePtr++ = 0x89; *codePtr++ = 0xde;  // mov rsi, rbx (2nd argument)
  *codePtr++ = 0xbf; *codePtr++ = 0x01; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov edi, 0x1 (1st argument)
  *codePtr++ = 0x0f; *codePtr++ = 0x05;  // syscall
}


inline void
GeneratorElfX64::genGetchar(void)
{
  *codePtr++ = 0x48; *codePtr++ = 0xc7; *codePtr++ = 0xc0; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov rax, 0 (set system call to write)
  *codePtr++ = 0xba; *codePtr++ = 0x01; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov edx, 0x01 (3rd argument)
  *codePtr++ = 0x48; *codePtr++ = 0x89; *codePtr++ = 0xde;  // mov rsi, rbx (2nd argument)
  *codePtr++ = 0xbf; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00; *codePtr++ = 0x00;  // mov edi, 0x0 (1st argument)
  *codePtr++ = 0x0f; *codePtr++ = 0x05;  // syscall
}


inline void
GeneratorElfX64::genLoopStart(void)
{
  loopStack.push(codePtr);
  *codePtr++ = 0x80; *codePtr++ = 0x3b; *codePtr++ = 0x00;  // cmp byte ptr [rbx], 0
  *codePtr++ = 0x0f; *codePtr++ = 0x84;
  *reinterpret_cast<uint32_t *>(codePtr) = 0x00000000; codePtr += sizeof(uint32_t);
}


inline void
GeneratorElfX64::genLoopEnd(void)
{
  unsigned char *_codePtr = loopStack.top();
  loopStack.pop();
  *codePtr++ = 0xe9;
  *reinterpret_cast<int32_t *>(codePtr) = -static_cast<int32_t>(codePtr + sizeof(int32_t) - _codePtr); codePtr += sizeof(int32_t);
  *reinterpret_cast<int32_t *>(_codePtr + 5) = static_cast<int32_t>(codePtr - (_codePtr + 9));
}


}  // namespace bf
#endif  // GENERATOR_ELF_X64
