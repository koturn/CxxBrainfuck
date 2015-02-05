#ifndef WINSUBSET_H
#define WINSUBSET_H


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 
#    define WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#  endif
#  ifndef NOGDICAPMASKS
#    define NOGDICAPMASKS
#    define WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#  endif
#  ifndef NOVIRTUALKEYCODES
#    define NOVIRTUALKEYCODES
#    define WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#  endif
#  ifndef NOWINMESSAGES
#    define NOWINMESSAGES
#    define WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#  endif
#  ifndef NOWINSTYLES
#    define NOWINSTYLES
#    define WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#  endif
#  ifndef NOSYSMETRICS
#    define NOSYSMETRICS
#    define WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#  endif
#  ifndef NOMENUS
#    define NOMENUS
#    define WINSUBSET_NOMENUS_IS_NOT_DEFINED
#  endif
#  ifndef NOICONS
#    define NOICONS
#    define WINSUBSET_NOICONS_IS_NOT_DEFINED
#  endif
#  ifndef NOKEYSTATES
#    define NOKEYSTATES
#    define WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#  endif
#  ifndef NOSYSCOMMANDS
#    define NOSYSCOMMANDS
#    define WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#  endif
#  ifndef NORASTEROPS
#    define NORASTEROPS
#    define WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#  endif
#  ifndef NOSHOWWINDOW
#    define NOSHOWWINDOW
#    define WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#  endif
#  ifndef OEMRESOURCE
#    define OEMRESOURCE
#    define WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#  endif
#  ifndef NOATOM
#    define NOATOM
#    define WINSUBSET_NOATOM_IS_NOT_DEFINED
#  endif
#  ifndef NOCLIPBOARD
#    define NOCLIPBOARD
#    define WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#  endif
#  ifndef NOCOLOR
#    define NOCOLOR
#    define WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#  endif
#  ifndef NOCTLMGR
#    define NOCTLMGR
#    define WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#  endif
#  ifndef NODRAWTEXT
#    define NODRAWTEXT
#    define WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#  endif
#  ifndef NOGDI
#    define NOGDI
#    define WINSUBSET_NOGDI_IS_NOT_DEFINED
#  endif
#  ifndef NOKERNEL
#    define NOKERNEL
#    define WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#  endif
#  ifndef NOUSER
#    define NOUSER
#    define WINSUBSET_NOUSER_IS_NOT_DEFINED
#  endif
#  ifndef NONLS
#    define NONLS
#    define WINSUBSET_NONLS_IS_NOT_DEFINED
#  endif
#  ifndef NOMB
#    define NOMB
#    define WINSUBSET_NOMB_IS_NOT_DEFINED
#  endif
#  ifndef NOMEMMGR
#    define NOMEMMGR
#    define WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#  endif
#  ifndef NOMETAFILE
#    define NOMETAFILE
#    define WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#    define WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#  endif
#  ifndef NOMSG
#    define NOMSG
#    define WINSUBSET_NOMSG_IS_NOT_DEFINED
#  endif
#  ifndef NOOPENFILE
#    define NOOPENFILE
#    define WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#  endif
#  ifndef NOSCROLL
#    define NOSCROLL
#    define WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#  endif
#  ifndef NOSERVICE
#    define NOSERVICE
#    define WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#  endif
#  ifndef NOSOUND
#    define NOSOUND
#    define WINSUBSET_NOSOUND_IS_NOT_DEFINED
#  endif
#  ifndef NOTEXTMETRIC
#    define NOTEXTMETRIC
#    define WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#  endif
#  ifndef NOWH
#    define NOWH
#    define WINSUBSET_NOWH_IS_NOT_DEFINED
#  endif
#  ifndef NOWINOFFSETS
#    define NOWINOFFSETS
#    define WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#  endif
#  ifndef NOCOMM
#    define NOCOMM
#    define WINSUBSET_NOCOMM_IS_NOT_DEFINED
#  endif
#  ifndef NOKANJI
#    define NOKANJI
#    define WINSUBSET_NOKANJI_IS_NOT_DEFINED
#  endif
#  ifndef NOHELP
#    define NOHELP
#    define WINSUBSET_NOHELP_IS_NOT_DEFINED
#  endif
#  ifndef NOPROFILER
#    define NOPROFILER
#    define WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#  endif
#  ifndef NODEFERWINDOWPOS
#    define NODEFERWINDOWPOS
#    define WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#  endif
#  ifndef NOMCX
#    define NOMCX
#    define WINSUBSET_NOMCX_IS_NOT_DEFINED
#  endif

