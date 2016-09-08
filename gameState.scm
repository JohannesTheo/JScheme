
(define empty "..")
(define white "w")
(define red "r")
(define red_men "rr")
(define red_king "RR")
(define white_men "ww")
(define white_king "MM")

(define original_gs '("rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr"
			".." ".." ".." ".." ".." ".." ".." ".."
			"ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww"))

(define gameState '("rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr"
			".." ".." ".." ".." ".." ".." ".." ".."
			"ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww"))

(define (resetGameState)
	(set! gameState original_gs)
	(set! original_gs '("rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr" "rr"
			".." ".." ".." ".." ".." ".." ".." ".."
			"ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww" "ww")))

(define (new_checkers)
 	(define round 1)
	(define red_pieces 12)
	(define white_pieces 12)
	(define draw_count 0)

	(define (reset)
 		(set! round 1)
		(set! red_pieces 12)
		(set! white_pieces 12)
		(set! draw_count 0)
		(resetGameState)
	)
	
	(define (setAndGetGameState op)
	  	(if (eq? op 'round) round
	  	(if (eq? op 'red) red_pieces
	  	(if (eq? op 'white) white_pieces
	  	(if (eq? op 'draw) draw_count
		(if (eq? op 'round++) (set! round (+ round 1))
		(if (eq? op 'red--) (set! red_pieces (- red_pieces 1))
		(if (eq? op 'white--) (set! white_pieces (- white_pieces 1))
		(if (eq? op 'draw++) (set! draw_count (+ draw_count 1))
		(if (eq? op 'draw_reset) (set! draw_count 0)
		(if (eq? op 'reset) (reset)
		(error "invalid operation in gameState") ))))))))))
	  )
setAndGetGameState)

(define (red? pos)
  	(define pieceAtPos (nth pos gameState))
	(if (string=? pieceAtPos red_men)
	  	#t
		(if (string=? pieceAtPos red_king)
		#t
		#f)))

(define (white? pos)
  	(define pieceAtPos (nth pos gameState))
	(if (string=? pieceAtPos white_men)
	  	#t
		(if (string=? pieceAtPos white_king)
		#t
		#f)))

(define (empty? pos)
  	(define pieceAtPos (nth pos gameState))
	(if (string=? pieceAtPos empty)
	  	#t
		#f))

(define (king? pos)
  	(define pieceAtPos (nth pos gameState))
	(if (string=? pieceAtPos white_king) #t
	(if (string=? pieceAtPos red_king) #t
	#f)))

(define (men? pos)
  	(define pieceAtPos (nth pos gameState))
	(if (string=? pieceAtPos white_men) #t
	(if (string=? pieceAtPos red_men) #t
	#f)))

(define (getColorAtIndex index)
	(if (white? index) white
	(if (red? index) red
	empty)))

(define (updateGameState)

  	(define isJump (current_move 'isJump nil))
  	(define index1 (current_move 'index1 nil))
  	(define index2 (current_move 'index2 nil))
  	(define victim (current_move 'victim nil))
	
	(if isJump
	  	(applyJump index1 index2 victim)
		(applyNormalMove index1 index2))

	(current_game 'round++)
	(checkForWinOrDraw)
)

(define (applyNormalMove index1 index2)
  	
  	(define pieceAt1 (nth index1 gameState))	
	(set_nth index1 gameState empty)
	(set_nth index2 gameState pieceAt1)
	(checkForPromotion index2)
	(current_game 'draw++)	
	(current_move 'set_lastMoveWasJump #f)
)

(define (applyJump index1 index2 victim)
  	
  	(define pieceAt1 (nth index1 gameState))	
	(set_nth index1 gameState empty)
	(set_nth victim gameState empty)
	(set_nth index2 gameState pieceAt1)
	(checkForPromotion index2)

	(if (white? index2) (current_game 'red--))
	(if (red? index2) (current_game 'white--))
	(current_game 'draw_reset)
	(current_move 'set_lastMoveWasJump #t)
)


(define (checkForPromotion index)
  	(if (white? index)
	  	(if (< index 5) (set_nth index gameState white_king)
		)
	(if (red? index)
	  	(if (> index 28) (set_nth index gameState red_king)))))

(define (checkForWinOrDraw)
	
  	(define white_pieces (current_game 'white))
  	(define red_pieces (current_game 'red))
  	(define draw (current_game 'draw))

	(if (>= 0 white_pieces) (display "\n\n!!! RED WINS !!!\n\n")
	  	(if (>= 0 red_pieces) (display "\n\n!!! WHITE WINS !!!\n\n")
		  	(if (> draw 40) (display "\n\n!!! THIS IS A DRAW !!!\n\n")))))

