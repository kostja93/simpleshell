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

#Compile helper.c
gcc -c helper.c
echo "Compiled CI"

#Compile signals
#gcc -c signal_handling.c
#echo "Compiled signals"

#Compile commands.c
gcc -c commands.c
echo "Compiled commands"

#Create smpsh_scanner.c
#flex -osmpsh_scanner.c smpsh_scanner.l
#echo "Build new smpsh_scanner.c"

#Build and bind files
#gcc helper.o
#echo "Bind CI"
gcc -o smpsh smpsh_base.c helper.o commands.o lex.yy.o -lfl
echo "Build and bind"

#start shell
echo "start smpsh via valgrind"
#valgrind --leak-check=full --log-file=logfile.txt
./smpsh