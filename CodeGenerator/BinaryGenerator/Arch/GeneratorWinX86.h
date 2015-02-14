#ifndef GENERATOR_WIN_X86_H
#define GENERATOR_WIN_X86_H


#include "../BinaryGenerator.h"
#include "winsubset.h"


namespace bf {


class GeneratorWinX86 : public BinaryGenerator {
private:
  static const unsigned int PE_HEADER_SIZE = 0x200;
  static const unsigned int IDATA_SIZE = 0x200;
  static const unsigned int CODE_SIZE = 16 * 1024;
  static const unsigned int EXE_SIZE = PE_HEADER_SIZE + IDATA_SIZE + CODE_SIZE;
  static const unsigned int ADDR_PUTCHAR = 0x00405044;
  static const unsigned int ADDR_GETCHAR = ADDR_PUTCHAR + 4;
  static const unsigned int ADDR_BUF = 0x00406000;
  inline void genPlorogue(void);
  inline void genEpirogue(void);
  inline void genHeader(void);
  inline void genFooter(void);
  inline void genNextN(int value);
  inline void genPrevN(int value);
  inline void genAdd(int value);
  inline void genSub(int value);
  inline void genPutchar(void);
  inline void genGetchar(void);
  inline void genLoopStart(void);
  inline void genLoopEnd(void);
public:
  GeneratorWinX86(BfIR irCode, std::size_t codeSize=DEFAULT_MAX_CODE_SIZE) :
    BinaryGenerator(irCode) {}
};




inline void
GeneratorWinX86::genPlorogue(void)
{
  codePtr = code + PE_HEADER_SIZE + IDATA_SIZE;
  *codePtr++ = 0x31; *codePtr++ = 0xc9;  // xor ecx, ecx
  *codePtr++ = 0x57;  // push edi
  *codePtr++ = 0xbf; *reinterpret_cast<int *>(codePtr) = ADDR_BUF;  // mov edi, addr_buf
  codePtr += sizeof(int);
}


inline void
GeneratorWinX86::genEpirogue(void)
{
  *codePtr++ = 0x5f;  // pop edi
  *codePtr++ = 0x31; *codePtr++ = 0xc0;  // xor eax, eax
  *codePtr++ = 0xc3;  // ret
  codeSize = CODE_SIZE;
}


inline void
GeneratorWinX86::genHeader(void)
{
  static const unsigned char STUB[] = {
    // 00-3b: DOS Header
    'M', 'Z',   0x50, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0xff, 0xff, 0x00, 0x00,
    0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 3c-3f: Pointer to PE Header (=80)
    0x80, 0x00, 0x00, 0x00,
    // 40-7f: DOS Stub
    0xba, 0x10, 0x00, 0x0e, 0x1f, 0xb4, 0x09, 0xcd, 0x21, 0xb8, 0x01, 0x4c, 0xcd, 0x21, 0x90, 0x90,
    'T',  'h',  'i',  's',  ' ',  'p',  'r',  'o',  'g',  'r',  'a',  'm',  ' ',  'c',  'a',  'n',
    'n',  'o',  't',  ' ',  'b',  'e',  ' ',  'r',  'u',  'n',  ' ',  'i',  'n',  ' ',  'D',  'O',
    'S',  ' ',  'm',  'o',  'd',  'e',  '.',  '\r', '\n', '$',  0,    0,    0,    0,    0,    0,
    // 80-83: PE Signature
    'P',  'E',  0,    0
  };
  static const char CRT_DLL[] = "msvcrt.dll\0\0\0\0\0";
  static const char STR_PUTCHAR[] = "putchar";
  static const char STR_GETCHAR[] = "getchar";

  unsigned char *ptr = code;
  std::memcpy(ptr, STUB, sizeof(STUB));
  ptr += sizeof(STUB);

  IMAGE_FILE_HEADER *ifh = reinterpret_cast<IMAGE_FILE_HEADER *>(ptr);
  ifh->Machine = 0x014c;
  ifh->NumberOfSections = 3;
  ifh->TimeDateStamp = 0;
  ifh->PointerToSymbolTable = 0;
  ifh->NumberOfSymbols = 0;
  ifh->SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
  ifh->Characteristics = 0x030f;
  ptr += sizeof(IMAGE_FILE_HEADER);

  IMAGE_OPTIONAL_HEADER32 *ioh = reinterpret_cast<IMAGE_OPTIONAL_HEADER32 *>(ptr);
  ioh->Magic = 0x010b;
  ioh->MajorLinkerVersion = 6;
  ioh->MinorLinkerVersion = 0;
  ioh->SizeOfCode = static_cast<DWORD>(codeSize);
  ioh->SizeOfInitializedData = 0;
  ioh->SizeOfUninitializedData = 65536;
  ioh->AddressOfEntryPoint = 0x1000;
  ioh->BaseOfCode = 0x1000;
  ioh->BaseOfData = 0x6000;
  ioh->ImageBase = 0x00400000;
  ioh->SectionAlignment = 0x1000;
  ioh->FileAlignment = 0x0200;
  ioh->MajorOperatingSystemVersion = 4;
  ioh->MinorOperatingSystemVersion = 0;
  ioh->MajorImageVersion = 0;
  ioh->MinorImageVersion = 0;
  ioh->MajorSubsystemVersion = 4;
  ioh->MinorSubsystemVersion = 0;
  ioh->Win32VersionValue = 0;
  ioh->SizeOfImage = 0x16000;
  ioh->SizeOfHeaders = PE_HEADER_SIZE;
  ioh->CheckSum = 0;
  ioh->Subsystem = 3;
  ioh->DllCharacteristics = 0;
  ioh->SizeOfStackReserve = 1024 * 1024;
  ioh->SizeOfStackCommit = 8 * 1024;
  ioh->SizeOfHeapReserve = 1024 * 1024;
  ioh->SizeOfHeapCommit = 4 * 1024;
  ioh->LoaderFlags = 0;
  ioh->NumberOfRvaAndSizes = 16;
  ioh->DataDirectory[1].VirtualAddress = 0x5000;  // import table
  ioh->DataDirectory[1].Size = 100;
  ptr += sizeof(IMAGE_OPTIONAL_HEADER32);

  // .text section
  IMAGE_SECTION_HEADER *ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".text");
  ish->Misc.VirtualSize = static_cast<DWORD>(codeSize);
  ish->VirtualAddress = 0x1000;
  ish->SizeOfRawData = static_cast<DWORD>(codeSize);
  ish->PointerToRawData = PE_HEADER_SIZE + IDATA_SIZE;
  ish->Characteristics = 0x60500060;
  ptr += sizeof(IMAGE_SECTION_HEADER);

