# JScheme
 --------------------------------------------------------------------------------------
*Assignment for the lecture "Design und Implementation fortgeschrittener Programmiersprachen" at Media University Stuttgart - summer term 2016*
 --------------------------------------------------------------------------------------



##### (JS (shortFor JScheme (shortFor JojoScheme (shortFor JohannesScheme)))) 

JS is a simple scheme implementation written in C. The name was chosen intentionally and may be confused with scheme interpreters written in Java. This is not the case. Only C and scheme code here.


## Getting started

1. Install

	```
	git clone git@github.com:JohannesTheo/JScheme.git
	cd ./JScheme
	make js
	```

2. Run
	```
	./jscheme
	```

## Features

* Interactive AST Interpreter with REPL
* JS supports closures and lexical scopes via inner defines/lambdas
* JS supports Continuation-passing style (CPS) with Tail calls
	* switch between CPS and non CPS-mode interactively
* JS supports overflow detection for numbers
* JS supports colored output if available
* JS has a small checkers game demo

> when compiled with DEBUG ( which is default so far )

* JS supports a nice list of debug options

## Debug options

In JS type `%XY` to toggle an option or print help

* `%` -> print debug option list
* `%CP` -> turn CPS on/off
* `%DT` -> print detailed types of jscheme atoms
* `%ET` -> print a colored eval trace ( works only in non CPS mode )
* `%PS` -> print stack frames of CPS (works only in CPS mode)
* `%PI` -> print detailed info when using `(include "someFile.scm")`

## JScheme demo

#### A checkers game written in JScheme
This is my helloWorld scheme program using the minimal functionality of JScheme :)

* run with `(include "checkers.scm")` in JS

```
NEW GAME STARTED
------------------------------
| Round: 1 white: 12 red: 12 | WHITEs move
| Moves left before draw: 40 |
------------------------------

 col 1  2  3  4  5  6  7  8
row -------------------------
 A |    rr    rr    rr    rr | A
 B | rr    rr    rr    rr    | B
 C |    rr    rr    rr    rr | C
 D | ..    ..    ..    ..    | D
 E |    ..    ..    ..    .. | E
 F | ww    ww    ww    ww    | F
 G |    ww    ww    ww    ww | G
 H | ww    ww    ww    ww    | H
    -------------------------
     1  2  3  4  5  6  7  8

 JS> (move F1 E2)
```

*TO-DO: Enforce jumps, detect multi-jumps, implement a cool AI in JScheme.*

#### A stupid tail call infinity loop demo

* run with `(include "tailcallDemo.scm")` in JS

<br>
# JScheme DOCS

For more detailed examples please have a look at the "checkers.scm" demo or "init.scm"!

### Atoms 
* `nil`
* `#t`
* `#f`
* `#void`
* `numbers` ( integers only atm )
* `strings`
* `symbols`
* `lists`
* `functions`

### Builtin functions
* `+` plus
* `-` minus
* `*` times
* `/` quotient
* `>` greater than
* `eq?` compare js objects
* `cons?` is a list?
* `car` get the first element of a list
* `cdr` get the rest of a list (exclusive the first element)
* `cons` make a new list
* `set-car!` set the first element of a list
* `set-cdr!` set the rest of a list
* `display` print for user output ( strings without quotes )
* `write` print to read back ( strings with quotes )
* `string=?` compare strings
* `include` a file ( read in, evaluate and give control back to interactive REPL)

	```
	JS> (include "some File.scm")
	#void
	```

### Builtin syntax
(All the syntax is working in CPS and non-CPS mode)


* `define` variables or functions
	```
	JS> (define a 100) 
	#void
	JS> (define (f x y z) (+ x y z))
	#void
	JS> (f 1 2 a)
	103
	```

* `lambda` anonymous functions
	```
	JS> ((lambda (x y)(* x y)) 3 3)
	9
	```

* `if` conditional if else
	```
	JS> (define condition #t)
	#void
	JS> (define ifExpr "condition was true")
	#void
	JS> (define elseExpr "condition was false")
	#void
	JS> (if condition ifExpr elseExpr)
	"condition was true"
	```

* `quote` returns the unevaluated expr.
	```
	JS> (quote '(1 2 3 a))
	(1 2 3 a)
	```

* `set!` set a variable to a new value
	```
	JS> (define a "helloWorld")
	#void
	JS> (set! a 128)
	#void
	JS> ((lambda () (set! a "hello environment scopes")))
	#void
	JS> a 
	"hello environment scopes"
	```

### init.scm

Load with `(include "init.scm")` in JS

* `not` invert bool
* `<` less than 
* `=` compare numbers
* `!=` not equal
* `>=` greater than or equal
* `<=` less than or equal
* `zero?` 
* `negative?`
* `positive?`
* `abs` absolute value of a number
* `even?` is number even or odd
* `nth` get the nth element of a list
* `set_nth` set the nth element of a list
* `indexOfS?` search for a specific string in a list and get its index if found
* `newline` print a new line
* `error` print a message and abort

## TO-DOs

* REFACTOR and clean stuff in general!
* ADD more inline documentation!
* ADD more builtins in general but especially for list operations such as *list* etc.
* ADD more atoms chars, floats, vectors, *infinite* big numbers
* ADD more I/O support
* ADD support for variable number of arguments in functions
* PIMP the reader in general
* ADD a garbage collector
* ADD a BYTECODE compiler
* go on holidays first :)

