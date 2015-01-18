### This Makefile was written for nmake. ###
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
INCS     = $(MSVCDBG_INCS)
MACROS   = $(MSVC_MACROS)
CPPFLAGS = /nologo $(COPTFLAGS) /EHsc /W4 /c $(INCS) $(MACROS)
LDFLAGS  = /nologo $(LDOPTFLAGS)

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


all: $(MSVCDBG_DIR)/NUL $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJ)

$(MAIN_OBJ): $(MAIN_SRC) $(HEADER)

$(OBJ): $(SRC) $(HEADER)


$(MSVCDBG_DIR)/NUL:
	@if not exist $(@D)/NUL \
		$(GIT) clone $(MSVCDBG_REPOSITORY)


clean:
	$(RM) $(TARGET) $(MAIN_OBJ) $(OBJ) *.pdb *.ilk
cleanobj:
	$(RM) $(MAIN_OBJ) $(OBJ) *.pdb *.ilk
