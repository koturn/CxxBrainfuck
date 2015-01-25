### This Makefile was written for nmake. ###
XBYAK_DIR        = xbyak
XBYAK_REPOSITORY = https://github.com/herumi/$(XBYAK_DIR)
XBYAK_INCS       = /I$(XBYAK_DIR)/

GETOPT_DIR        = getopt
GETOPT_REPOSITORY = https://github.com/koturn/$(GETOPT_DIR).git
GETOPT_LIBS_DIR   = $(GETOPT_DIR)/lib
GETOPT_LIB        = getopt$(DBG_SUFFIX).lib
GETOPT_LDLIBS     = /LIBPATH:$(GETOPT_LIBS_DIR) $(GETOPT_LIB)
GETOPT_INCS       = /Igetopt/include/


!if "$(CRTDLL)" == "true"
CRTLIB = /MD$(DBG_SUFFIX)
!else
CRTLIB = /MT$(DBG_SUFFIX)
!endif

!if "$(DEBUG)" == "true"
MSVCDBG_DIR        = msvcdbg
MSVCDBG_REPOSITORY = https://github.com/koturn/$(MSVCDBG_DIR).git
MSVCDBG_INCS       = /Imsvcdbg/

DBG_SUFFIX  = d
COPTFLAGS   = /Od /GS /Zi $(CRTLIB)
LDOPTFLAGS  = /Od /GS /Zi $(CRTLIB)
MSVC_MACROS = /D_CRTDBG_MAP_ALLOC /D_USE_MATH_DEFINES /D_SECURE_SCL=1
!else
DBG_SUFFIX  =
COPTFLAGS   = /Ox /GL $(CRTLIB)
LDOPTFLAGS  = /Ox /GL $(CRTLIB)
MSVC_MACROS = /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS \
              /D_SECURE_SCL=0 /D_USE_MATH_DEFINES
!endif

CPP      = cl
RM       = del /F
MAKE     = $(MAKE) /nologo
GIT      = git
INCS     = $(XBYAK_INCS) $(GETOPT_INCS) $(MSVCDBG_INCS)
MACROS   = /DUSE_XBYAK /DXBYAK_NO_OP_NAMES $(MSVC_MACROS)
CPPFLAGS = /nologo $(COPTFLAGS) /EHsc /W4 /c $(INCS) $(MACROS)
LDFLAGS  = /nologo $(LDOPTFLAGS)
LDLIBS   = /link $(GETOPT_LDLIBS)

TARGET   = brainfuck.exe
MAIN_OBJ = main.obj
OBJ      = brainfuck.obj
MAIN_SRC = $(MAIN_OBJ:.obj=.cpp)
SRC      = $(OBJ:.obj=.cpp)
HEADER   = $(OBJ:.obj=.h)
MAKEFILE = msvc.mk


.SUFFIXES: .cpp .obj .exe
.obj.exe:
	$(CPP) $(LDFLAGS) $** /Fe$@ $(LDLIBS)
.cpp.obj:
	$(CPP) $(CPPFLAGS) $** /Fo$@


all: $(GETOPT_LIBS_DIR)/$(GETOPT_LIB) $(XBYAK_DIR)/xbyak/xbyak.h $(MSVCDBG_DIR)/NUL $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJ)

$(MAIN_OBJ): $(MAIN_SRC) $(HEADER)

$(OBJ): $(SRC) $(HEADER)


$(XBYAK_DIR)/xbyak/xbyak.h:
	@if not exist $(@D)/NUL \
		$(GIT) clone $(XBYAK_REPOSITORY)

$(GETOPT_LIBS_DIR)/$(GETOPT_LIB):
	@if not exist $(@D)/NUL \
		$(GIT) clone $(GETOPT_REPOSITORY)
	cd $(GETOPT_DIR)  &  $(MAKE) /f $(MAKEFILE)  &  cd $(MAKEDIR)

$(MSVCDBG_DIR)/NUL:
	@if not exist $(@D)/NUL \
		$(GIT) clone $(MSVCDBG_REPOSITORY)


clean:
	$(RM) $(TARGET) $(MAIN_OBJ) $(OBJ) *.ilk *.pdb
cleanobj:
	$(RM) $(MAIN_OBJ) $(OBJ) *.ilk *.pdb
