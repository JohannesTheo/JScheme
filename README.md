# JScheme

*Assignment for the lecture "Design und Implementation fortgeschrittener Programmiersprachen" at Media University Stuttgart - summer term 2016*

## (JS (shortFor JScheme (shortFor JojoScheme (shortFor JohannesScheme)))) 

JS is a simple scheme implementation written in C. The name was chosen intentionally and may be confused with scheme interpreters written in Java. This is not the case. Only C and scheme code here.


### install

git clone git@github.com:JohannesTheo/JScheme.git
cd ./JScheme
make js

### run
./jscheme

### (fancy) features:

JS supports Continuation-passing style (CPS) with Tail calls.
JS supports overflow detection for numbers
JS supports colored output if available.

When compiled with DEBUG ( which is default so far )

JS supports a list of debug options:

%   -> print debug option list
%CP -> turn CPS on/off
%DT -> print detailed types of jscheme atoms
%ET -> print a colored eval trace ( works only in non CPS mode )
%PS -> print stack frames of CPS (works only in CPS mode)
%PI -> print detailed info when using (include "someFile.scm")

### demo

##### a checkers game written in JScheme
My helloWorld scheme program using the minimal functionality of JScheme :)

In JS run: (include "checkers.scm")

TO-DO: Enforce jumps, detect multi-jumps, implement a cool AI in JScheme.

#### a stupid tail call infinity loop demo
In JS run: (include "tailcallDemo.scm")

## JScheme DOCS

### Atoms 
nil
#t
#f
#void
numbers ( no floats atm )
strings
symbols
lists
functions

### BuiltinFunctions
+
-
*
/
>
eq?
cons?
car
cdr
cons
set-car!
set-cdr!
display
write
string=?
include

### BuiltinSyntax
(All syntax working in CPS and non-CPS mode)

define
lambda
if
quote
set!






