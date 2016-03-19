# Makefile 
# Guoyong Shi, 2014-2015

SRC=\
	main.c \
	parse_func.c \
	Symbol_Table.c \
	MNA_Matrix.c

OBJ=$(SRC:.c=.o)

.c.o:
	gcc -g -c $<

all: parse runparse

parse: parse.lex parse.y
	flex -oparse.yy.c parse.lex
	bison -d parse.y

runparse: $(OBJ)
	gcc -g -o $@ $(OBJ)

clean:
	rm -f *.o *.obj *.exe parse.yy.c parse.tab.h parse.tab.c

