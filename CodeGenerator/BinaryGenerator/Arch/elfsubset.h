/*!
 * @file elfsubset.h
 * @brief Subset of <elf.h> (<elf32.h> and <elf64.h>)
 * @author koturn
 */
#ifndef ELFSUBSET_H
#define ELFSUBSET_H




#if !defined(__linux__) && !defined(__CYGWIN__)
#if __cplusplus >= 201103L
#  include <cstdint>
#else
#  include <stdint.h>
#endif
/*
 * Indexes into the e_ident array.  Keep synced with
 * http://www.sco.com/developers/gabi/latest/ch4.eheader.html
 */
#define EI_MAG0         0   /* Magic number, byte 0. */
#define EI_MAG1         1   /* Magic number, byte 1. */
#define EI_MAG2         2   /* Magic number, byte 2. */
#define EI_MAG3         3   /* Magic number, byte 3. */
#define EI_CLASS        4   /* Class of machine. */
#define EI_DATA         5   /* Data format. */
#define EI_VERSION      6   /* ELF format version. */
#define EI_OSABI        7   /* Operating system / ABI identification */
#define EI_ABIVERSION   8   /* ABI version */
#define OLD_EI_BRAND    8   /* Start of architecture identification. */
#define EI_PAD          9   /* Start of padding (per SVR4 ABI). */
#define EI_NIDENT       16  /* Size of e_ident array. */

/* Values for the magic number bytes. */
#define ELFMAG0  0x7f
#define ELFMAG1  'E'
#define ELFMAG2  'L'
#define ELFMAG3  'F'
#define ELFMAG   "\177ELF"  /* magic string */
#define SELFMAG   4         /* magic string size */

/* Values for e_ident[EI_VERSION] and e_version. */
#define EV_NONE     0
#define EV_CURRENT  1

/* Values for e_ident[EI_CLASS]. */
#define ELFCLASSNONE  0  /* Unknown class. */
#define ELFCLASS32    1  /* 32-bit architecture. */
#define ELFCLASS64    2  /* 64-bit architecture. */

/* Values for e_ident[EI_DATA]. */
#define ELFDATANONE  0  /* Unknown data format. */
#define ELFDATA2LSB  1  /* 2's complement little-endian. */
#define ELFDATA2MSB  2  /* 2's complement big-endian. */

/* Values for e_ident[EI_OSABI]. */
#define ELFOSABI_NONE        0    /* UNIX System V ABI */
#define ELFOSABI_HPUX        1    /* HP-UX operating system */
#define ELFOSABI_NETBSD      2    /* NetBSD */
#define ELFOSABI_LINUX       3    /* GNU/Linux */
#define ELFOSABI_HURD        4    /* GNU/Hurd */
#define ELFOSABI_86OPEN      5    /* 86Open common IA32 ABI */
#define ELFOSABI_SOLARIS     6    /* Solaris */
#define ELFOSABI_AIX         7    /* AIX */
#define ELFOSABI_IRIX        8    /* IRIX */
#define ELFOSABI_FREEBSD     9    /* FreeBSD */
#define ELFOSABI_TRU64       10   /* TRU64 UNIX */
#define ELFOSABI_MODESTO     11   /* Novell Modesto */
#define ELFOSABI_OPENBSD     12   /* OpenBSD */
#define ELFOSABI_OPENVMS     13   /* Open VMS */
#define ELFOSABI_NSK         14   /* HP Non-Stop Kernel */
#define ELFOSABI_AROS        15   /* Amiga Research OS */
#define ELFOSABI_ARM         97   /* ARM */
#define ELFOSABI_STANDALONE  255  /* Standalone (embedded) application */

#define ELFOSABI_SYSV      ELFOSABI_NONE  /* symbol used in old spec */
#define ELFOSABI_MONTEREY  ELFOSABI_AIX   /* Monterey */

