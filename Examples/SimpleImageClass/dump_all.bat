@ECHO OFF
clang++ -std=c++20 -emit-ast 	./sources/Boundaries.cpp		-o		./dumps/Boundaries.ast		
clang++ -std=c++20 -emit-ast 	./sources/Image.cpp 			-o 		./dumps/Image.ast			
clang++ -std=c++20 -emit-ast 	./sources/Range.cpp 			-o 		./dumps/Range.ast 			
clang++ -std=c++20 -emit-ast	./sources/SharedPixels.cpp 		-o 		./dumps/SharedPixels.ast	
   
ECHO all files were dumped in the dumps directory
PAUSE

