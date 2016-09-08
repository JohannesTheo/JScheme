(define (loop1) (display "\nTHIS IS A TAILCALL LOOP\n") (loop2))
(define (loop2) (display "works like a charme! :)\n") (loop1))

(loop1)
