(include "init.scm")

(define board_prototype 
" col 1  2  3  4  5  6  7  8
row  -------------------------
 1  |    1     2     3     4  |
 2  | 5     6     7     8     |
 3  |    9     10    11    12 |
 4  | 13    14    15    16    |
 5  |    17    18    19    20 |
 6  | 21    22    23    24    |
 7  |    25    26    27    28 |
 8  | 29    30    31    32    |
     -------------------------")

(define gs '(   "rr"   "rr"   "rr"   "rr" 
		    "rr"   "rr"   "rr"   "rr"
		"rr"   "rr"   "rr"   "rr" 
		    ".."   ".."   ".."   ".." 
		".."    ".."   ".."   ".."
		    "ww"   "ww"   "ww"   "ww" 
		"ww"   "ww"   "ww"   "ww"
		    "ww"   "ww"   "ww"   "ww"   ))

(define indexChars '( "A" "B" "C" "D" "E" "F" "G" "H"))

(define (display_nth n l) (display (nth n l)))

(define (print_board board)
  		(print_board_top)
		(print_row 1 board) 
		(print_row 2 board)
		(print_row 3 board)
		(print_row 4 board)
		(print_row 5 board)
		(print_row 6 board)
		(print_row 7 board)
		(print_row 8 board)
		(print_board_bottom)
		)

(define (print_board_top)
  	(display " col 1  2  3  4  5  6  7  8 \n")
	(display "row -------------------------\n"))

(define (print_board_bottom)
  	(display "    -------------------------\n")
  	(display "     1  2  3  4  5  6  7  8 \n"))

(define (print_row row board)
  	(display " ") (display (nth row indexChars))
	(if (even? row)
	  	(display " | ")
	  	(display " |"))
	(define startIndex (+ 1 (- (* row 4) 4)))
	(define endIndex (* row 4))
	(if (even? row)
	  	(print_even_row startIndex endIndex board)
	  	(print_odd_row startIndex endIndex board))
	(display (nth row indexChars))
	(display "\n"))

(define (print_even_row start end board)
	(display "   ")
	(display_nth start board)
	(display " ")
  	(if (< start end)
		(print_even_row (+ start 1) end board)
		(display "| ")))

(define (print_odd_row start end board)
	(display " ")
	(display_nth start board)
	(display "   ")
  	(if (< start end)
		(print_odd_row (+ start 1) end board)
		(display " | ")))

