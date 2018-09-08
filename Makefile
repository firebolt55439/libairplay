TARGET=libairplay

SRCDIR=src/
OBJDIR=obj/

CC=clang++
CFLAGS=-c -std=c++11 -g -O2 -Wall -Wno-unused-function -Wshadow -fno-rtti -fblocks
LDFLAGS=-stdlib=libc++ -lpthread -g

SRCS=$(wildcard $(SRCDIR)*.cpp)
OBJS=$(addprefix $(OBJDIR),$(notdir $(SRCS:.cpp=.o)))

.PHONY: all
all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJDIR)
