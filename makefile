test:fsqlf
	cat test_text.sql | ./fsqlf

fsqlf:lex.yy.c
	gcc -o fsqlf lex.yy.c -lfl
	#strip sqlb
lex.yy.c: fsqlf.l print_keywords.h settings.h debuging.h global_variables.h
	flex fsqlf.l

clean:
	rm -f lex.yy.c 
	rm -f fsqlf

