(include "Cprint.scm")

(define empty "..")
(define red_men "rr")
(define red_king "RR")
(define white_men "ww")
(define white_king "WW")

(define move pos1 pos2)
	


(move A2 C3)

(if (isValid? A2 C3)

(define moveTable '("A1" "A3" "A5" "A7"
		    "B2" "B4" "B6" "B8"
		    "C1" "C3" "C5" "C7"
		    "D2" "D4" "D6" "D8"
		    "E1" "E3" "E5" "E7"
		    "F2" "F4" "F6" "F8"
		    "G1" "G3" "G5" "G7"
		    "H2" "H4" "H6" "H8"))

(define (indexOfS? string l)
  	(define (indexOfS_helper accu string l)
	  	(if (
