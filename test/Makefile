.PHONY: check clean criterion

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
CRITERION := /usr/lib/libcriterion.so

check: $(TARGET)
	$(TARGET)

$(TARGET): $(SOURCES) $(BINDEXO) $(CRITERION)
	$(CC) -o $@ $(SOURCES) $(BINDEXO) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS)

$(BINDEXO):
	make -C .. obj/bindex.o

$(CRITERION):
	../misc/criterion

clean:
	-rm -rf $(TARGET)