  // .idata section
  ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".idata");
  ish->Misc.VirtualSize = 100;
  ish->VirtualAddress = 0x5000;
  ish->SizeOfRawData = 512;
  ish->PointerToRawData = PE_HEADER_SIZE;
  ish->Characteristics = 0xc0300040;
  ptr += sizeof(IMAGE_SECTION_HEADER);

  // .bss section
  ish = reinterpret_cast<IMAGE_SECTION_HEADER *>(ptr);
  std::strcpy(reinterpret_cast<char *>(ish->Name), ".bss");
  ish->Misc.VirtualSize = 65536;
  ish->VirtualAddress = 0x6000;
  ish->Characteristics = 0xc0400080;
  ptr = code + PE_HEADER_SIZE;

  int *idt = reinterpret_cast<int *>(ptr);
  // IDT 1
  idt[0] = 0x5028;
  idt[1] = 0;
  idt[2] = 0;
  idt[3] = 0x5034;
  idt[4] = 0x5044;
  // IDT (End)
  idt[5] = 0;
  idt[6] = 0;
  idt[7] = 0;
  idt[8] = 0;
  idt[9] = 0;
  ptr += sizeof(int) * 10;

  int *ilt = reinterpret_cast<int *>(ptr);
  ilt[0] = 0x5050;
  ilt[1] = 0x505a;
  ilt[2] = 0;
  ptr += sizeof(int) * 3;

  std::memcpy(ptr, CRT_DLL, sizeof(CRT_DLL));
  ptr += sizeof(CRT_DLL);

  int *iat = reinterpret_cast<int *>(ptr);
  iat[0] = 0x5050;
  iat[1] = 0x505a;
  iat[2] = 0;
  ptr += sizeof(int) * 3;

  *reinterpret_cast<short *>(ptr) = 0x0000;
  ptr += sizeof(short);
  std::memcpy(ptr, STR_PUTCHAR, sizeof(STR_PUTCHAR));
  ptr += sizeof(STR_PUTCHAR);

  *reinterpret_cast<short *>(ptr) = 0x0000;
  ptr += sizeof(short);
  std::memcpy(ptr, STR_GETCHAR, sizeof(STR_GETCHAR));
  ptr += sizeof(STR_GETCHAR);
}


