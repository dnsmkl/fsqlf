PRJNAME=fsqlf

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-pedantic-errors
CFLAGS+=-g
CFLAGS+=-Iinclude

CXXFLAGS+=-DVERSION=\"$(VERSION)\"
CXXFLAGS+=-Iinclude

ifdef WIN
	BLD=builds/windows
	OS_TARGET=windows
	EXEC_CLI=$(BLD)/fsqlf.exe
	EXEC_GUI=$(BLD)/wx_fsqlf.exe
	UTIL_TXT2H=$(BLD)/text_to_header.exe
	CFLAGS+=-DBUILDING_LIBFSQLF
	# i686
	# https://myonlineusb.wordpress.com/2011/06/08/what-is-the-difference-between-i386-i486-i586-i686-i786/
	# CC=i586-mingw32msvc-gcc
	# CXX=i586-mingw32msvc-g++
	# CXXFLAGS+= `/usr/i586-mingw32msvc/bin/wx-config --cxxflags | sed 's/-mthreads//'`
	# LDFLAGS+= `/usr/i586-mingw32msvc/bin/wx-config --libs     | sed 's/-mthreads//'`
	CC=i686-w64-mingw32-gcc
	CXX=i686-w64-mingw32-g++
	CXXFLAGS+= `/usr/i686-w64-mingw32/bin/wx-config --cxxflags | sed 's/-mthreads//'`
	LDFLAGS+= `/usr/i686-w64-mingw32/bin/wx-config --libs     | sed 's/-mthreads//'`
	# Option "-mthreads" needs to be removed, so mingwm10.dll would not be needed
	# (http://old.nabble.com/mingwm10.dll-ts8920679.html)
else
	BLD=builds/linux
	OS_TARGET=linux
	PREFIX=/usr/local
	EXEC_CLI=$(BLD)/fsqlf
	EXEC_GUI=$(BLD)/wx_fsqlf
	UTIL_TXT2H=$(BLD)/text_to_header
	CC=gcc
	CXX=g++
	CXXFLAGS+= `wx-config --cxxflags`
	LDFLAGS+= `wx-config --libs`
	ifneq (Darwin, ${_system_type})
		CFLAGS+=-m32
	else
		CFLAGS+=-m64
	endif
endif

ifeq (Darwin, ${_system_type})
	LIBNAME=$(BLD)/libfsqlf.dylib
	LIBFLAGS=-dynamiclib
else
	ifdef WIN
		LIBNAME=$(BLD)/libfsqlf.dll
		LIBFLAGS=-shared -Wl,--out-implib,libfsqlf.a
	else
		LIBNAME=$(BLD)/libfsqlf.so
		LIBFLAGS=-shared
	endif
endif



.PHONY: all clean zip test install uninstall



all: $(EXEC_CLI) $(EXEC_GUI)



#
# BUILD LIB
#
LCOBJ += $(BLD)/lib_fsqlf/conf_file/conf_file_create.o
LCOBJ += $(BLD)/lib_fsqlf/conf_file/conf_file_read.o
LCOBJ += $(BLD)/lib_fsqlf/formatter/lex_wrapper.o
LCOBJ += $(BLD)/lib_fsqlf/formatter/print_keywords.o
LCOBJ += $(BLD)/lib_fsqlf/formatter/tokque.o
LCOBJ += $(BLD)/lib_fsqlf/kw/kw.o
LCOBJ += $(BLD)/lib_fsqlf/kw/kwmap.o
LCOBJ += $(BLD)/lib_fsqlf/lex/token.o
LCOBJ += $(BLD)/utils/queue/queue.o
LCOBJ += $(BLD)/utils/stack/stack.o
LCOBJ += $(BLD)/utils/string/read_int.o
BLDDIRS += $(dir $(LCOBJ))

$(LCOBJ): $(BLD)/%.o: ./%.c | $(BLDDIRS)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(BLD) -Ilib_fsqlf/formatter
$(BLD)/lex.yy.o: $(BLD)/lex.yy.c
	$(CC) -o $@ -c $< $(CFLAGS) -Ilib_fsqlf/formatter

$(filter lib_fsqlf/%,$(LCOBJ)): $(BLDP)%.o: ./%.c include/lib_fsqlf.h

$(LIBNAME): $(LCOBJ) $(BLD)/lex.yy.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS)

$(BLD)/lib_fsqlf/conf_file/conf_file_read.o: utils/string/read_int.h
$(BLD)/lib_fsqlf/formatter/lex_wrapper.o: $(BLD)/lex.yy.h
$(BLD)/lex.yy.h: $(BLD)/lex.yy.c
$(BLD)/lex.yy.c: lib_fsqlf/formatter/fsqlf.lex lib_fsqlf/formatter/print_keywords.h
	# flex options (e.g. `-o`) has to be before input file
	flex -o $@ --header-file=$(BLD)/lex.yy.h $<



#
# BUILD CLI
#
COBJ += $(BLD)/cli/main.o
COBJ += $(BLD)/cli/cli.o
BLDDIRS += $(dir $(COBJ))

$(COBJ): $(BLD)/%.o: ./%.c include/lib_fsqlf.h | $(BLDDIRS)
	$(CC) -o $@ -c $< $(CFLAGS)   

INTUTIL = $(BLD)/utils/string/read_int.o
$(EXEC_CLI): $(COBJ) $(INTUTIL) $(LIBNAME)
	$(CC) -o $@ $(CFLAGS) $(COBJ) $(INTUTIL) -L$(BLD) -lfsqlf -Wl,-rpath,.



