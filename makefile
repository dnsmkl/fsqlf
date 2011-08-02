

#for linux building
LINEXEC=bin/fsqlf

#for windows building
WINEXEC:=$(LINEXEC).exe

GUI_EXEC=bin/gui_wx_basic
GUI_WINEXEC=$(GUI_EXEC).exe

EXECUTABLES=$(WINEXEC) $(LINEXEC) $(GUI_EXEC) $(GUI_WINEXEC)


PROJECTFOLDER=fsqlf
SRC=core/fsqlf.lex
HEADERS=$(wildcard core/*.h)

# where all executables will be put
BIN_FOLDER=bin/

# needed for building archive
NAME:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(NAME).zip

LEX_OUTPUT=core/lex.yy.c

# For cleaning up. These are produced by some editors.
TMP_BACKUPS=$(wildcard *~)




#executables
$(WINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	i586-mingw32msvc-gcc   $<   -o $@
	strip $@

$(LINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	gcc  $<   -o $@
	strip $@

$(GUI_EXEC):   gui/gui_wx_basic.cpp | $(BIN_FOLDER)
	g++   $<   -o $@   `wx-config --cxxflags`   `wx-config --libs`

$(GUI_WINEXEC):   gui/gui_wx_basic.cpp | $(BIN_FOLDER)
	i586-mingw32msvc-g++  $<  -o $(GUI_WINEXEC)  `/usr/i586-mingw32msvc/bin/wx-config --libs | sed 's/-mthreads//'`    `/usr/i586-mingw32msvc/bin/wx-config --cxxflags | sed 's/-mthreads//'`
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
	cd .. && zip -q $(PROJECTFOLDER)/$@ $(PROJECTFOLDER)/$(LINEXEC) $(PROJECTFOLDER)/$(WINEXEC)  $(PROJECTFOLDER)/LICENSE $(PROJECTFOLDER)/README $(PROJECTFOLDER)/$(GUI_EXEC)



#ponies!
.PHONY: test clean zip
clean:
	rm -f lex.yy.c $(EXECUTABLES) $(TMP_BACKUPS) $(PROJECTFOLDER)*.zip

test:$(EXECUTABLES)
	cat test_text.sql | ./$(LINEXEC)

zip:$(ZIP_NAME)

all:$(EXECUTABLES)
