CC = gcc  #49

CFLAGS = -Wall -Wextra -pedantic -std=c99 -Wconversion -m64


%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

# flex:$(flex smpsh_scanner.l)
	
lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

my_os: lex.yy.o commands.o  helper.o smpsh_base.o
	$(CC) -o my_os smpsh_base.o helper.o commands.o lex.yy.o -lfl


