### This Makefile was written for GNU Make. ###
XBYAK_DIR        := xbyak
XBYAK_REPOSITORY := https://github.com/herumi/$(XBYAK_DIR)
XBYAK_INCS       := -I$(XBYAK_DIR)/

ifeq ($(DEBUG),true)
    CXXOPTFLAGS := -O0 -g3 -ftrapv -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_GLIBCXX_DEBUG
    LDLIBS      += -lssp
else
ifeq ($(OPT),true)
    CXXOPTFLAGS := -flto -Ofast -march=native -DNDEBUG
    LDOPTFLAGS  := -flto -Ofast -s
else
ifeq ($(LTO),true)
    CXXOPTFLAGS := -flto -DNDEBUG
    LDOPTFLAGS  := -flto
else
    CXXOPTFLAGS := -O3 -DNDEBUG
    LDOPTFLAGS  := -O3 -s
endif
endif
endif

C_WARNING_FLAGS := -Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 \
                   -Wcast-align -Wcast-qual -Wconversion \
                   -Wfloat-equal -Wpointer-arith -Wswitch-enum \
                   -Wwrite-strings -pedantic
# CXX_WARNING_FLAGS := $(C_WARNING_FLAGS) -Woverloaded-virtual

CXX      := g++
MAKE     := make
GIT      := git
INCS     := $(XBYAK_INCS)
MACROS   := -DUSE_XBYAK -DXBYAK_NO_OP_NAMES
CXXFLAGS := -pipe $(CXX_WARNING_FLAGS) $(CXXOPTFLAGS) $(INCS) $(MACROS)
LDFLAGS  := -pipe $(LDOPTFLAGS)

TARGET   := brainfuck
MAIN_OBJ := main.o
OBJ1     := Brainfuck.o
OBJ2     := BfIRCompiler.o
OBJ3     := BfJitCompiler.o
MAIN_SRC := $(MAIN_SRC:%.o=%.cpp)
SRC1     := $(OBJ1:%.o=%.cpp)
SRC2     := $(OBJ2:%.o=%.cpp)
SRC3     := $(OBJ3:%.o=%.cpp)
HEADER1  := $(OBJ1:%.o=%.h)
HEADER2  := $(OBJ2:%.o=%.h)
HEADER3  := $(OBJ3:%.o=%.h)
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

%.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@
%.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@


.PHONY: all
all: $(XBYAK_DIR)/xbyak/xbyak.h $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJ1) $(OBJ2) $(OBJ3)

$(MAIN_OBJ): $(MAIN_SRC)

$(MAIN_SRC): $(HEADER1)

$(OBJ1): $(SRC1)

$(OBJ2): $(SRC2)

$(OBJ3): $(SRC3)

$(SRC1): $(HEADER1) $(HEADER2) $(HEADER3) $(GENERATORS)

$(SRC2): $(HEADER2)

$(SRC3): $(HEADER3)


$(XBYAK_DIR)/xbyak/xbyak.h:
	@if [ ! -d $(dir $@) ]; then \
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
