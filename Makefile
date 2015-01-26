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
CXX_WARNING_FLAGS := $(C_WARNING_FLAGS) -Woverloaded-virtual

CXX      := g++
MAKE     := make
GIT      := git
INCS     := $(XBYAK_INCS)
MACROS   := -DUSE_XBYAK -DXBYAK_NO_OP_NAMES
CXXFLAGS := -pipe $(CXX_WARNING_FLAGS) $(CXXOPTFLAGS) $(INCS) $(MACROS)
LDFLAGS  := -pipe $(LDOPTFLAGS)
TARGET   := brainfuck
MAIN_OBJ := main.o
OBJ      := Brainfuck.o
MAIN_SRC := $(MAIN_SRC:%.o=%.cpp)
SRC      := $(OBJ:%.o=%.cpp)
HEADER   := $(OBJ:%.o=%.h)

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

$(TARGET): $(MAIN_OBJ) $(OBJ)

$(MAIN_OBJ): $(MAIN_SRC) $(HEADER)

$(OBJ): $(SRC) $(HEADER)

$(XBYAK_DIR)/xbyak/xbyak.h:
	@if [ ! -d $(dir $@) ]; then \
		$(GIT) clone $(XBYAK_REPOSITORY); \
	fi


.PHONY: test
test:
	./$(TARGET) -h


.PHONY: clean
clean:
	$(RM) $(TARGET) $(MAIN_OBJ) $(OBJ)
.PHONY: cleanobj
cleanobj:
	$(RM) $(MAIN_OBJ) $(OBJ)
