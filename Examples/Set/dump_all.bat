@ECHO OFF
clang -std=c11 -emit-ast ./sources/set.c 		-o 		./dumps/set.ast			
clang -std=c11 -emit-ast ./sources/set_tests.c 	-o 		./dumps/set_tests.ast	
clang -std=c11 -emit-ast ./sources/main.c 		-o 		./dumps/main.ast		
   
ECHO all files were dumped in the dumps directory
PAUSE