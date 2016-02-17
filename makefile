PROJECTFOLDER=fsqlf

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-pedantic-errors
CFLAGS+=-g
CFLAGS+=-Iinclude

CXXFLAGS+=-DVERSION=\"$(VERSION)\"

ifdef WIN
	OS_TARGET=windows
	EXEC_CLI=fsqlf.exe
	EXEC_GUI=wx_fsqlf.exe
	CC=i586-mingw32msvc-gcc
	CXX=i586-mingw32msvc-g++
	CXXFLAGS+= `/usr/i586-mingw32msvc/bin/wx-config --cxxflags | sed 's/-mthreads//'`
	LDFLAGS+= `/usr/i586-mingw32msvc/bin/wx-config --libs     | sed 's/-mthreads//'`
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
LCOBJ += lib_fsqlf/kw/kwall_init.o
LCOBJ += lib_fsqlf/lex/token.o
LCOBJ += utils/queue/queue.o
LCOBJ += utils/stack/stack.o
LCOBJ += utils/string/read_int.o

$(LCOBJ): %.o: %.c
	$(CC) $(CFLAGS)  -c $<  -o $@

$(LIBNAME): $(LCOBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) $^   -o $@

lib_fsqlf/conf_file/conf_file_create.o: lib_fsqlf/conf_file/conf_file_constants.h
lib_fsqlf/conf_file/conf_file_read.o: lib_fsqlf/conf_file/conf_file_constants.h utils/string/read_int.h

lib_fsqlf/formatter/lex.yy.h: lib_fsqlf/formatter/lex.yy.c
lib_fsqlf/formatter/lex.yy.c: lib_fsqlf/formatter/fsqlf.lex lib_fsqlf/formatter/print_keywords.h
	# flex options (e.g. `-o`) has to be before input file
	flex  -o $@ --header-file=lib_fsqlf/formatter/lex.yy.h $<



#
# BUILD CLI
#
COBJ += cli/main.o
COBJ += cli/cli.o

$(COBJ): %.o: %.c
	$(CC) $(CFLAGS)  -c $<  -o $@

cli/main.o: lib_fsqlf/formatter/lex.yy.h

$(EXEC_CLI): $(COBJ) $(LIBNAME)
	$(CC) $(CFLAGS)  $(COBJ)  -L. -lfsqlf  -Wl,-rpath,.  -o $@
	# strip $@



#
# BUILD GUI
#
$(EXEC_GUI): wx_fsqlf.o  basic_notepad.o  dnd_target.o | $(EXEC_CLI)
	$(CXX)  $^  -o $@  $(CXXFLAGS)  $(LDFLAGS)
	strip $@

# generic rule for C++ building
CXXOBJ = wx_fsqlf.o basic_notepad.o dnd_target.o

$(CXXOBJ): %.o: gui/%.cpp  gui/%.hpp
	$(CXX)  -c $<  -o $@  $(CXXFLAGS)

wx_fsqlf.o: gui/basic_notepad.hpp
basic_notepad.o: gui/dnd_target.hpp  gui/license_text.h

gui/license_text.h: LICENSE
	buildtools/text_to_header.sh  $<  $@



#
# TESTING
#

# Simple regression testing - testing against gold (pre-saved correct output)
# Given certain input to `fsqlf`, actual output (lead) is compared
# against to it's predefined expected output (gold).
# TF stands for "test file".
TF_INPUT = $(wildcard testcases/*_input.sql)
TF_ACTUAL_RESULTS = $(patsubst %_input.sql,%_actual.sql,$(TF_INPUT))
$(TF_ACTUAL_RESULTS): %_actual.sql: %_input.sql | %_expected.sql
	@./fsqlf $< $@
	@diff -q $@ $|
	@rm $@
	@echo "Result as expected for: " $<

test-gold: $(EXEC_CLI)  $(TF_ACTUAL_RESULTS)

test: test-gold

# Output for visual inspection.
test-print: $(EXEC_CLI)
	./$(EXEC_CLI) testcases/bigquery.sql \
	|  awk -F, '{ printf("%4d # ", NR) ; print}'

# When adding new test cases %.sql files, auto-generate %_expected.sql files.
TF_NEW_GOLD = $(patsubst %.sql,%_expected.sql,$(TF_INPUT))
$(TF_NEW_GOLD): %_expected.sql: %.sql
	./fsqlf $< $@

generate-gold: $(TF_NEW_GOLD)
	# Now please manualy add new gold files to git repo. Thanks.



#
#  CLEANUP
#
TMP_BAKUPS=$(wildcard */*~) $(wildcard *~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)

clean: clean_local  clean_win  clean_obj  clean_test

clean_local:
	rm -R -f $(EXEC_GUI) $(EXEC_CLI)  lib_fsqlf/formatter/lex.yy.c  $(TMP_BAKUPS) \
		lib_fsqlf/formatter/lex.yy.h \
		$(LIBNAME) libfsqlf.a \
		$(wildcard $(PROJECTFOLDER)*.zip) tmp gui/license_text.h $(CONF_FILE)
	make clean_obj

clean_win:
	make clean_local WIN=1

clean_obj:
	rm -f *.o lib_fsqlf/*.o lib_fsqlf/*/*.o utils/*/*.o cli/*.o

clean_test:
	rm -f $(TF_ACTUAL_RESULTS)



#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: lib_fsqlf/kw/kw_defaults.def $(EXEC_CLI)
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

prep_bin:   $(EXEC_CLI) $(EXEC_GUI) formatting.conf
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
