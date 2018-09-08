TARGET=libairplay

SRCDIR=src/
OBJDIR=obj/
BINDIR=bin/

CC=clang++
CFLAGS=-c -std=c++11 -g -O2 -Wall -Wno-unused-function -Wshadow -fno-rtti -fblocks
LDFLAGS=-stdlib=libc++ -lpthread -g

SRCS=$(wildcard $(SRCDIR)*.cpp)
OBJS=$(addprefix $(OBJDIR),$(notdir $(SRCS:.cpp=.o)))
EXECUTABLE=$(BINDIR)$(TARGET)
DEPS=$(wildcard $(OBJDIR)*.d)

.PHONY: all
all: $(TARGET)

.PHONY: dirs
dirs:
	mkdir -p $(OBJDIR) $(BINDIR)

$(TARGET): $(OBJS) dirs
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)
	dsymutil $(EXECUTABLE)

$(OBJDIR)%.o: $(SRCDIR)%.cpp dirs
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM -MP -MT $@ -MT $(OBJDIR)$*.d $(CFLAGS) $< > obj/$*.d

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)