inline void
GeneratorWinX86::genFooter(void)
{
  binSize = EXE_SIZE;
}


inline void
GeneratorWinX86::genNextN(int value)
{
  for (int i = 0; i < value; i++) {
    *codePtr++ = 0x66; *codePtr++ = 0x41;  // inc cx
  }
}


inline void
GeneratorWinX86::genPrevN(int value)
{
  for (int i = 0; i < value; i++) {
    *codePtr++ = 0x66; *codePtr++ = 0x49;  // dec cx
  }
}


inline void
GeneratorWinX86::genAdd(int value)
{
  for (int i = 0; i < value; i++) {
    *codePtr++ = 0xfe; *codePtr++ = 0x04; *codePtr++ = 0x0f;  // inc byte [edi+ecx]
  }
}


inline void
GeneratorWinX86::genSub(int value)
{
  for (int i = 0; i < value; i++) {
    *codePtr++ = 0xfe; *codePtr++ = 0x0c; *codePtr++ = 0x0f;  // dec byte [edi+ecx]
  }
}


inline void
GeneratorWinX86::genPutchar(void)
{
  *codePtr++ = 0x51;  // push ecx
  *reinterpret_cast<int *>(codePtr) = 0x0f04b60f;  // movzx eax,byte [edi+ecx]
  codePtr += sizeof(int);

  *codePtr++ = 0x50;  // push eax
  *codePtr++ = 0xa1; *reinterpret_cast<int *>(codePtr) = ADDR_PUTCHAR;  // mov eax, [addr_putchar]
  codePtr += sizeof(int);

  *codePtr++ = 0xff; *codePtr++ = 0xd0;  // call eax
  *codePtr++ = 0x58;  // pop eax
  *codePtr++ = 0x59;  // pop ecx
}


inline void
GeneratorWinX86::genGetchar(void)
{
  *codePtr++ = 0x51;  // push ecx
  *codePtr++ = 0xa1; *reinterpret_cast<int *>(codePtr) = ADDR_GETCHAR;  // mov eax, [addr_getchar]
  codePtr += sizeof(int);

  *codePtr++ = 0xff; *codePtr++ = 0xd0;  // call eax
  *codePtr++ = 0x59;  // pop ecx
  *codePtr++ = 0x88; *codePtr++ = 0x04; *codePtr++ = 0x0f;  // mov [edi+ecx],al
}


inline void
GeneratorWinX86::genLoopStart(void)
{
  loopStack.push(codePtr);
  *reinterpret_cast<int *>(codePtr) = 0x000f3c80;  // cmp byte [edi+ecx], 0
  codePtr += sizeof(int);
  // jz Jump to just behind the corresponding ] (define address later)
  *codePtr++ = 0x0f; *codePtr++ = 0x84;
  *reinterpret_cast<int *>(codePtr) = 0x00000000;  // cmp byte [edi+ecx], 0
  codePtr += sizeof(int);
}


inline void
GeneratorWinX86::genLoopEnd(void)
{
  unsigned char *_codePtr = loopStack.top();
  loopStack.pop();
  *codePtr++ = 0xe9; *reinterpret_cast<int *>(codePtr) = _codePtr - (codePtr + sizeof(int));  // jmp idxLoop
  codePtr += sizeof(int);
  *reinterpret_cast<int *>(_codePtr + 6) = codePtr - (_codePtr + 10);  // Rewrites the top of the loop
}


}  // namespace bf
#endif  // GENERATOR_WIN_X86_H
