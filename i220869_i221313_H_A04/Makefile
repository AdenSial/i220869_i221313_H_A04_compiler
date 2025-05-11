# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

all: json2relcsv

# Final binary
json2relcsv: main.o ast.o parser.o scanner.o csv_writer.o
	$(CC) $(CFLAGS) -o $@ main.o ast.o parser.o scanner.o csv_writer.o

# Bison (parser)
parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

parser.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c -o parser.o

# Flex (scanner)
scanner.o: scanner.l parser.tab.h
	flex scanner.l
	$(CC) $(CFLAGS) -c lex.yy.c -o scanner.o

# Other source files
main.o: main.c ast.h csv_writer.h
	$(CC) $(CFLAGS) -c main.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

csv_writer.o: csv_writer.c csv_writer.h ast.h
	$(CC) $(CFLAGS) -c csv_writer.c

# Clean target
clean:
	rm -f *.o *.csv json2relcsv lex.yy.c parser.tab.c parser.tab.h
	rm -f output/*.csv

