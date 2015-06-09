#!/bin/bash
#Script for rebuildung
#Make sure to run
#	$chmod +x makeShell.sh
#before using the script(makes the file executable)

#Start script
echo "########################################"
echo "Build shell begins"
echo "########################################"

#Create lex.yy.c
flex "smpsh_scanner.l"
echo "Build new Lexer"

#Compile lex.yy.c
gcc -c lex.yy.c
echo "Compiled LEXER"

#Build and bind files
#gcc helper.o
#echo "Bind CI"
gcc -g -Wall -o smpsh smpsh_base.c helper.h helper.c commands.h commands.c signals.h signals.c lex.yy.o -lfl
echo "Build and bind"

#start shell
echo "start smpsh via valgrind"
#valgrind --leak-check=full --log-file=logfile.txt
./smpsh