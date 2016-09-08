
(define A2 "A2") (define A4 "A4") (define A6 "A6") (define A8 "A8")
(define B1 "B1") (define B3 "B3") (define B5 "B5") (define B7 "B7")
(define C2 "C2") (define C4 "C4") (define C6 "C6") (define C8 "C8")
(define D1 "D1") (define D3 "D3") (define D5 "D5") (define D7 "D7")
(define E2 "E2") (define E4 "E4") (define E6 "E6") (define E8 "E8")
(define F1 "F1") (define F3 "F3") (define F5 "F5") (define F7 "F7")
(define G2 "G2") (define G4 "G4") (define G6 "G6") (define G8 "G8")
(define H1 "H1") (define H3 "H3") (define H5 "H5") (define H7 "H7")

(define moveTable '("A2" "A4" "A6" "A8"
		    "B1" "B3" "B5" "B7"
		    "C2" "C4" "C6" "C8"
		    "D1" "D3" "D5" "D7"
		    "E2" "E4" "E6" "E8"
		    "F1" "F3" "F5" "F7"
		    "G2" "G4" "G6" "G8"
		    "H1" "H3" "H5" "H7"))

(define (new_move starter)
	(define player starter)
	(define index1 -1)
	(define index2 -1)
	(define isJump #f)
	(define victim -1)
	(define lastMoveWasJump #f)

	(define (reset)
		(set! player "..")
	  	(set! index1 -1)
		(set! index2 -1)
		(set! isJump #f)
		(set! victim -1))

	(define (reset_all)
	  	(reset)
		(set! lastMoveWasJump #f))

	(define (setAndGetMoveState op arg)
	  	(if (eq? op 'player) player
	  	(if (eq? op 'index1) index1
	  	(if (eq? op 'index2) index2
	  	(if (eq? op 'isJump) isJump
	  	(if (eq? op 'victim) victim
	  	(if (eq? op 'lastMoveWasJump) lastMoveWasJump
	  	(if (eq? op 'set_player) (set! player arg)
	  	(if (eq? op 'set_index1) (set! index1 arg)
	  	(if (eq? op 'set_index2) (set! index2 arg)
	  	(if (eq? op 'set_isJump) (set! isJump arg)
	  	(if (eq? op 'set_victim) (set! victim arg)
	  	(if (eq? op 'set_lastMoveWasJump) (set! lastMoveWasJump arg)
	  	(if (eq? op 'reset) (reset)
	  	(if (eq? op 'reset_all) (reset_all)
		(error "invalid operation in move") ))))))))))))))
	)
	setAndGetMoveState
)

(define (move pos1 pos2)
  	(define index1 (indexOfS? pos1 moveTable))
  	(define index2 (indexOfS? pos2 moveTable))
	(if (not (isValidMove index1 index2))
	  	(abortMove)
		(updateGameState))
	(print_board gameState))

(define (abortMove)
	  (display "NOT a valid move, try again...\n\n")
	  (current_move 'reset nil))
	
(define (isValidMove index1 index2)
  	(if (eq? index1 -1) #f
	(if (eq? index2 -1) #f
	(if (not (empty? index2)) #f
	(if (even? (current_game 'round))
	  	(isValidMoveRed index1 index2)
	  	(isValidMoveWhite index1 index2)
	)))))

(define (isValidMoveRed index1 index2)

  	(current_move 'set_player "r")
	(if (not (red? index1)) #f
	(if (king? index1) (tryNormalMoveOrJump index1 index2)
	(if (<= index2 index1) #f
	(tryNormalMoveOrJump index1 index2))))
)

(define (isValidMoveWhite index1 index2)

  	(current_move 'set_player "w")
	(if (not (white? index1)) #f
	(if (king? index1) (tryNormalMoveOrJump index1 index2)
	(if (>= index2 index1) #f
	(tryNormalMoveOrJump index1 index2))))
)


(define (getRowOfIndex index) (+ 1 (/ (- index 1) 4)))

(define (getColumnOfIndex index) 
  	(define row (getRowOfIndex index))
	(define normIndex (- index (* 4 (- row 1))))
	(if (even? row)
		(- (* normIndex 2) 1)
		(* normIndex 2)))

(define (getIndexForRowCol row col)
  	(if (even? row)
	  	(+ (/ (+ col 1)  2) (* 4 (- row 1)))
	  	(+ (/ col 2) (* 4 (- row 1)))))

(define (tryNormalMoveOrJump index1 index2)
	
	(define row1 (getRowOfIndex index1))
	(define row2 (getRowOfIndex index2))
	(define rowDiff (abs (- row2 row1)))

  	(if (> rowDiff 0)
	  	(if (= 1 rowDiff) (tryNormalMove index1 index2)
		(if (= 2 rowDiff) (tryJump index1 index2 row1 row2)
		#f))
	#f)
)


(define (tryNormalMove index1 index2)
	(define col1 (getColumnOfIndex index1))
	(define col2 (getColumnOfIndex index2))
	(define colDif (abs (- col2 col1)))
	
	(current_move 'set_index1 index1)
	(current_move 'set_index2 index2)
	(current_move 'set_isJump #f)
	
	(if (= 1 colDif) #t #f))
	
(define (getIndexOfVictim index1 row1 jumpRight jumpDown)
	
  	(if jumpRight
		(if jumpDown
		  	(if (even? row1)
				(+ index1 4)
				(+ index1 5))
			(if (even? row1)
				(- index1 4)
				(- index1 3)))
		(if jumpDown
		  	(if (even? row1)
				(+ index1 3)
				(+ index1 4))
			(if (even? row1)
				(- index1 5)
				(- index1 4)))
	)
)


(define (tryJump index1 index2 row1 row2)
	(define col1 (getColumnOfIndex index1))
	(define col2 (getColumnOfIndex index2))
	(define colDif (abs (- col2 col1)))
	(define jumpRight (> col2 col1))
	(define jumpDown (> row2 row1))
	(define victim (getIndexOfVictim index1 row1 jumpRight jumpDown))
	(define victim_color (getColorAtIndex victim))
	(define player_color (getColorAtIndex index1))

	(current_move 'set_index1 index1)
	(current_move 'set_index2 index2)
	(current_move 'set_victim victim)
	(current_move 'set_isJump #t)
	
	(if (not (= 2 colDif)) #f
	(if (string=? victim_color player_color) #f
	(if (empty? victim) #f
	#t))))
	  	