#  include <windows.h>

#  ifdef WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WIN32_LEAN_AND_MEAN
#  endif
#  ifdef WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#    undef WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#    undef NOGDICAPMASKS
#  endif
#  ifdef WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#    undef WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#    undef NOVIRTUALKEYCODES
#  endif
#  ifdef WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#    undef NOWINMESSAGES
#  endif
#  ifdef WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#    undef NOWINSTYLES
#  endif
#  ifdef WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#    undef WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#    undef NOSYSMETRICS
#  endif
#  ifdef WINSUBSET_NOMENUS_IS_NOT_DEFINED
#    undef WINSUBSET_NOMENUS_IS_NOT_DEFINED
#    undef NOMENUS
#  endif
#  ifdef WINSUBSET_NOICONS_IS_NOT_DEFINED
#    undef WINSUBSET_NOICONS_IS_NOT_DEFINED
#    undef NOICONS
#  endif
#  ifdef WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#    undef WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#    undef NOKEYSTATES
#  endif
#  ifdef WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#    undef WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#    undef NOSYSCOMMANDS
#  endif
#  ifdef WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#    undef WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#    undef NORASTEROPS
#  endif
#  ifdef WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#    undef WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#    undef NOSHOWWINDOW
#  endif
#  ifdef WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#    undef WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#    undef OEMRESOURCE
#  endif
#  ifdef WINSUBSET_NOATOM_IS_NOT_DEFINED
#    undef WINSUBSET_NOATOM_IS_NOT_DEFINED
#    undef NOATOM
#  endif
#  ifdef WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#    undef WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#    undef NOCLIPBOARD
#  endif
#  ifdef WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#    undef WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#    undef NOCOLOR
#  endif
#  ifdef WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#    undef WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#    undef NOCTLMGR
#  endif
#  ifdef WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#    undef WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#    undef NODRAWTEXT
#  endif
#  ifdef WINSUBSET_NOGDI_IS_NOT_DEFINED
#    undef WINSUBSET_NOGDI_IS_NOT_DEFINED
#    undef NOGDI
#  endif
#  ifdef WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#    undef WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#    undef NOKERNEL
#  endif
#  ifdef WINSUBSET_NOUSER_IS_NOT_DEFINED
#    undef WINSUBSET_NOUSER_IS_NOT_DEFINED
#    undef NOUSER
#  endif
#  ifdef WINSUBSET_NONLS_IS_NOT_DEFINED
#    undef WINSUBSET_NONLS_IS_NOT_DEFINED
#    undef NONLS
#  endif
#  ifdef WINSUBSET_NOMB_IS_NOT_DEFINED
#    undef WINSUBSET_NOMB_IS_NOT_DEFINED
#    undef NOMB
#  endif
#  ifdef WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#    undef WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#    undef NOMEMMGR
#  endif
#  ifdef WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#    undef WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#    undef NOMETAFILE
#  endif
#  ifdef WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#    undef WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#    undef NOMINMAX
#  endif
#  ifdef WINSUBSET_NOMSG_IS_NOT_DEFINED
#    undef WINSUBSET_NOMSG_IS_NOT_DEFINED
#    undef NOMSG
#  endif
#  ifdef WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#    undef WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#    undef NOOPENFILE
#  endif
#  ifdef WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#    undef WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#    undef NOSCROLL
#  endif
#  ifdef WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#    undef WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#    undef NOSERVICE
#  endif
#  ifdef WINSUBSET_NOSOUND_IS_NOT_DEFINED
#    undef WINSUBSET_NOSOUND_IS_NOT_DEFINED
#    undef NOSOUND
#  endif
#  ifdef WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#    undef WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#    undef NOTEXTMETRIC
#  endif
#  ifdef WINSUBSET_NOWH_IS_NOT_DEFINED
#    undef WINSUBSET_NOWH_IS_NOT_DEFINED
#    undef NOWH
#  endif
#  ifdef WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#    undef NOWINOFFSETS
#  endif
#  ifdef WINSUBSET_NOCOMM_IS_NOT_DEFINED
#    undef WINSUBSET_NOCOMM_IS_NOT_DEFINED
#    undef NOCOMM
#  endif
#  ifdef WINSUBSET_NOKANJI_IS_NOT_DEFINED
#    undef WINSUBSET_NOKANJI_IS_NOT_DEFINED
#    undef NOKANJI
#  endif
#  ifdef WINSUBSET_NOHELP_IS_NOT_DEFINED
#    undef WINSUBSET_NOHELP_IS_NOT_DEFINED
#    undef NOHELP
#  endif
#  ifdef WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#    undef WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#    undef NOPROFILER
#  endif
#  ifdef WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#    undef WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#    undef NODEFERWINDOWPOS
#  endif
#  ifdef WINSUBSET_NOMCX_IS_NOT_DEFINED
#    undef WINSUBSET_NOMCX_IS_NOT_DEFINED
#    undef NOMCX
#  endif
#else
// Clone from winnt.h in MSVC 2012
#  if __cplusplus >= 201103L
#    include <cstdint>
#  else
#    include <stdint.h>
#  endif
// Clone from WinDef.h in MSVC 2012
typedef uint32_t  DWORD;
typedef uint8_t   BYTE;
typedef uint16_t  WORD;


