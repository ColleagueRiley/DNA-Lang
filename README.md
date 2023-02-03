# DNA LANG
  Very very simple example DNA based programming language 

# Compiling 
  including the argument `-c` will compile the DNA code, otherwise the code will be run as a script

  including the `-s` argument after the `-c` argument will keep the built c++ file

# base pairs
  ```
  A  T
  G  C
  ```

  Base pairs must be correct or there will be an error\
  Pairs must be seperated by two spaces, as shown

# Functions
  ATG - print\
  ATC - input

# Statements
  GCA - if

# Operator
  TGC - ==   

# Numbers
  AAA - 0\
  GGG - 1

# Arguments 
  if an function requires an argument, the next line of the file will be passed as the argument 

  eg. print

  ```
  A  T
  T  A
  G  C
  OUTPUT
  ```

# How it works
  * Base pairs are checked
  * first nucleobase of the DNA line is converted to RNA
  * RNA is send to cmd queue until queue has 3 RNA nucleobases
  * Function Arguments are collected as needed
  * cmd queue is read and code is run or converted to c++
  * [if compiled] c++ code is compiled
