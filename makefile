PROJECTFOLDER=fsqlf

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-pedantic-errors
CFLAGS+=-g
CFLAGS+=-Iinclude

CXXFLAGS+=-DVERSION=\"$(VERSION)\"
CXXFLAGS+=-Iinclude

ifdef WIN
	OS_TARGET=windows
	EXEC_CLI=fsqlf.exe
	EXEC_GUI=wx_fsqlf.exe
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
	OS_TARGET=linux
	PREFIX=/usr/local
	EXEC_CLI=fsqlf
	EXEC_GUI=wx_fsqlf
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
	LIBNAME=libfsqlf.dylib
	LIBFLAGS=-dynamiclib
else
	ifdef WIN
		LIBNAME=libfsqlf.dll
		LIBFLAGS=-shared -Wl,--out-implib,libfsqlf.a
	else
		LIBNAME=libfsqlf.so
		LIBFLAGS=-shared
	endif
endif


.PHONY: all  clean  zip  test  test-print  test-gold  clean_obj  clean_test  install  uninstall



all: $(EXEC_CLI)  $(EXEC_GUI)



#
# BUILD LIB
#
LCOBJ += lib_fsqlf/conf_file/conf_file_create.o
LCOBJ += lib_fsqlf/conf_file/conf_file_read.o
LCOBJ += lib_fsqlf/formatter/lex.yy.o
LCOBJ += lib_fsqlf/formatter/lex_wrapper.o
LCOBJ += lib_fsqlf/formatter/print_keywords.o
LCOBJ += lib_fsqlf/formatter/tokque.o
LCOBJ += lib_fsqlf/kw/kw.o
LCOBJ += lib_fsqlf/kw/kwmap.o
LCOBJ += lib_fsqlf/lex/token.o
LCOBJ += utils/queue/queue.o
LCOBJ += utils/stack/stack.o
LCOBJ += utils/string/read_int.o

$(LCOBJ): %.o: %.c
	$(CC) $(CFLAGS)  -c $<  -o $@

$(filter lib_fsqlf/%,$(LCOBJ)): %.o: %.c include/lib_fsqlf.h

$(LIBNAME): $(LCOBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) $^   -o $@

lib_fsqlf/conf_file/conf_file_read.o: utils/string/read_int.h
lib_fsqlf/formatter/lex_wrapper.o: lib_fsqlf/formatter/lex.yy.h
lib_fsqlf/formatter/lex.yy.h: lib_fsqlf/formatter/lex.yy.c
lib_fsqlf/formatter/lex.yy.c: lib_fsqlf/formatter/fsqlf.lex lib_fsqlf/formatter/print_keywords.h
	# flex options (e.g. `-o`) has to be before input file
	flex  -o $@ --header-file=lib_fsqlf/formatter/lex.yy.h $<



#
# BUILD CLI
#
COBJ += cli/main.o
COBJ += cli/cli.o

$(COBJ): %.o: %.c include/lib_fsqlf.h
	$(CC) $(CFLAGS)  -c $<  -o $@

$(EXEC_CLI): $(COBJ) $(LIBNAME)
	$(CC) $(CFLAGS)  $(COBJ)  -L. -lfsqlf  -Wl,-rpath,.  -o $@
	# strip $@


#
# BUILD GUI
#
$(EXEC_GUI): wx_fsqlf.o  basic_notepad.o  dnd_target.o  $(LIBNAME)
	$(CXX)  $^  -o $@  $(CXXFLAGS)  $(LDFLAGS)  -L. -lfsqlf  -Wl,-rpath,.
	strip $@

# generic rule for C++ building
CXXOBJ = wx_fsqlf.o basic_notepad.o dnd_target.o

$(CXXOBJ): %.o: gui/%.cpp  gui/%.hpp
	$(CXX)  -c $<  -o $@  $(CXXFLAGS)

wx_fsqlf.o: gui/basic_notepad.hpp
basic_notepad.o: gui/dnd_target.hpp  gui/license_text.h

gui/license_text.h: LICENSE text_to_header
	./text_to_header  $<  $@  LICENSE_TEXT

text_to_header: utils/text_to_header/text_to_header.c
	$(CC) $(CFLAGS)  $<  -o $@

#
# TESTING
#

# Simple regression testing - testing against gold (pre-saved correct output)
# Given certain input to `fsqlf`, actual output (lead) is compared
# against to it's predefined expected output (gold).
# TF stands for "test file".
test: test-format-files

test-format-files: $(EXEC_CLI) tests/format_files_test
	cd tests && ./format_files_test

tests/tools/file_compare.o: tests/tools/file_compare.c
	$(CC) $(CFLAGS)  -c $<  -o $@

tests/format_files_test.o: tests/format_files_test.c
	$(CC) $(CFLAGS)  -c $<  -o $@

tests/format_files_test: tests/format_files_test.o tests/tools/file_compare.o
	$(CC) $(CFLAGS)  $^  -o $@



#
#  CLEANUP
#
TMP_BAKUPS=$(wildcard */*~) $(wildcard *~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)

clean: clean_local  clean_win  clean_obj  clean_test

clean_local:
	rm -R -f $(EXEC_GUI) $(EXEC_CLI)  lib_fsqlf/formatter/lex.yy.c  $(TMP_BAKUPS) \
		lib_fsqlf/formatter/lex.yy.h \
		$(LIBNAME) libfsqlf.a \
		$(wildcard $(PROJECTFOLDER)*.zip) tmp gui/license_text.h $(CONF_FILE) \
		text_to_header builds
	make clean_obj

clean_win:
	make clean_local WIN=1

clean_obj:
	rm -f *.o lib_fsqlf/*.o lib_fsqlf/*/*.o utils/*/*.o cli/*.o

clean_test:
	rm -f tests/format_files_test tests/*.o tests/tools/*.o tests/cases/*_actual.sql




#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: lib_fsqlf/kw/kwmap_defaults.def $(EXEC_CLI)
	./$(EXEC_CLI) --create-config-file

VERSION:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(VERSION).zip

zip: tmp_folder
	rm -f $(ZIP_NAME)
	git archive master  -o $(ZIP_NAME)  --format=zip --prefix='$(PROJECTFOLDER)/source/'
	cd tmp/ &&   zip -r ../$(ZIP_NAME)  $(PROJECTFOLDER)

tmp_folder: LICENSE README.md
	make prep_bin
	make clean_obj # to ensure that object files are for needed OS
	make prep_bin WIN=1
	cp    -t tmp/$(PROJECTFOLDER)   $^

prep_bin: $(EXEC_CLI) $(EXEC_GUI) $(LIBNAME) formatting.conf
	mkdir -p tmp/$(PROJECTFOLDER)/$(OS_TARGET)
	cp    -t tmp/$(PROJECTFOLDER)/$(OS_TARGET)    $^



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
