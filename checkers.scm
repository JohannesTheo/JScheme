(include "init.scm")
(include "gamePrint.scm")
(include "move.scm")
(include "gameState.scm")

(define current_game (new_checkers))
(define current_move (new_move "w"))


(define (welcome)
  	
	(newline) (newline) 
	(display "*****************************************") (newline)
	(display "*                                       *") (newline)
	(display "*     Welcome to the checkers demo      *") (newline)
	(display "*                                       *") (newline)
	(display "*****************************************") (newline))

(define (help)
  	
	(display "*****************************************") (newline)
	(display "*                                       *") (newline)
  	(display "*  PLAY: (move pos1 pos2)               *") (newline)
	(display "*     -> (move F1 E2)                   *") (newline)
	(display "*  or -> (move \"F1\" \"E2\")               *")(newline)
	(display "*                                       *") (newline)
	(display "*  NEW GAME: (new_game)                 *") (newline)
	(display "*  HELP: (help)                         *") (newline) 
	(display "*                                       *") (newline)
	(display "*****************************************") (newline)
	(newline) (newline))

(define (new_game)
	
  	(newline)
	(display "NEW GAME STARTED") (newline)
  	(current_game 'reset)
  	(current_move 'reset_all nil)
	(print_board gameState))

(welcome)
(help)
(new_game)
