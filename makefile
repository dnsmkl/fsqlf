PROJECTFOLDER=fsqlf

CFLAGS+=-DVERSION=\"$(VERSION)\"
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
CCFLAGS=-m32 -Wall
CXX=g++
CXXFLAGS+= `wx-config --cxxflags`
LDFLAGS+= `wx-config --libs`
endif



.PHONY: all  clean  zip  test  test-print  test-gold  clean_obj  clean_test  install  uninstall



all: $(EXEC_CLI)  $(EXEC_GUI)



#
# BUILD
#
$(EXEC_CLI): core/lex.yy.c
	$(CC) $(CCFLAGS)  $<   -o $@
	strip $@

core/lex.yy.c: core/fsqlf.lex  $(wildcard core/*.h core/*.def)
	flex  -o $@  $< # options (i.e. `-o`) has to be before input file

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
	tools/text_to_header.sh  $<  $@



#
# TESTING
#

# Simple regression testing - testing against gold (pre-saved correct output)
# Given certain input to `fsqlf`, actual output (lead) is compared
# against to it's predefined expected output (gold).
TESTO_EXPECTED = $(wildcard testing/*.output_gold.sql)
TESTO_ACTUAL = $(patsubst %.output_gold.sql,%.output_lead.sql,$(TESTO_EXPECTED))
$(TESTO_ACTUAL):testing/%.output_lead.sql: testing/%.sql | testing/%.output_gold.sql
	./fsqlf $< $@
	diff $@ $|
	rm $@

test-gold: $(EXEC_CLI)  $(TESTO_ACTUAL)

test: test-gold  test-print

# Output for visual inspection
test-print: $(EXEC_CLI)
	./$(EXEC_CLI) testing/bigquery.sql \
	|  awk -F, '{ printf("%4d # ", NR) ; print}'



#
#  CLEANUP
#
TMP_BAKUPS=$(wildcard */*~) $(wildcard *~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)

clean: clean_local  clean_win  clean_obj  clean_test

clean_local:
	rm -R -f $(EXEC_GUI) $(EXEC_CLI)  $(LEX_OUTPUT)  $(TMP_BAKUPS)  \
		$(wildcard $(PROJECTFOLDER)*.zip) tmp gui/license_text.h $(CONF_FILE)

clean_obj:
	rm -f *.o

clean_win:
	make clean_local WIN=1

clean_test:
	rm -f $(TESTO_ACTUAL)



#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: core/t_kw_settings_list.def core/conf_file_create.h
	./$(EXEC_CLI) --create-config-file

VERSION:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(VERSION).zip

zip: tmp_folder formatting.conf
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
