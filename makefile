
#windows specific
WINCC=i586-mingw32msvc-gcc
WINEXEC=fsqlf.exe

#linux specific
CC=gcc
EXEC=fsqlf




SRC=fsqlf.l
HEADERS=$(wildcard *.h)

.PHONY: test clean

all:$(WINEXEC) $(EXEC)


$(WINEXEC):lex.yy.c 
	$(WINCC) lex.yy.c -o $@
	strip $@
	
$(EXEC):lex.yy.c 
	$(CC) lex.yy.c -o $@
	strip $@

lex.yy.c: $(SRC) $(HEADERS)
	flex $(SRC)

#ponies!
clean:
	rm -f lex.yy.c 
	rm -f $(EXEC) $(WINEXEC)

test:fsqlf
	cat test_text.sql | ./$(EXEC)
