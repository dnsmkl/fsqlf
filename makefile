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
# -mthreads needs to be removed, so mingwm10.dll would not be needed (http://old.nabble.com/mingwm10.dll-ts8920679.html)
else
OS_TARGET=linux
EXEC_CLI=fsqlf
EXEC_GUI=wx_fsqlf
CC=gcc
CCFLAGS=-m32 -Wall
CXX=g++
CXXFLAGS+= `wx-config --cxxflags`
LDFLAGS+= `wx-config --libs`
endif



.PHONY: all  clean  zip  test  test-print  test-compare



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

$(CXXOBJ): %.o: gui/%.cpp
	$(CXX)  -c $<  -o $@  $(CXXFLAGS)

wx_fsqlf.o: gui/wx_fsqlf.cpp  gui/wx_fsqlf.hpp  gui/basic_notepad.hpp
basic_notepad.o: gui/basic_notepad.cpp  gui/basic_notepad.hpp  gui/dnd_target.hpp  gui/license_text.h
dnd_target.o: gui/dnd_target.cpp  gui/dnd_target.hpp

gui/license_text.h: LICENSE
	tools/text_to_header.sh  $<  $@



#
# TESTING
#
TEST_SAMPLE=testing/sample_main.sql
TEST_TMP_ORIGINAL=testing/tmp_test_original.txt
TEST_TMP_FORMATED=testing/tmp_test_formated.txt

test: test-print  test-compare
testgold: testing/sample_main_output_lead.sql testing/sample_main_output_gold.sql
	diff $+

testing/sample_main_output_lead.sql: testing/sample_main.sql $(EXEC_CLI)
	./fsqlf $< $@

test-print: $(EXEC_CLI)
	./$(EXEC_CLI) $(TEST_SAMPLE) |  awk -F, '{ printf("%4d # ", NR) ; print}'

test-compare: $(EXEC_CLI)  $(TEST_TMP_ORIGINAL)  $(TEST_TMP_FORMATED)
	diff -i -E -b -w -B -q $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)

$(TEST_TMP_ORIGINAL):
	cat        $(TEST_SAMPLE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_ORIGINAL)

$(TEST_TMP_FORMATED):
	./$(EXEC_CLI) $(TEST_SAMPLE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_FORMATED)



#
#  CLEANUP
#
TMP_BAKUPS=$(wildcard */*~) $(wildcard *~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)

clean: clean_local  clean_win

clean_local:
	rm -R -f $(EXEC_GUI) $(EXEC_CLI)  $(LEX_OUTPUT)  $(TMP_BAKUPS)  $(wildcard $(PROJECTFOLDER)*.zip) tmp gui/license_text.h $(CONF_FILE)
	rm -f *.o

clean_win:
	make clean_local WIN=1



#
# BUILD ARCHIVE  (source and binaries for publishing)
#
formatting.conf: core/t_kw_settings_list.def core/create_conf_file.h
	./$(EXEC_CLI) --create-config-file

VERSION:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(VERSION).zip

zip: tmp_folder formatting.conf
	rm -f $(ZIP_NAME)
	git archive master  -o $(ZIP_NAME)  --format=zip --prefix='$(PROJECTFOLDER)/source/'
	cd tmp/ &&   zip -r ../$(ZIP_NAME)  $(PROJECTFOLDER)

tmp_folder: LICENSE README.md
	make prep_bin
	make clean
	make prep_bin WIN=1
	cp    -t tmp/$(PROJECTFOLDER)   $^

prep_bin:   $(EXEC_CLI) $(EXEC_GUI) formatting.conf
	mkdir -p tmp/$(PROJECTFOLDER)/$(OS_TARGET)
	cp    -t tmp/$(PROJECTFOLDER)/$(OS_TARGET)    $^



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
