@ECHO OFF
clang++ -std=c++20 -emit-ast ./sources/original.cpp -o ./dumps/original.ast
clang++ -std=c++20 -emit-ast ./sources/s1a.cpp      -o ./dumps/s1a.ast     
clang++ -std=c++20 -emit-ast ./sources/s1b.cpp      -o ./dumps/s1b.ast     
clang++ -std=c++20 -emit-ast ./sources/s1c.cpp      -o ./dumps/s1c.ast     
clang++ -std=c++20 -emit-ast ./sources/s2a.cpp      -o ./dumps/s2a.ast     
clang++ -std=c++20 -emit-ast ./sources/s2b.cpp      -o ./dumps/s2b.ast     
clang++ -std=c++20 -emit-ast ./sources/s2c.cpp      -o ./dumps/s2c.ast     
clang++ -std=c++20 -emit-ast ./sources/s2d.cpp      -o ./dumps/s2d.ast     
clang++ -std=c++20 -emit-ast ./sources/s3a.cpp      -o ./dumps/s3a.ast     
clang++ -std=c++20 -emit-ast ./sources/s3b.cpp      -o ./dumps/s3b.ast     
clang++ -std=c++20 -emit-ast ./sources/s3c.cpp      -o ./dumps/s3c.ast     
clang++ -std=c++20 -emit-ast ./sources/s3d.cpp      -o ./dumps/s3d.ast     
clang++ -std=c++20 -emit-ast ./sources/s3e.cpp      -o ./dumps/s3e.ast     
clang++ -std=c++20 -emit-ast ./sources/s4a.cpp      -o ./dumps/s4a.ast     
clang++ -std=c++20 -emit-ast ./sources/s4b.cpp      -o ./dumps/s4b.ast     
clang++ -std=c++20 -emit-ast ./sources/s4c.cpp      -o ./dumps/s4c.ast     
clang++ -std=c++20 -emit-ast ./sources/s4d.cpp      -o ./dumps/s4d.ast     
ECHO all files were dumped in the dumps directory
PAUSE