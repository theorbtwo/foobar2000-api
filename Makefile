#!/usr/bin/make -f 

CC = gcc
CFLAGS += -fexceptions -std=gnu++0x -fpermissive -DUNICODE

#CC = clang
#CFLAGS += -fexceptions -fdiagnostics-fixit-info -std=gnu++0x

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

%.i: %.cpp
	$(CC) -E $< $(CFLAGS) > $@

PFCOBJS = pfc/base64.o pfc/guid.o pfc/pathUtils.o pfc/profiler.o pfc/sort.o pfc/string_conv.o pfc/stringNew.o pfc/utf8.o pfc/bsearch.o pfc/other.o pfc/printf.o pfc/selftest.o pfc/stdafx.o pfc/string.o pfc/threads.o

pfc/pfc.a: $(PFCOBJS)
	ar -rcs $@ $^