#
# BUILD GUI
#
CXXOBJ += $(BLD)/gui/wx_fsqlf.o
CXXOBJ += $(BLD)/gui/basic_notepad.o
CXXOBJ += $(BLD)/gui/dnd_target.o
BLDDIRS += $(dir $(CXXOBJ))

$(CXXOBJ): $(BLD)/%.o: ./%.cpp ./%.hpp | $(BLDDIRS)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -I$(BLD)

$(EXEC_GUI): $(CXXOBJ) $(LIBNAME)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -L$(BLD) -lfsqlf -Wl,-rpath,.

$(BLD)/gui/wx_fsqlf.o: gui/basic_notepad.hpp
$(BLD)/gui/basic_notepad.o: gui/dnd_target.hpp $(BLD)/license_text.h

$(UTIL_TXT2H): utils/text_to_header/text_to_header.c
	$(CC) -o $@ $< $(CFLAGS)
$(BLD)/license_text.h: LICENSE $(UTIL_TXT2H)
	$(UTIL_TXT2H) $< $@ LICENSE_TEXT



#
# TESTING
#
# Simple regression testing - testing against gold (pre-saved correct output)
# Given certain input to `fsqlf`, actual output (lead) is compared
# against to it's predefined expected output (gold).
# TF stands for "test file".
TSTOBJ += $(BLD)/tests/tools/file_compare.o
TSTOBJ += $(BLD)/tests/format_files_test.o
BLDDIRS += $(dir $(TSTOBJ))
BLDDIRS += $(BLD)/tests/cases

test: $(EXEC_CLI) $(BLD)/tests/format_files_test
	$(BLD)/tests/format_files_test

$(TSTOBJ): $(BLD)/%.o: ./%.c | $(BLDDIRS)
	$(CC) -o $@ -c $< $(CFLAGS) \
		-D PATH_FSQLF_CLI=\"$(EXEC_CLI)\" \
		-D PATH_FSQLF_LIB=\"$(BLD)\" \
		-D PATH_TC_STATICSQL=\"tests/cases/\" \
		-D PATH_TC_GENERATED=\"$(BLD)/tests/cases/\"

$(BLD)/tests/format_files_test: $(TSTOBJ)
	$(CC) -o $@ $^ $(CFLAGS)



#
# OUT OF SOURCE BUILD FOLDERS
#
$(sort $(BLDDIRS)):
	mkdir -p $@



#
#  CLEANUP
#
clean:
	rm -f -R builds/



#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: lib_fsqlf/kw/kwmap_defaults.def $(EXEC_CLI)
	LD_LIBRARY_PATH=$(BLD) ./$(EXEC_CLI) --create-config-file

VERSION:=$(shell git describe master)
PKGAREA:=builds/packaging
ZIP_NAME:=$(PKGAREA)/$(PRJNAME).$(VERSION).zip

zip: tmp_folder
	rm -f $(ZIP_NAME)
	git archive master  -o $(ZIP_NAME)  --format=zip --prefix='$(PRJNAME)/source/'
	zip -r $(ZIP_NAME) $(PKGAREA)/$(PRJNAME)

tmp_folder: LICENSE README.md
	make prep_bin
	make prep_bin WIN=1
	cp -t $(PKGAREA)/$(PRJNAME) $^

prep_bin: $(EXEC_CLI) $(EXEC_GUI) $(LIBNAME) formatting.conf
	rm -Rf $(PKGAREA)/$(PRJNAME)/$(OS_TARGET)
	mkdir -p $(PKGAREA)/$(PRJNAME)/$(OS_TARGET)
	cp -t $(PKGAREA)/$(PRJNAME)/$(OS_TARGET) $^



#
# INSTALLATION
#
ifeq ($(OS_TARGET),linux)

install: $(EXEC_CLI) $(EXEC_GUI) formatting.conf
	install -d $(PREFIX)/bin
	install $(EXEC_CLI) $(EXEC_GUI) $(PREFIX)/bin
	install -d $(PREFIX)/share/fsqlf
	install -m 644 formatting.conf $(PREFIX)/share/fsqlf/formatting.conf.example
	install -d $(PREFIX)/lib
	install $(LIBNAME) $(PREFIX)/lib

uninstall:
ifdef EXEC_CLI
	rm -vf $(PREFIX)/bin/$(EXEC_CLI)
endif
ifdef EXEC_GUI
	rm -vf $(PREFIX)/bin/$(EXEC_GUI)
endif
	rm -vf $(PREFIX)/share/fsqlf/formatting.conf.example
	rm -vfd $(PREFIX)/share/fsqlf
	rm -vf $(PREFIX)/lib/$(LIBNAME)
endif



#
# CMake
#
CMREL:=-DCMAKE_BUILD_TYPE=RELEASE
CMTLINUX32:=-DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain-linux-x86-gcc.cmake
CMTLINUX64:=-DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain-linux-x86-64-gcc.cmake
CMTWIN32:=-DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain-windows-x86-mingw.cmake

cmake-linux-x86:
	mkdir -p builds/linux-x86
	cd builds/linux-x86 && cmake $(CMREL) $(CMTLINUX32) ../.. && make && make test

cmake-linux-x86-64:
	mkdir -p builds/linux-x86-64
	cd builds/linux-x86-64 && cmake $(CMREL) $(CMTLINUX64) ../.. && make && make test

cmake-win-x86:
	mkdir -p builds/win-x86
	cd builds/win-x86 && cmake $(CMREL) $(CMTWIN32) ../.. && make && make test



# makefile reference
# $@ - target
# $+ - all prerequisites
# $^ - all prerequisites, but list each name only once
# $< - first prerequisite
# $? - all prerequisites newer than target
# $| - order only prerequisites
#
# See also:
# http://www.gnu.org/software/make/manual/make.html#Automatic-Variables
