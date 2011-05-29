
#for windows building
WINCC=i586-mingw32msvc-gcc
WINEXEC=fsqlf.exe

#for linux building
CC=gcc
LINEXEC=fsqlf


EXECUTABLES=$(WINEXEC) $(LINEXEC)

PROJECTFOLDER=fsqlf
SRC=fsqlf.l
HEADERS=$(wildcard *.h)


# needed for building archive
NAME:=$(shell git describe master)
ZIP_NAME:=$(PROJECTFOLDER).$(NAME).zip



# For cleaning up. These are produced by some editors.
TMP_BACKUPS=$(wildcard *~)





$(WINEXEC):lex.yy.c 
	$(WINCC) $^ -o $@
	strip $@

$(LINEXEC):lex.yy.c 
	$(CC) $^ -o $@
	strip $@

lex.yy.c: $(SRC) $(HEADERS)
	flex $(SRC)



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
