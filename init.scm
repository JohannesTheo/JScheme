
(define (not bool)
  	(if bool #f #t))

(define (< a b) (> b a))
(define (= a b) (eq? a b))
(define (!= a b) (not (= a b)))
(define (>= a b) (not (< a b)))
(define (<= a b) (not (> a b)))

(define (zero? n)
  	(eq? n 0))

(define (negative? n)
  	(> 0 n))

(define (positive? n)
  	(> n 0))

(define (abs num)
  	(if (negative? num)
	  	(* num -1)
		num))

(define (even? a)
	(if (= 0 a)
		#t
		(if (> a 0)
  			(if (= (/ a 2) (/ (+ a 1) 2))
	  		#t
	  		#f)
			(if (= (/ a 2) (/ (- a 1) 2))
			#t
			#f)
		)
	)
)

(define (nth n l)
	(if (< n 1)
		(error "(nth): index error")
		(if (eq? l nil)
			(error "(nth): index out of range")
			(if (= n 1)
				(car l)
				(nth (- n 1) (cdr l) )))))

(define (set_nth n l o)
	(if (< n 1)
	  	(error "(set_nth): index error")
		(if (eq? l nil)
			(error "(set_nth): index out of range")
			(if (= n 1)
			  	(set-car! l o)
				(set_nth (- n 1) (cdr l) o )))))

(define (indexOfS? string l)
  	(define (indexOfS?_helper accu string l)
	  	(if (eq? l nil)
	  		-1
		  	(if (string=? string (car l))
		  		accu
				(indexOfS?_helper (+ accu 1) string (cdr l))
				)))
	(indexOfS?_helper 1 string l))

(define (newline) (display "\n"))

(define (error msg)
  	(display msg) (newline)
	abort)
