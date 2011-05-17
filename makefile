CC=gcc
#to build for windows use : CC=i586-mingw32msvc-gcc

EXEC=fsqlf
SRC=fsqlf.l
INCLUDES=print_keywords.h settings.h debuging.h global_variables.h



test:fsqlf
	cat test_text.sql | ./$(EXEC)

$(EXEC):lex.yy.c 
	$(CC) lex.yy.c -lfl  -o $(EXEC)
	strip $(EXEC)
lex.yy.c: $(SRC) $(INCLUDES)
	flex $(SRC)

clean:
	rm -f lex.yy.c 
	rm -f $(EXEC)

