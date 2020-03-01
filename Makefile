all: tc64monitor

lex.yy.c: parser.lex
	flex parser.lex

tc64monitor: lex.yy.c tc64monitor.c
	gcc -static -o tc64monitor lex.yy.c tc64monitor.c -lfl
	mkdir -p ./bin
	cp tc64monitor ./bin
	cp runtc64mon.sh ./bin


.PHONY: clean
clean:
	rm tc64monitor lex.yy.c
	rm -Rf ./bin
