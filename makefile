

#for linux building
LINEXEC=bin/fsqlf

#for windows building
WINEXEC:=$(LINEXEC).exe

GUI_EXEC=bin/gui_wx_basic
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

$(GUI_EXEC):   gui/gui_wx_basic.cpp | $(BIN_FOLDER) $(LINEXEC)
	g++   $<   -o $@   `wx-config --cxxflags`   `wx-config --libs`
	strip $@

$(GUI_WINEXEC):   gui/gui_wx_basic.cpp | $(BIN_FOLDER) $(WINEXEC)
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

TMP_BAKUPS=$(wildcard *~) $(wildcard core/*~) $(wildcard gui/*~)

clean:
	rm -f $(EXECUTABLES)  $(LEX_OUTPUT)  $(TMP_BAKUPS)  $(wildcard $(PROJECTFOLDER)*.zip)


test:$(LINEXEC)
	cat test_text.sql | ./$(LINEXEC)

zip:$(ZIP_NAME)

all:$(EXECUTABLES)
