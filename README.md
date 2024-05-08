# CloneDetection

## What Is It?
This is a console application that can find repeated fragments (particularly - functions and methods) in C++ source code, so-called **_code clones_**. The algorithm is independent from code entities' names and can find even **near-clones** (slightly modified fragments, for example: two rows with variables declarations can be swapped, variables can be renamed or *for* statement replaced with *while*). Such **clones** usually occur as a result of copy-pasting of some working code, maybe slightly changing it. In programmers circles that approach is considered as extremely harmful, since it is heavily maintainable and might lead to vexing bugs. 

## Algorithm
Approach, in general, based on the count matrix method (article: ["CMCD: Count Matrix based Code Clone Detection"](https://ieeexplore.ieee.org/abstract/document/6130694), Yuan, Y. and Guo, Y., 2011, December. CMCD: Count matrix based code clone detection. In 2011 18th Asia-Pacific Software Engineering Conference (pp. 250-257). IEEE.).
As authors did, we also evaluate the algorithm on 16 clone scenarios from primitive to complicated. Scenarios are proposed on page 16 in the [article by Roy et al.](https://www.sciencedirect.com/science/article/pii/S0167642309000367) (Roy, C.K., Cordy, J.R. and Koschke, R., 2009. Comparison and evaluation of code clone detection techniques and tools: A qualitative approach. Science of computer programming, 74(7), pp.470-495.).

Clone detection consists of two stages. The first stage is variables vectorization and the second is matching similar vectors and calculating functions similarity rate.

Every variable in the function/method can be represented by **count vector** - vector of numbers characterizing usage of that variable in the function (how many times used inside function, how defined, how many times changed and so on). Every function or method can be represented by **count matrix** - set of all its variables' *count vectors*.

*The distance between two count vectors* is ordinary Euclidean distance. Then it normilized in \[0,1\] range (since variables that was used more times likely to differ more), substructed from 1 and that way we can determine the similarity between two variables. If variables are absolutely identical - they have equal vectors, distance is zero and similarity is one (100%). If variables differ significantly, then normalized distance is close to 1 and similarity is almost 0.

To *compare two functions* the *bipartite graph* is built: the *vertices* in the every part is the *variables* corresponding to each function and the *weight of edge* between any two variables is their *similarity*. The part which less is augmented by zero-count-vectors. Then the *maximum bipartite matching problem* is solved and the **functions similarity** is calculated as **mean similarity in variables matching**. Then that value is compared with **functions similarity threshold** and if *functions similarity* more than that hyperparameter or equal then functions are declared **clones**.

## Counted Variables Usage Conditions
Values 1-4 are binary values (i.e. 0 or 1):
1.	Is the variable a local?
2.	Is the variable a member field?
3.	Is the variable a parameter of the function?
4.	Is the variable a global?
Remained values represent the count the variable used in the particular scenario.
5.	Changed by operator (addition-/subtraction-/multiplication-/division-/modulus-assignment, increment/decrement)
6.	Used inside call expression (i.e. passed as an argument to any function inside the analyzed one)
7.	Used with square brackets (array-subscript)
8.	Used inside square brackets (used for calculating index of an element in array)
9.	Used for addition or subtraction (addition/subtraction, addition-/subtraction-assignment, increment, decrement)
10.	Used for multiplication or division (multiplication/division, multiplication-/division-assignment, unary plus/minus)
11.	Used for modulus (modulus, modulus-assignment)
12.	Used for comparison (less, greater, less or equal, greater or equal, equal, not equal)
13.	Used inside conditional statement (if / switch / ternary operator ?:)
14.	Used inside loop (while / for / for range / do while)
15.	Defined with call expression
16.	Defined with addition or subtraction
17.	Defined with multiplication or division
18.	Defined with modulus
19.	Defined with comparison
20.	Defined with literals

Conditions 9-12 and 16-19 are analyzed through finding the corresponding operators (built in functions or implemented methods).

## Dependencies
1. For solving *maximum bipartite matching problem* the [**dlib** library](http://dlib.net/) is used ([used method](http://dlib.net/dlib/optimization/max_cost_assignment_abstract.h.html#max_cost_assignment)). It is compiled as stand-alone project.
2. For analysing usage of variables the *AST (Abstract Syntax Tree)* is built with the library by *clang*: [**libclang**](https://clang.llvm.org/doxygen/group__CINDEX.html) - dynamic library with C-API.
3. Application works with prepared **.ast** files, that *clang* can build.

To create **.ast** files you have to install appropriate **LLVM**-version from their [Github](https://github.com/llvm/llvm-project/releases/) (for me it was LLVM-18.1.3-win64.exe, previous versions can be not supported by our application) and add *LLVM* to system path (more detailed about installation see [here](https://metanit.com/cpp/tutorial/1.8.php)).
Dependency from *libclang* was set already in the VS Projects' Properties, so all you need is copy folder with installed LLVM to Solution directory and delete all besides *LLVM\lib\libclang* and *LLVM\include* (see example in *LLVM\* directory in the repository).

The project was built and tested with Visual Studio 2022 Community edition in Debug x64 and Release x64 modes. In order to build it for x32 platform you have to download the appropriate LLVM version.

## User Guide
Two applications are provided: 
- for ***.ast** files dumping* from sources
- for *analysing* these **.ast** files *for clones*.

Both applications are *launched through cmd* and accept the path to *config file*, that is passed to application as the only command line argument. Config examples for both apps can be found in the *Examples\\* directory. All supported config parameters are described below.

### **.ast files dumping**
   - Config

     Config for ast dumping is the file with .txt extension. Each row represents a parameter need to be set, entered as

     **\<ParamName\> ParamValue**

     Several *ParamValues* can be set after single *\<ParamName\>*, then they has to be separated with spaces. If *ParamValue* has multiple words (contains white spaces), but must be treated as a single value (for example, any path with spaces), then it must be **enslosed in double quotes ""**.

     Supported parameters (the order does not matter; **\<ParamName<sup>1!</sup>\>** means that parameter can not be entered multiple times; **path\\<sup>me!</sup>** means that path must exist; **\<ParamName<sup>mult</sup>\>** means that parameter can be set multiple times, then all its values will be saved):
     - **\<DumpsDirectory<sup>1!</sup>\> path\\to\\dir\\** - directory where all *.ast files* will be saved. If directory does not exist, it will be created
     - **\<IncludeDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory, that will be passed to clang-dumper **for all source files** as the **-I** argument
     - **\<IncludeDirectoryR<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory and all its nested directories (*R* means recursive) will be passed to clang-dumper **for all source files** as the **-I** argument
     - **\<LibsDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory, that will be passed to clang-dumper **for all source files** as the **-L** argument
     - **\<LibsDirectoryR<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory and all its nested directories (*R* means recursive) will be passed to clang-dumper **for all source files** as the **-L** argument
     - **\<Lib<sup>mult</sup>\> libname** - add library that will be passed to clang-dumper **for all source files** as the **-l** argument
     - **\<CommonArgs<sup>mult</sup>\> common args separated with spaces** - options that will be passed to clang-dumper **for all source files**
     - **\<SourcesDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory, where source files (**.c and .cpp files**) for dumping will be searched
     - **\<SourcesDirectoryR<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - in this directory and in all its nested directories source files (**.c and .cpp files**) for dumping will be searched
     - **\<Source<sup>mult</sup>\> path\\to\\source\\file\\<sup>me!</sup> specifig arguments separated with spaces** - path to source file (**.c and .cpp files**) for dumping and its specifig clang options

      All clang options can be found [here](https://clang.llvm.org/docs/ClangCommandLineReference.html)

  - Result

      All **.c** and **.cpp** dumps are saved in the **\<DumpsDirectory\>** with the same filename, but extension replaced from **.c/.cpp** to **.ast**

### **clone detection**
   - Config

     Config for clone detection is the file with .txt extension. Each row represents a parameter need to be set, entered as

     **\<ParamName\> ParamValue**

     Only one *ParamValue* allowed with a single *\<ParamName\>* (in one row, not in the all config). If *ParamValue* has multiple words (contains white spaces), but must be treated as a single value (for example, any path with spaces), then it must be **enslosed in double quotes ""**.
     
     Clones are searched in two projects as if they were analysed for plagiarism. I.e. the application finds dublicates between two projects. If one intends to find duplicates within a single project, he has to pass the same project twice. Functions with the same location are not saved in the results so the functions will not be claimed the clones of themselves. Such a strategy also prevents claiming different instantiations of the same template to be clones. Nevertheless, overloads may be detected as clones.

     Supported parameters (**\<ParamName<sup>om!</sup>\>** means that the order matters for this parameter: if this \<ParamName\> is encountered several times, the latest value will be set; **path\\<sup>me!</sup>** means that path must exist; **\<ParamName<sup>mult</sup>\>** means that parameter can be set multiple times, then all its values will be saved):
     - **\<Output<sup>om!</sup>\> output** - how to save the results. Now only saving into .txt file is supported, so the *output* must be path\\for\\results\\filename.txt. If directory for results does not exist, it will be created
     - **\<Framework<sup>om!</sup>\> name** - the name of framework to use. Now only **.c** and **.cpp** files with the clang framework are supported, so the only allowed value is **clang** (which is set by default if this param is not provided in the config)
     - **\<FirstProjectDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - directory with the files from the first project that will be analysed for clone codes (**.ast** files for **clang-framework**)
     - **\<SecondProjectDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - the same as \<FirstProjectDirectory\>, but for the second project
     - **\<FirstProjectDirectoryR<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - the same as \<FirstProjectDirectory\>, but find files not only in this particular directory, but also in the all nested
     - **\<SecondProjectDirectoryR<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - the same as \<FirstProjectDirectoryR\>, but for the second project
     - **\<ExcludedDirectory<sup>mult</sup>\> path\\to\\dir\\<sup>me!</sup>** - if the function definition located in file, that located inside this directory, then it won't be saved in the results
     - **\<ExcludedSource<sup>mult</sup>\> path\\to\\source\\filename<sup>me!</sup>** - if the function definition located in this file, then it won't be saved in the results
     - **\<MinSimilarity<sup>om!</sup>\> float value** - if two functions have similarity greater or equal to this value, they are considered to be clones. In the current implementation values from 0 to 1 are treated as a fraction and converted into the percentages. Values greater than 1 are treated as the percentages. Default value is 0. Suggested value is 0.6.
     - **\<MinVariables<sup>om!</sup>\> integer value** - if one function in pair has less variables than this value, then the pair is not saved even if they are clones. It can be helpful for exxcluding functions such getters, setters and so on. Default value is 0. Suggested value is 4.
     - **\<CodeAnalysisThreads<sup>om!</sup>\> integer value** - functions implementations analysis is parallelized. You can manage the count of threads involved for that. Nevertheless, this stage is much faster than the comparing stage, so it does not impact the total execution time significantly. Default value is 1. Suggested value is 1.
     - **\<CountMatricesComparingThreads<sup>om!</sup>\> integer value** - count matrices, built based on the functons implementations, can be compared in parallel. You can manage the count of threads involved for that. Default value is 1. Suggested value is 2.
     - **\<MatricesComparingAlgorithm<sup>om!</sup>\> algorithm name** - the name of algorithm for count matrices comparing. Only **BipartiteMatching** (which is set by default if this param is not provided in the config) is supported now.
     
     Other parameters are additional and dependend on the previous choices. Current supported additional parameters:
     - **\<CountVectorMetrics<sup>om!</sup>\> metrics name** - the name of metrics between count vectors. Only **NormilizedEuclideanDistance** (which is set by default if this param is not provided in the config) is supported now.
     - **\<BipartiteMatchingAlgorithm<sup>om!</sup>\> metrics name** - the name of algorithm for matching variables based on their count vectors. Only **MaxTotalWeight** (which is set by default if this param is not provided in the config) is supported now.
     - **\<SimilarityEstimator<sup>om!</sup>\> metrics name** - the name of method for final functions' similarity calculation based on the variables' similarity. Only **MeanWeight** (which is set by default if this param is not provided in the config) is supported now.
    
  - Results

      Results are saved in the **\<Output\>** file. The results contain:
    - \<MinSimilarity\> and \<MinVariables\> parameters that were used
    - How many functions were analysed
    - How many functions were excluded
    - How many clones were detected
    - List of clones sorted in the descending similarity order. For each pair of clones their similarity provided and similarity of all its matched variables.
    - Execution time for each stage

- ## Examples
All examples can be found in the *Examples\\* directory. *ast dumping* examples located in the *Examples\\ast_dumping\\* directory and clone detection examples located in the *Examples\\cmcd\\* directory.
They contains:
- configs for both apps
- source files for ast dumping
- .bat scripts for ast dumping. The .ast files themselves are not included in the repository, but can be generated with the corresponding .bat scripts
- .bat scripts for clone detection, that operate on the .ast files, that will be saved in the *Examples\\ast_dumping\\some_project_name\\dumps\\* directories after launching ast-dumping scripts
- results for clone detection app. Some results can contain outdated information or deprecated format

## Used IDE
VisualStudio 2022 Community

## Known Bugs
- All functions of the standart library are excluded from alalysis by default (i.e. they are not even passed with conditions analyser). For excluding other sources (some external libraries that was used in the compared projects), the **\<ExcludedDirectory\>** and **\<ExcludedSource\>** parameters must be set. Current implementation excludes them only at the results saving stage. Nevertheless, they are passed with conditions analyser and involved in pairwise functions comparing. That can lead to long lasting extraneous calculations for projects with a lot of external source files (.h, .hpp, .c, .cpp). Such a behaviour has to be fixed in the future. 

- When clone detection app ends up with the exeption, the .ast files it used may be corrupted. In that case you have to repeat the dumping.