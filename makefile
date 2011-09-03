

#for linux building
LINEXEC=bin/fsqlf

#for windows building
WINEXEC:=$(LINEXEC).exe

GUI_SRC=gui/wx_fsqlf.cpp
GUI_EXEC=bin/wx_fsqlf
GUI_WINEXEC=$(GUI_EXEC).exe

EXECUTABLES=$(WINEXEC) $(LINEXEC) $(GUI_EXEC) $(GUI_WINEXEC)


PROJECTFOLDER=fsqlf
SRC=core/fsqlf.lex
HEADERS=$(wildcard core/*.h core/*.def)

# where all executables will be put
BIN_FOLDER=bin/

# needed for building archive
NAME:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(NAME).zip

LEX_OUTPUT=core/lex.yy.c




#executables
$(WINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	i586-mingw32msvc-gcc   $<   -o $@
	strip $@

$(LINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	gcc  $<   -o $@
	strip $@

$(GUI_EXEC):    $(GUI_SRC) | $(BIN_FOLDER) $(LINEXEC)
	g++   $<   -o $@   `wx-config --cxxflags`   `wx-config --libs`
	strip $@

$(GUI_WINEXEC): $(GUI_SRC) | $(BIN_FOLDER) $(WINEXEC)
	i586-mingw32msvc-g++  $<  -o $(GUI_WINEXEC) \
		`/usr/i586-mingw32msvc/bin/wx-config --libs     | sed 's/-mthreads//'` \
		`/usr/i586-mingw32msvc/bin/wx-config --cxxflags | sed 's/-mthreads//'`
	strip $@
# -mthreads needs to be removed , so mingwm10.dll would not be needed
# http://old.nabble.com/mingwm10.dll-ts8920679.html




#some prerequisites
$(LEX_OUTPUT): $(SRC) $(HEADERS)
	flex   -o $@   $<

$(BIN_FOLDER):
	mkdir -p $(BIN_FOLDER)


#archive for publishing
$(ZIP_NAME): $(SRC) $(HEADERS) $(EXECUTABLES) LICENSE README
	git archive master --prefix='$(PROJECTFOLDER)/source/' --format=zip -o $@
	cd .. && zip -q $(PROJECTFOLDER)/$@  	\
		$(PROJECTFOLDER)/LICENSE	\
		$(PROJECTFOLDER)/README 	\
		$(foreach  exec,$(EXECUTABLES),$(PROJECTFOLDER)/$(exec))





#ponies!
.PHONY: test clean zip

TMP_BAKUPS=$(wildcard *~) $(wildcard core/*~) $(wildcard gui/*~) $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)
clean:
	rm -f $(EXECUTABLES)  $(LEX_OUTPUT)  $(TMP_BAKUPS)  $(wildcard $(PROJECTFOLDER)*.zip)


TESTFILE=test_text.sql
TEST_TMP_ORIGINAL=tmp_test_original.txt
TEST_TMP_FORMATED=tmp_test_formated.txt
test:$(LINEXEC)
	# Print formated output
	#-------------------- Start of formated SQL --------------------#
	./$(LINEXEC) $(TESTFILE) |  awk -F, '{ printf("%4d # ", NR) ; print}'
	#
	#--------------------- End of formated SQL ---------------------#
	# Test if the output is equivalent to the input (except for spaces, tabs and new lines)
	cat        $(TESTFILE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_ORIGINAL);
	$(LINEXEC) $(TESTFILE) |  tr '\n' ' ' | sed 's/[\t ]//g' | sed 's/outer//gi' | sed 's/inner//gi' > $(TEST_TMP_FORMATED)
	diff -i -E -b -w -B -q $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)
	rm $(TEST_TMP_ORIGINAL) $(TEST_TMP_FORMATED)




zip:$(ZIP_NAME)

all:$(EXECUTABLES)
