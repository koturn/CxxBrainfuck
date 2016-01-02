### This Makefile was written for GNU Make. ###
XBYAK_DIR        := xbyak
XBYAK_REPOSITORY := https://github.com/herumi/$(XBYAK_DIR)
XBYAK_INCS       := -I$(XBYAK_DIR)/

ifeq ($(DEBUG),true)
    OPT_CFLAGS   := -O0 -g3 -ftrapv -fstack-protector-all -D_FORTIFY_SOURCE=2
ifneq ($(shell echo $$OSTYPE),cygwin)
    OPT_CFLAGS   := $(OPT_CFLAGS) -fsanitize=address -fno-omit-frame-pointer
endif
    OPT_CXXFLAGS := $(OPT_CFLAGS) -D_GLIBCXX_DEBUG
    OPT_LDLIBS   := -lssp
else
ifeq ($(OPT),true)
    OPT_CFLAGS   := -flto -Ofast -march=native -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -flto -Ofast -s
else
ifeq ($(LTO),true)
    OPT_CFLAGS   := -flto -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -flto
else
    OPT_CFLAGS   := -O3 -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -O3 -s
endif
endif
endif

WARNING_CFLAGS := -Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 \
                  -Wcast-align -Wcast-qual -Wconversion \
                  -Wfloat-equal -Wpointer-arith -Wswitch-enum \
                  -Wwrite-strings -pedantic
WARNING_CXXFLAGS := $(WARNING_CFLAGS) -Woverloaded-virtual


CXX      := g++
MAKE     := make
GIT      := git
INCS     := $(XBYAK_INCS)
MACROS   := -DUSE_XBYAK -DXBYAK_NO_OP_NAMES
CXXFLAGS := -pipe $(WARNING_CXXFLAGS) $(OPT_CXXFLAGS) $(INCS) $(MACROS)
LDFLAGS  := -pipe $(OPT_LDFLAGS)

TARGET   := brainfuck
MAIN_OBJ := main.o
OBJ1     := Brainfuck.o
OBJ2     := BfIRCompiler.o
OBJ3     := BfJitCompiler.o
MAIN_SRC := $(MAIN_SRC:.o=.cpp)
SRC1     := $(OBJ1:.o=.cpp)
SRC2     := $(OBJ2:.o=.cpp)
SRC3     := $(OBJ3:.o=.cpp)
HEADER1  := $(OBJ1:.o=.h)
HEADER2  := $(OBJ2:.o=.h)
HEADER3  := $(OBJ3:.o=.h)
GENERATORS := $(addprefix CodeGenerator/, \
                  CodeGenerator.h \
                  _AllGenerator.h \
                  $(addprefix SourceGenerator/, \
                      SourceGenerator.h \
                      $(addprefix Lang/, \
                          GeneratorC.h \
                          GeneratorCpp.h \
                          GeneratorCSharp.h \
                          GeneratorJava.h \
                          GeneratorLua.h \
                          GeneratorPython.h \
                          GeneratorRuby.h)) \
                  $(addprefix BinaryGenerator/, \
                      BinaryGenerator.h \
                      $(addprefix Arch/, \
                          GeneratorWinX86.h \
                          GeneratorElfX64.h \
                          winsubset.h \
                          elfsubset.h)))


ifeq ($(OS),Windows_NT)
    TARGET := $(addsuffix .exe, $(TARGET))
else
    TARGET := $(addsuffix .out, $(TARGET))
endif

.SUFFIXES: .exe .o .out
.o.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.o, $^) $(LDLIBS) -o $@
.o.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.o, $^) $(LDLIBS) -o $@
.o:
	$(CXX) $(LDFLAGS) $(filter %.c %.o, $^) $(LDLIBS) -o $@

%.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@
%.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@


.PHONY: all
all: $(XBYAK_DIR)/xbyak/xbyak.h $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJ1) $(OBJ2) $(OBJ3)

$(MAIN_OBJ): $(MAIN_SRC)

$(MAIN_SRC): $(HEADER1)

$(OBJ1): $(SRC1) $(HEADER1) $(HEADER2) $(HEADER3) $(GENERATORS)

$(OBJ2): $(SRC2) $(HEADER2)

$(OBJ3): $(SRC3) $(HEADER3)


$(XBYAK_DIR)/xbyak/xbyak.h:
	@if [ ! -d $(@D) ]; then \
		$(GIT) clone $(XBYAK_REPOSITORY); \
	fi


.PHONY: test
test:
	./$(TARGET) -h


.PHONY: clean
clean:
	$(RM) $(TARGET) $(MAIN_OBJ) $(OBJ1) $(OBJ2) $(OBJ3)
.PHONY: cleanobj
cleanobj:
	$(RM) $(MAIN_OBJ) $(OBJ1) $(OBJ2) $(OBJ3)