//
// File header format.
//
typedef struct _IMAGE_FILE_HEADER {
  WORD  Machine;
  WORD  NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD  SizeOfOptionalHeader;
  WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;


//
// Directory format.
//
typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16


//
// Optional header format.
//
typedef struct _IMAGE_OPTIONAL_HEADER {
  //
  // Standard fields.
  //
  WORD  Magic;
  BYTE  MajorLinkerVersion;
  BYTE  MinorLinkerVersion;
  DWORD SizeOfCode;
  DWORD SizeOfInitializedData;
  DWORD SizeOfUninitializedData;
  DWORD AddressOfEntryPoint;
  DWORD BaseOfCode;
  DWORD BaseOfData;

  //
  // NT additional fields.
  //
  DWORD ImageBase;
  DWORD SectionAlignment;
  DWORD FileAlignment;
  WORD  MajorOperatingSystemVersion;
  WORD  MinorOperatingSystemVersion;
  WORD  MajorImageVersion;
  WORD  MinorImageVersion;
  WORD  MajorSubsystemVersion;
  WORD  MinorSubsystemVersion;
  DWORD Win32VersionValue;
  DWORD SizeOfImage;
  DWORD SizeOfHeaders;
  DWORD CheckSum;
  WORD  Subsystem;
  WORD  DllCharacteristics;
  DWORD SizeOfStackReserve;
  DWORD SizeOfStackCommit;
  DWORD SizeOfHeapReserve;
  DWORD SizeOfHeapCommit;
  DWORD LoaderFlags;
  DWORD NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;


//
// Section header format.
//
#define IMAGE_SIZEOF_SHORT_NAME  8

typedef struct _IMAGE_SECTION_HEADER {
  BYTE Name[IMAGE_SIZEOF_SHORT_NAME];
  union {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
  } Misc;
  DWORD VirtualAddress;
  DWORD SizeOfRawData;
  DWORD PointerToRawData;
  DWORD PointerToRelocations;
  DWORD PointerToLinenumbers;
  WORD  NumberOfRelocations;
  WORD  NumberOfLinenumbers;
  DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)


#endif  // WINSUBSET_H
