.PHONY: all clean check install

CC       := gcc
CFLAGS   := -g -MMD -MP -Wall -fPIC
LDFLAGS  :=
CPPFLAGS := -I./include

CFLAGS   += $(shell pkg-config fuse --cflags)
LDFLAGS  += $(shell pkg-config fuse --libs)
BINARY   := bindex

TARGET   := ./bin/$(BINARY)
SRCDIR   := ./src
OBJDIR   := ./obj
SOURCES  := $(wildcard $(SRCDIR)/*.c)
OBJECTS  := $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  := $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -rf $(OBJDIR) $(DEPENDS) $(TARGET)

check:
	make --no-print-directory -C test

install: $(TARGET)
	mv -v $(TARGET) /usr/bin/

-include $(DEPENDS)
