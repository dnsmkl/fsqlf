PROJECTFOLDER=fsqlf
SRC=core/fsqlf.lex
HEADERS=$(wildcard core/*.h core/*.def)

# where all executables will be put
BIN_FOLDER=bin/


LEX_OUTPUT=core/lex.yy.c



ifdef WIN
OS_TARGET=windows
EXEC_CLI=fsqlf.exe
EXEC_GUI=wx_fsqlf.exe
CC=i586-mingw32msvc-gcc
CXX=i586-mingw32msvc-g++
CXXFLAGS=`/usr/i586-mingw32msvc/bin/wx-config --libs     | sed 's/-mthreads//'`          `/usr/i586-mingw32msvc/bin/wx-config --cxxflags | sed 's/-mthreads//'`
# -mthreads needs to be removed , so mingwm10.dll would not be needed (http://old.nabble.com/mingwm10.dll-ts8920679.html)
else
OS_TARGET=linux
EXEC_CLI=fsqlf
EXEC_GUI=wx_fsqlf
CC=gcc
CXX=g++
CXXFLAGS=`wx-config --cxxflags`   `wx-config --libs`
endif






.PHONY: all clean zip test test-print test-compare


#some prerequisites
$(LEX_OUTPUT): $(SRC) $(HEADERS)
	flex   -o $@   $<

$(BIN_FOLDER):
	mkdir -p $(BIN_FOLDER)

#  BUILD
all:$(EXEC_CLI) $(EXEC_GUI)
$(EXEC_CLI):$(LEX_OUTPUT)
	$(CC)   $<   -o $@
	strip $@

$(EXEC_GUI):   gui/wx_fsqlf.cpp   |   $(EXEC_CLI)
	$(CXX)   $<   -o $@   $(CXXFLAGS)
	strip $@



#  TESTING
TEST_SAMPLE=testing/sample.sql
TEST_TMP_ORIGINAL=testing/tmp_test_original.txt
TEST_TMP_FORMATED=testing/tmp_test_formated.txt
test: test-print test-compare

test-print:$(EXEC_CLI)
	./$(EXEC_CLI) $(TEST_SAMPLE) |  awk -F, '{ printf("%4d # ", NR) ; print}'

test-compare:$(EXEC_CLI) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)
	diff -i -E -b -w -B -q $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)
$(TEST_TMP_ORIGINAL):
	cat        $(TEST_SAMPLE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_ORIGINAL)
$(TEST_TMP_FORMATED):
	$(EXEC_CLI) $(TEST_SAMPLE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_FORMATED)



#  CLEANUP
TMP_BAKUPS=$(wildcard */*~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)
clean:   clean_local   clean_win
clean_local:
	rm -R -f $(EXEC_GUI) $(EXEC)  $(LEX_OUTPUT)  $(TMP_BAKUPS)  $(wildcard $(PROJECTFOLDER)*.zip) tmp
clean_win:
	make clean_local WIN=1



#  BUILD ARCHIVE  (source and binaries for publishing)
CONF_FILE=formatting.conf
VERSION:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(VERSION).zip
zip: tmp_folder
	rm -f $(ZIP_NAME)
	make $(ZIP_NAME)
	git archive master  -o $(ZIP_NAME)  --format=zip --prefix='$(PROJECTFOLDER)/source/'
	cd tmp/ &&   zip -r ../$(ZIP_NAME)  $(PROJECTFOLDER)

tmp_folder: LICENSE README
	make prep_bin
	make prep_bin WIN=1
	cp    -t tmp/$(PROJECTFOLDER)   $^


prep_bin:   $(EXEC_CLI) $(EXEC_GUI) $(CONF_FILE)
	mkdir -p tmp/$(PROJECTFOLDER)/$(OS_TARGET)
	cp    -t tmp/$(PROJECTFOLDER)/$(OS_TARGET)    $^