/* e_ident */
#define IS_ELF(ehdr) \
  ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
   (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
   (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
   (ehdr).e_ident[EI_MAG3] == ELFMAG3)

/* Values for e_type. */
#define ET_NONE    0       /* Unknown type. */
#define ET_REL     1       /* Relocatable. */
#define ET_EXEC    2       /* Executable. */
#define ET_DYN     3       /* Shared object. */
#define ET_CORE    4       /* Core file. */
#define ET_LOOS    0xfe00  /* First operating system specific. */
#define ET_HIOS    0xfeff  /* Last operating system-specific. */
#define ET_LOPROC  0xff00  /* First processor-specific. */
#define ET_HIPROC  0xffff  /* Last processor-specific. */

/* Values for e_machine. */
#define EM_NONE         0    /* Unknown machine. */
#define EM_M32          1    /* AT&T WE32100. */
#define EM_SPARC        2    /* Sun SPARC. */
#define EM_386          3    /* Intel i386. */
#define EM_68K          4    /* Motorola 68000. */
#define EM_88K          5    /* Motorola 88000. */
#define EM_860          7    /* Intel i860. */
#define EM_MIPS         8    /* MIPS R3000 Big-Endian only. */
#define EM_S370         9    /* IBM System/370. */
#define EM_MIPS_RS3_LE  10   /* MIPS R3000 Little-Endian. */
#define EM_PARISC       15   /* HP PA-RISC. */
#define EM_VPP500       17   /* Fujitsu VPP500. */
#define EM_SPARC32PLUS  18   /* SPARC v8plus. */
#define EM_960          19   /* Intel 80960. */
#define EM_PPC          20   /* PowerPC 32-bit. */
#define EM_PPC64        21   /* PowerPC 64-bit. */
#define EM_S390         22   /* IBM System/390. */
#define EM_V800         36   /* NEC V800. */
#define EM_FR20         37   /* Fujitsu FR20. */
#define EM_RH32         38   /* TRW RH-32. */
#define EM_RCE          39   /* Motorola RCE. */
#define EM_ARM          40   /* ARM. */
#define EM_SH           42   /* Hitachi SH. */
#define EM_SPARCV9      43   /* SPARC v9 64-bit. */
#define EM_TRICORE      44   /* Siemens TriCore embedded processor. */
#define EM_ARC          45   /* Argonaut RISC Core. */
#define EM_H8_300       46   /* Hitachi H8/300. */
#define EM_H8_300H      47   /* Hitachi H8/300H. */
#define EM_H8S          48   /* Hitachi H8S. */
#define EM_H8_500       49   /* Hitachi H8/500. */
#define EM_IA_64        50   /* Intel IA-64 Processor. */
#define EM_MIPS_X       51   /* Stanford MIPS-X. */
#define EM_COLDFIRE     52   /* Motorola ColdFire. */
#define EM_68HC12       53   /* Motorola M68HC12. */
#define EM_MMA          54   /* Fujitsu MMA. */
#define EM_PCP          55   /* Siemens PCP. */
#define EM_NCPU         56   /* Sony nCPU. */
#define EM_NDR1         57   /* Denso NDR1 microprocessor. */
#define EM_STARCORE     58   /* Motorola Star*Core processor. */
#define EM_ME16         59   /* Toyota ME16 processor. */
#define EM_ST100        60   /* STMicroelectronics ST100 processor. */
#define EM_TINYJ        61   /* Advanced Logic Corp. TinyJ processor. */
#define EM_X86_64       62   /* Advanced Micro Devices x86-64 */
#define EM_AMD64        EM_X86_64  /* Advanced Micro Devices x86-64 (compat) */
#define EM_PDSP         63    /* Sony DSP Processor. */
#define EM_FX66         66    /* Siemens FX66 microcontroller. */
#define EM_ST9PLUS      67    /* STMicroelectronics ST9+ 8/16 microcontroller. */
#define EM_ST7          68    /* STmicroelectronics ST7 8-bit microcontroller. */
#define EM_68HC16       69    /* Motorola MC68HC16 microcontroller. */
#define EM_68HC11       70    /* Motorola MC68HC11 microcontroller. */
#define EM_68HC08       71    /* Motorola MC68HC08 microcontroller. */
#define EM_68HC05       72    /* Motorola MC68HC05 microcontroller. */
#define EM_SVX          73    /* Silicon Graphics SVx. */
#define EM_ST19         74    /* STMicroelectronics ST19 8-bit mc. */
#define EM_VAX          75    /* Digital VAX. */
#define EM_CRIS         76    /* Axis Communications 32-bit embedded processor. */
#define EM_JAVELIN      77    /* Infineon Technologies 32-bit embedded processor. */
#define EM_FIREPATH     78    /* Element 14 64-bit DSP Processor. */
#define EM_ZSP          79    /* LSI Logic 16-bit DSP Processor. */
#define EM_MMIX         80    /* Donald Knuth's educational 64-bit proc. */
#define EM_HUANY        81    /* Harvard University machine-independent object files. */
#define EM_PRISM        82    /* SiTera Prism. */
#define EM_AVR          83    /* Atmel AVR 8-bit microcontroller. */
#define EM_FR30         84    /* Fujitsu FR30. */
#define EM_D10V         85    /* Mitsubishi D10V. */
#define EM_D30V         86    /* Mitsubishi D30V. */
#define EM_V850         87    /* NEC v850. */
#define EM_M32R         88    /* Mitsubishi M32R. */
#define EM_MN10300      89    /* Matsushita MN10300. */
#define EM_MN10200      90    /* Matsushita MN10200. */
#define EM_PJ           91    /* picoJava. */
#define EM_OPENRISC     92    /* OpenRISC 32-bit embedded processor. */
#define EM_ARC_A5       93    /* ARC Cores Tangent-A5. */
#define EM_XTENSA       94    /* Tensilica Xtensa Architecture. */
#define EM_VIDEOCORE    95    /* Alphamosaic VideoCore processor. */
#define EM_TMM_GPP      96    /* Thompson Multimedia General Purpose Processor. */
#define EM_NS32K        97    /* National Semiconductor 32000 series. */
#define EM_TPC          98    /* Tenor Network TPC processor. */
#define EM_SNP1K        99    /* Trebia SNP 1000 processor. */
#define EM_ST200        100   /* STMicroelectronics ST200 microcontroller. */
#define EM_IP2K         101   /* Ubicom IP2xxx microcontroller family. */
#define EM_MAX          102   /* MAX Processor. */
#define EM_CR           103   /* National Semiconductor CompactRISC microprocessor. */
#define EM_F2MC16       104   /* Fujitsu F2MC16. */
#define EM_MSP430       105   /* Texas Instruments embedded microcontroller msp430. */
#define EM_BLACKFIN     106   /* Analog Devices Blackfin (DSP) processor. */
#define EM_SE_C33       107   /* S1C33 Family of Seiko Epson processors. */
#define EM_SEP          108   /* Sharp embedded microprocessor. */
#define EM_ARCA         109   /* Arca RISC Microprocessor. */
#define EM_UNICORE      110   /* Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University */

/* Non-standard or deprecated. */
#define EM_486          6       /* Intel i486. */
#define EM_MIPS_RS4_BE  10      /* MIPS R4000 Big-Endian */
#define EM_ALPHA_STD    41      /* Digital Alpha (standard value). */
#define EM_ALPHA        0x9026  /* Alpha (written in the absence of an ABI) */

/* Special section indexes. */
#define SHN_UNDEF      0       /* Undefined, missing, irrelevant. */
#define SHN_LORESERVE  0xff00  /* First of reserved range. */
#define SHN_LOPROC     0xff00  /* First processor-specific. */
#define SHN_HIPROC     0xff1f  /* Last processor-specific. */
#define SHN_LOOS       0xff20  /* First operating system-specific. */
#define SHN_HIOS       0xff3f  /* Last operating system-specific. */
#define SHN_ABS        0xfff1  /* Absolute values. */
#define SHN_COMMON     0xfff2  /* Common data. */
#define SHN_XINDEX     0xffff  /* Escape -- index stored elsewhere. */
#define SHN_HIRESERVE  0xffff  /* Last of reserved range. */

/* sh_type */
#define SHT_NULL            0   /* inactive */
#define SHT_PROGBITS        1   /* program defined information */
#define SHT_SYMTAB          2   /* symbol table section */
#define SHT_STRTAB          3   /* string table section */
#define SHT_RELA            4   /* relocation section with addends */
#define SHT_HASH            5   /* symbol hash table section */
#define SHT_DYNAMIC         6   /* dynamic section */
#define SHT_NOTE            7   /* note section */
#define SHT_NOBITS          8   /* no space section */
#define SHT_REL             9   /* relocation section - no addends */
#define SHT_SHLIB           10  /* reserved - purpose unknown */
#define SHT_DYNSYM          11  /* dynamic symbol table section */
#define SHT_INIT_ARRAY      14  /* Initialization function pointers. */
#define SHT_FINI_ARRAY      15  /* Termination function pointers. */
#define SHT_PREINIT_ARRAY   16  /* Pre-initialization function ptrs. */
#define SHT_GROUP           17  /* Section group. */
#define SHT_SYMTAB_SHNDX    18  /* Section indexes (see SHN_XINDEX). */
#define SHT_LOOS            0x60000000  /* First of OS specific semantics */
#define SHT_LOSUNW          0x6ffffff4
#define SHT_SUNW_dof        0x6ffffff4
#define SHT_SUNW_cap        0x6ffffff5
#define SHT_SUNW_SIGNATURE  0x6ffffff6
#define SHT_GNU_HASH        0x6ffffff6
#define SHT_SUNW_ANNOTATE   0x6ffffff7
#define SHT_SUNW_DEBUGSTR   0x6ffffff8
#define SHT_SUNW_DEBUG      0x6ffffff9
#define SHT_SUNW_move       0x6ffffffa
#define SHT_SUNW_COMDAT     0x6ffffffb
#define SHT_SUNW_syminfo    0x6ffffffc
#define SHT_SUNW_verdef     0x6ffffffd
#define SHT_GNU_verdef      0x6ffffffd  /* Symbol versions provided */
#define SHT_SUNW_verneed    0x6ffffffe
#define SHT_GNU_verneed     0x6ffffffe  /* Symbol versions required */
#define SHT_SUNW_versym     0x6fffffff
#define SHT_GNU_versym      0x6fffffff  /* Symbol version table */
#define SHT_HISUNW          0x6fffffff
#define SHT_HIOS            0x6fffffff  /* Last of OS specific semantics */
#define SHT_LOPROC          0x70000000  /* reserved range for processor */
#define SHT_AMD64_UNWIND    0x70000001  /* unwind information */
#define SHT_MIPS_REGINFO    0x70000006
#define SHT_MIPS_OPTIONS    0x7000000d
#define SHT_MIPS_DWARF      0x7000001e  /* MIPS gcc uses MIPS_DWARF */
#define SHT_HIPROC          0x7fffffff  /* specific section header types */
#define SHT_LOUSER          0x80000000  /* reserved range for application */
#define SHT_HIUSER          0xffffffff  /* specific indexes */

/* Flags for sh_flags. */
#define SHF_WRITE             0x1         /* Section contains writable data. */
#define SHF_ALLOC             0x2         /* Section occupies memory. */
#define SHF_EXECINSTR         0x4         /* Section contains instructions. */
#define SHF_MERGE             0x10        /* Section may be merged. */
#define SHF_STRINGS           0x20        /* Section contains strings. */
#define SHF_INFO_LINK         0x40        /* sh_info holds section index. */
#define SHF_LINK_ORDER        0x80        /* Special ordering requirements. */
#define SHF_OS_NONCONFORMING  0x100       /* OS-specific processing required. */
#define SHF_GROUP             0x200       /* Member of section group. */
#define SHF_TLS               0x400       /* Section contains TLS data. */
#define SHF_MASKOS            0x0ff00000  /* OS-specific semantics. */
#define SHF_MASKPROC          0xf0000000  /* Processor-specific semantics. */

/* Values for p_type. */
#define PT_NULL          0  /* Unused entry. */
#define PT_LOAD          1  /* Loadable segment. */
#define PT_DYNAMIC       2  /* Dynamic linking information segment. */
#define PT_INTERP        3  /* Pathname of interpreter. */
#define PT_NOTE          4  /* Auxiliary information. */
#define PT_SHLIB         5  /* Reserved (not used). */
#define PT_PHDR          6  /* Location of program header itself. */
#define PT_TLS           7  /* Thread local storage segment */
#define PT_LOOS          0x60000000  /* First OS-specific. */
#define PT_SUNW_UNWIND   0x6464e550  /* amd64 UNWIND program header */
#define PT_GNU_EH_FRAME  0x6474e550
#define PT_GNU_STACK     0x6474e551
#define PT_GNU_RELRO     0x6474e552
#define PT_LOSUNW        0x6ffffffa
#define PT_SUNWBSS       0x6ffffffa  /* Sun Specific segment */
#define PT_SUNWSTACK     0x6ffffffb  /* describes the stack segment */
#define PT_SUNWDTRACE    0x6ffffffc  /* private */
#define PT_SUNWCAP       0x6ffffffd  /* hard/soft capabilities segment */
#define PT_HISUNW        0x6fffffff
#define PT_HIOS          0x6fffffff  /* Last OS-specific. */
#define PT_LOPROC        0x70000000  /* First processor-specific type. */
#define PT_HIPROC        0x7fffffff  /* Last processor-specific type. */

/* Values for p_flags. */
#define PF_X         0x1  /* Executable. */
#define PF_W         0x2  /* Writable. */
#define PF_R         0x4  /* Readable. */
#define PF_MASKOS    0x0ff00000  /* Operating system-specific. */
#define PF_MASKPROC  0xf0000000  /* Processor-specific. */
#endif  // !defined(__linux__) && !defined(__CYGWIN__)




// <elf32.h>
#if defined(__linux__)
#  include <elf.h>
#elif defined(__CYGWIN__)
#  include <sys/elf32.h>
#else
/*
 * ELF definitions common to all 32-bit architectures.
 */
typedef uint32_t  Elf32_Addr;
typedef uint16_t  Elf32_Half;
typedef uint32_t  Elf32_Off;
typedef uint32_t  Elf32_Word;

/*
 * ELF header.
 */
typedef struct {
  unsigned char e_ident[EI_NIDENT];  /* File identification. */
  Elf32_Half e_type;       /* File type. */
  Elf32_Half e_machine;    /* Machine architecture. */
  Elf32_Word e_version;    /* ELF format version. */
  Elf32_Addr e_entry;      /* Entry point. */
  Elf32_Off  e_phoff;      /* Program header file offset. */
  Elf32_Off  e_shoff;      /* Section header file offset. */
  Elf32_Word e_flags;      /* Architecture-specific flags. */
  Elf32_Half e_ehsize;     /* Size of ELF header in bytes. */
  Elf32_Half e_phentsize;  /* Size of program header entry. */
  Elf32_Half e_phnum;      /* Number of program header entries. */
  Elf32_Half e_shentsize;  /* Size of section header entry. */
  Elf32_Half e_shnum;      /* Number of section header entries. */
  Elf32_Half e_shstrndx;   /* Section name strings section. */
} Elf32_Ehdr;

/*
 * Section header.
 */
typedef struct {
  Elf32_Word sh_name;       /* Section name (index into the section header string table). */
  Elf32_Word sh_type;       /* Section type. */
  Elf32_Word sh_flags;      /* Section flags. */
  Elf32_Addr sh_addr;       /* Address in memory image. */
  Elf32_Off  sh_offset;     /* Offset in file. */
  Elf32_Word sh_size;       /* Size in bytes. */
  Elf32_Word sh_link;       /* Index of a related section. */
  Elf32_Word sh_info;       /* Depends on section type. */
  Elf32_Word sh_addralign;  /* Alignment in bytes. */
  Elf32_Word sh_entsize;    /* Size of each entry in section. */
} Elf32_Shdr;

/*
 * Program header.
 */
typedef struct {
  Elf32_Word p_type;    /* Entry type. */
  Elf32_Off  p_offset;  /* File offset of contents. */
  Elf32_Addr p_vaddr;   /* Virtual address in memory image. */
  Elf32_Addr p_paddr;   /* Physical address (not used). */
  Elf32_Word p_filesz;  /* Size of contents in file. */
  Elf32_Word p_memsz;   /* Size of contents in memory. */
  Elf32_Word p_flags;   /* Access permission flags. */
  Elf32_Word p_align;   /* Alignment in memory and file. */
} Elf32_Phdr;
#endif  // defined(__linux__)




// <elf64.h>
#if defined(__linux__)
#  include <elf.h>
#elif defined(__CYGWIN__)
#  include <sys/elf64.h>
#else
/*
 * ELF definitions common to all 64-bit architectures.
 */
typedef uint64_t  Elf64_Addr;
typedef uint16_t  Elf64_Half;
typedef uint64_t  Elf64_Off;
typedef uint32_t  Elf64_Word;
typedef uint64_t  Elf64_Xword;

/*
 * ELF header.
 */
typedef struct {
  unsigned char e_ident[EI_NIDENT];  /* File identification. */
  Elf64_Half e_type;       /* File type. */
  Elf64_Half e_machine;    /* Machine architecture. */
  Elf64_Word e_version;    /* ELF format version. */
  Elf64_Addr e_entry;      /* Entry point. */
  Elf64_Off  e_phoff;      /* Program header file offset. */
  Elf64_Off  e_shoff;      /* Section header file offset. */
  Elf64_Word e_flags;      /* Architecture-specific flags. */
  Elf64_Half e_ehsize;     /* Size of ELF header in bytes. */
  Elf64_Half e_phentsize;  /* Size of program header entry. */
  Elf64_Half e_phnum;      /* Number of program header entries. */
  Elf64_Half e_shentsize;  /* Size of section header entry. */
  Elf64_Half e_shnum;      /* Number of section header entries. */
  Elf64_Half e_shstrndx;   /* Section name strings section. */
} Elf64_Ehdr;

/*
 * Section header.
 */
typedef struct {
  Elf64_Word  sh_name;       /* Section name (index into the section header string table). */
  Elf64_Word  sh_type;       /* Section type. */
  Elf64_Xword sh_flags;      /* Section flags. */
  Elf64_Addr  sh_addr;       /* Address in memory image. */
  Elf64_Off   sh_offset;     /* Offset in file. */
  Elf64_Xword sh_size;       /* Size in bytes. */
  Elf64_Word  sh_link;       /* Index of a related section. */
  Elf64_Word  sh_info;       /* Depends on section type. */
  Elf64_Xword sh_addralign;  /* Alignment in bytes. */
  Elf64_Xword sh_entsize;    /* Size of each entry in section. */
} Elf64_Shdr;

/*
 * Program header.
 */
typedef struct {
  Elf64_Word  p_type;    /* Entry type. */
  Elf64_Word  p_flags;   /* Access permission flags. */
  Elf64_Off   p_offset;  /* File offset of contents. */
  Elf64_Addr  p_vaddr;   /* Virtual address in memory image. */
  Elf64_Addr  p_paddr;   /* Physical address (not used). */
  Elf64_Xword p_filesz;  /* Size of contents in file. */
  Elf64_Xword p_memsz;   /* Size of contents in memory. */
  Elf64_Xword p_align;   /* Alignment in memory and file. */
} Elf64_Phdr;
#endif  // defined(__linux__)


#endif  // ELFSUBSET_H
