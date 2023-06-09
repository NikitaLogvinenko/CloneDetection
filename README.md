# CloneDetection

## What is it?
This is console application that can find repeated fragments (particullarly - functions and methods) in C++ source code, so-called **_code clones_**. The algorithm is independent from code entities' names and can find even **near-clones** (slightly modified fragments, for example: two rows with variables declarations can be swapped, variables can be renamed or *for* statement replaced with *while*). Such **clones** usually occurs as a result of copy-pasting of some working code, maybe slightly changing it. In programmers circles that approach is considered as extremely harmful, since it is hardly maintainable and might lead to vexing bugs. 

## Examples
- TestFiles/Roy_et_al_16_scenarios/sources - different scenarios clones
- TestFiles/Roy_et_al_16_scenarios/results - application performing results
- TestFiles/Roy_et_al_16_scenarios/analyse_all_scenarios.bat - calling application with parameters
- TestFiles/Roy_et_al_16_scenarios/dumps - analysed files
- TestFiles/Roy_et_al_16_scenarios/dump_all.bat - dumping translation units

## Algorithm
Approach, in general, based on the count matrix method (article: ["CMCD: Count Matrix based Code Clone Detection"](https://ieeexplore.ieee.org/abstract/document/6130694), Yuan, Y. and Guo, Y., 2011, December. CMCD: Count matrix based code clone detection. In 2011 18th Asia-Pacific Software Engineering Conference (pp. 250-257). IEEE.).
As authors did, we also evaluate the algorithm on 16 clone scenarios from primitive to really complicated. Scenarios are proposed on page 16 in the article [Roy, C.K., Cordy, J.R. and Koschke, R., 2009. Comparison and evaluation of code clone detection techniques and tools: A qualitative approach. Science of computer programming, 74(7), pp.470-495.](https://www.sciencedirect.com/science/article/pii/S0167642309000367).

Every variable in the function/method can be represented by **count vector** - vector of numbers characterizing usage of that variable in the function (how many times used inside function, how defined, how many times changed and so on). Every function or method can be represented by **count matrix** - set of all its variables' *count vectors*.

*The distance between two count vectors* is ordinary Euclidean distance. Then it normilized in \[0,1\] range (since variables that was used more times likely to differ more), substructed from 1 and that way we can determine the similarity between two variables. If variables are absolutely identical - they have equal vectors, distance is zero and similarity is one (100%). If variables differ significantly, then normalized distance is close to 1 and similarity is almost 0.

To *compare two functions* the *bipartite graph* is built: the *vertices* in the every part is the *variables* corresponding to each function and the *weight of edge* between any two variables is their *similarity*. The part which less is augmented by zero-count-vectors. Then the *maximum bipartite matching problem* is solved and the **functions similarity** is calculated as **mean similarity in variables matching**. Then that value is compared with **functions_similarity_threshold** and if *functions similarity* more than that hyperparameter or equal then functions are declared **clones**.

## Counted variables usage conditions
- `is_param`- 0 or 1
- `is_local_var` - 0 or 1
- `is_field` - 0 or 1
- `is_static_field` - 0 or 1
- `is_global_var` - 0 or 1

- `used_n_times` - counter
- `changed_minimum_n_times` - counter
- `used_inside_call_expr` - counter
- `used_inside_square_brackets` - counter
- `used_with_square_brackets` - counter

- `used_for_sum_or_diff` - counter
- `used_for_multiplication_or_division` - counter
- `used_for_modulus` - counter
- `used_for_comparison` - counter

- `used_inside_conditional_statement` - counter
- `used_inside_loop` - counter

- `defined` - 0 or 1
- `defined_with_call_expr` - counter
- `defined_with_sum_or_difference` - counter
- `defined_with_multiplication_or_division` - counter
- `defined_with_modulus` - counter
- `defined_with_comparison` - counter
- `defined_with_literals` - counter

## Dependencies
1. For solving *maximum bipartite matching problem* the [**dlib** library](http://dlib.net/) is used ([used method](http://dlib.net/dlib/optimization/max_cost_assignment_abstract.h.html#max_cost_assignment)). It is compiled as stand-alone project.
2. For analysing variables usage the *AST (Abstract Syntax Tree)* is built with the library by *clang*: [**libclang**](https://clang.llvm.org/doxygen/group__CINDEX.html#ga51eb9b38c18743bf2d824c6230e61f93) - static library with C-API. It is already downloaded and located in the LLVM folder.
3. Part of input format - set of pathes (passed as commind-line arguments) to the prepared **.ast** files. To create such files you have to install appropriate **LLVM**-version from their [Github](https://github.com/llvm/llvm-project/releases/) (for me it was LLVM-16.0.4-win64.exe) and add *LLVM* to system path (more detailed about instalation see [here](https://metanit.com/cpp/tutorial/1.8.php)). After that for every translation unit that you want to anylise you have to create the *.ast* file the way similar if you want to compile them, but calling **clang -Xclang -ast-dump** with proper arguments (you can see examples [here](https://bastian.rieck.me/blog/posts/2015/baby_steps_libclang_ast/) and all clang commands [here](https://clang.llvm.org/docs/ClangCommandLineReference.html)). You can dump pch files as well as source files and then use them while .cpp files dumping.

## InputOutput format
1. input (through command line arguments) 
- `functions_similarity_threshold` - value from 0 to 1
- path where results will be saved. Path must be free
- pathes to AST files for functions anylis

2. output (nonexistent path passed through first command line argument) - info about *clones-functions*:
- similarity
- functions spelling
- functions locations (file, line, column)
- matched variables and their similarities

## Used IDE
VisualStudio 2022
