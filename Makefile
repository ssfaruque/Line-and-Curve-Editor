
CPP			:= g++
CPPFLAGS	:= -g -Wall -std=c++11

INCLUDEDIR	:= include
SRCDIR		:= src
DEPDIR		:= build/deps
OBJDIR		:= build/objs

OS := $(shell uname -s)

ifeq ($(OS), Linux)
    LIBS	:= -lGL -lglut -lm
	CPPFLAGS += -Werror
endif

ifeq ($(OS), Darwin)
    LIBS	:= -framework OpenGL -framework GLUT
	CPPFLAGS += -Wno-deprecated-declarations
endif


PROGNAME	:= out
SRCS		:= $(shell ls src)
OBJS		:= $(SRCS:.cpp=.o)
DEPS		:= $(OBJS:.o=.d)


all: $(PROGNAME)

-include $(DEPS)

$(PROGNAME): $(patsubst %,$(OBJDIR)/%, $(OBJS)) 
	$(CPP) $(CPPFLAGS) $^ $(LIBS) -o $@ 


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(CPPFLAGS) -I $(INCLUDEDIR) -I libs -c $< -o $@ -MMD -MF $(DEPDIR)/$(patsubst $(OBJDIR)/%.o,%.d, $@)

clean:
	$(RM) build/objs/*
	$(RM) build/deps/*
	$(RM) out

