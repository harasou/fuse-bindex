.PHONY: clean

CC       := gcc
CFLAGS   := -g -MMD -MP -Wall
LDFLAGS  :=
CPPFLAGS := -I../include

CFLAGS   += $(shell pkg-config fuse --cflags)
LDFLAGS  += $(shell pkg-config fuse --libs) -lcriterion
BINARY   := bindex-test
BINDEXO  := ../obj/bindex.o

TARGET   := ./$(BINARY)
SOURCES  := $(wildcard *.c)

$(TARGET): $(SOURCES) $(BINDEXO)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(CPPFLAGS)

$(BINDEXO):
	make -C .. obj/bindex.o

clean:
	-rm -rf $(TARGET)
