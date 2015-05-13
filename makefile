PROJECTFOLDER=fsqlf

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-pedantic-errors
CFLAGS+=-g

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
CFLAGS+=-m32
CXX=g++
CXXFLAGS+= `wx-config --cxxflags`
LDFLAGS+= `wx-config --libs`
endif



.PHONY: all  clean  zip  test  test-print  test-gold  clean_obj  clean_test  install  uninstall



all: $(EXEC_CLI)  $(EXEC_GUI)



#
# BUILD CLI
#
COBJ += core/main.o
COBJ += core/cli.o
COBJ += core/conf_file/conf_file_create.o
COBJ += core/conf_file/conf_file_read.o
COBJ += core/debuging.o
COBJ += core/formatter/globals.o
COBJ += core/formatter/lex.yy.o
COBJ += core/formatter/print_keywords.o
COBJ += core/formatter/tokque.o
COBJ += core/lex/token.o
COBJ += core/kw/kw.o
COBJ += core/kw/kwall_init.o
COBJ += utils/stack/stack.o
COBJ += utils/queue/queue.o
COBJ += utils/string/read_int.o

$(COBJ): %.o: %.c
	$(CC) $(CFLAGS)  -c $<  -o $@

core/conf_file/conf_file_create.o: core/conf_file/conf_file_constants.h
core/conf_file/conf_file_read.o: core/conf_file/conf_file_constants.h utils/string/read_int.h
core/main.o: core/formatter/lex.yy.h

$(EXEC_CLI): $(COBJ)
	$(CC) $(CFLAGS)  $^   -o $@
	# strip $@

core/formatter/lex.yy.h: core/formatter/lex.yy.c
core/formatter/lex.yy.c: core/formatter/fsqlf.lex core/formatter/globals.h core/formatter/print_keywords.h
	# flex options (e.g. `-o`) has to be before input file
	flex  -o $@ --header-file=core/formatter/lex.yy.h $<


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
	rm -R -f $(EXEC_GUI) $(EXEC_CLI)  core/formatter/lex.yy.c  $(TMP_BAKUPS) \
		core/formatter/lex.yy.h \
		$(wildcard $(PROJECTFOLDER)*.zip) tmp gui/license_text.h $(CONF_FILE) \
	make clean_obj

clean_win:
	make clean_local WIN=1

clean_obj:
	rm -f *.o core/*.o core/*/*.o utils/*/*.o

clean_test:
	rm -f $(TF_ACTUAL_RESULTS)



#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: core/kw/kw_defaults.def core/conf_file/conf_file_create.h $(EXEC_CLI)
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

uninstall:
ifdef EXEC_CLI
	rm -vf $(PREFIX)/bin/$(EXEC_CLI)
endif
ifdef EXEC_GUI
	rm -vf $(PREFIX)/bin/$(EXEC_GUI)
endif
	rm -vf $(PREFIX)/share/fsqlf/formatting.conf.example
	rm -vfd $(PREFIX)/share/fsqlf

endif



# makefile reference
# $@ - target
# $+ - all prerequisites
# $^ - all prerequisites, but list each name only once
# $< - first prerequisite
# $? - all prerequisites newer then target
# $| - order only prerequisites
#
# See also:
# http://www.gnu.org/software/make/manual/make.html#Automatic-Variables
