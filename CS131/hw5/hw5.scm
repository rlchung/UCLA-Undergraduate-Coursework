;replaces acceptor function and provides calls to call/cc
(define backtracker 
	(lambda (frag)
		(call/cc (lambda (cc) (cons frag (lambda () (cc #f)))))))

(define match-junk
  (lambda (k frag backtracker)
    (or (backtracker frag) ;to be exececuted if there is no match
	(and (< 0 k)
	     (pair? frag)
	     (match-junk (- k 1) (cdr frag) backtracker)))))

(define match-*
  (lambda (matcher frag backtracker)
    (or (backtracker frag)
	(matcher frag
		 (lambda (frag1)
		   (and (not (eq? frag frag1))
			(match-* matcher frag1 backtracker)))))))

;to be utilized by larger make-matcher function
(define matcher-main
  (lambda (pat)
    (cond
    	;input 'pat' is symbol
     ((symbol? pat)
      (lambda (frag backtracker)
	(and (pair? frag)
	     (eq? pat (car frag))
	     (backtracker (cdr frag)))))

     	;input 'pat' is 'or pat'
     ((eq? 'or (car pat))
      (let make-or-matcher ((pats (cdr pat)))
	(if (null? pats)
	    (lambda (frag backtracker) #f)
	    (let ((head-matcher (matcher-main (car pats)))
		  (tail-matcher (make-or-matcher (cdr pats))))
	      (lambda (frag backtracker)
		(or (head-matcher frag backtracker)
		    (tail-matcher frag backtracker)))))))

     	;input 'pat' is 'list pat'
     ((eq? 'list (car pat))
      (let make-list-matcher ((pats (cdr pat)))
	(if (null? pats)
	    (lambda (frag backtracker) (backtracker frag))
	    (let ((head-matcher (matcher-main (car pats)))
		  (tail-matcher (make-list-matcher (cdr pats))))
	      (lambda (frag backtracker)
		(head-matcher frag
			      (lambda (frag1)
				(tail-matcher frag1 backtracker))))))))

     	;input 'pat' is 'junk pat'
     ((eq? 'junk (car pat))
      (let ((k (cadr pat)))
	(lambda (frag backtracker)
	  (match-junk k frag backtracker))))

     	;input 'pat' is '* pat'
     ((eq? '* (car pat))
      (let ((matcher (matcher-main (cadr pat))))
	(lambda (frag backtracker)
	  (match-* matcher frag backtracker)))))))

;must take in single arg 'pat' to be processed by matcher-main
(define (make-matcher pat)
		(lambda (frag) ((matcher-main pat) frag backtracker)))