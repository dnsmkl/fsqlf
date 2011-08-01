

#for linux building
CC=gcc
LINEXEC=core/fsqlf

#for windows building
WINCC=i586-mingw32msvc-gcc
WINEXEC:=$(LINEXEC).exe


EXECUTABLES=$(WINEXEC) $(LINEXEC)


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




$(WINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	$(WINCC)   $<   -o $@
	strip $@

$(LINEXEC):$(LEX_OUTPUT) | $(BIN_FOLDER)
	$(CC)   $<   -o $@
	strip $@

$(LEX_OUTPUT): $(SRC) $(HEADERS)
	flex   -o $@   $<

$(BIN_FOLDER):
	mkdir -p $(BIN_FOLDER)

$(ZIP_NAME): $(SRC) $(HEADERS) $(EXECUTABLES) LICENSE README
	git archive master --prefix='$(PROJECTFOLDER)/source/' --format=zip -o $@
	cd .. && zip -q $(PROJECTFOLDER)/$@ $(PROJECTFOLDER)/$(LINEXEC) $(PROJECTFOLDER)/$(WINEXEC) $(PROJECTFOLDER)/LICENSE $(PROJECTFOLDER)/README



#ponies!
.PHONY: test clean zip
clean:
	rm -f lex.yy.c $(EXECUTABLES) $(TMP_BACKUPS) $(PROJECTFOLDER)*.zip

test:$(EXECUTABLES)
	cat test_text.sql | ./$(LINEXEC)

zip:$(ZIP_NAME)

all:$(EXECUTABLES)
