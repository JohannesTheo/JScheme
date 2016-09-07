
(define original_gs '(   "rr"   "rr"   "rr"   "rr" 
		    "rr"   "rr"   "rr"   "rr"
			"rr"   "rr"   "rr"   "rr" 
		    ".."   ".."   ".."   ".." 
			".."    ".."   ".."   ".."
		    "ww"   "ww"   "ww"   "ww" 
			"ww"   "ww"   "ww"   "ww"
		    "ww"   "ww"   "ww"   "ww"   ))

(define gameState '(   ".."   ".."   ".."   ".." 
		    ".."   "ww"   "rr"   ".."
			".."   "ww"   ".."   ".." 
		    ".."   "rr"   ".."   ".." 
			".."    "ww"   ".."   ".."
		    ".."   ".."   "rr"   ".." 
			".."   "rr"   "ww"   ".."
		    ".."   ".."   ".."   ".."   ))

(define (resetGameState)
	(set! gameState original_gs)
	(set! original_gs '(   "rr"   "rr"   "rr"   "rr" 
		    	    "rr"   "rr"   "rr"   "rr"
				"rr"   "rr"   "rr"   "rr" 
		    	    ".."   ".."   ".."   ".." 
				".."    ".."   ".."   ".."
		    	    "ww"   "ww"   "ww"   "ww" 
				"ww"   "ww"   "ww"   "ww"
		   	     "ww"   "ww"   "ww"   "ww"   )))

(define empty "..")
(define white "w")
(define red "r")
(define red_men "rr")
(define red_king "RR")
(define white_men "ww")
(define white_king "MM")

(define (new_checkers)
 	(define round 1)
	(define red_pieces 4)
	(define white_pieces 4)
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

