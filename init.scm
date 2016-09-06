
(define (not bool)
  	(if bool #f #t))

(define (< a b) (> b a))
(define (= a b) (eq? a b))
(define (!= a b) (not (= a b)))
(define (>= a b) (not (< a b)))
(define (<= a b) (not (> a b)))

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


(define (newline) (display "\n"))

(define (error msg)
  	(display msg) (newline)
	abort)
