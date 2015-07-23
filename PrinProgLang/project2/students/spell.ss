;;  Zachary Allin
;;  RUID: 138-00-0613

; *********************************************
; *  314 Principles of Programming Languages  *
; *  Fall 2014                                *
; *                                           *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(load "include.ss")

;; contains simple dictionary with only four words
;;(load "test-dictionary.ss")

;; contains full dictionary 
(load "dictionary.ss")

;; -----------------------------------------------------
;; Simple test words 
;;   words are represented as lists of symbols, 
;;   e.g., "program" is represented as '(p r o g r a m)

(define hello '(h e l l o))
(define program '(p r o g r a m))
(define of '(o f))
(define language '(l a n g u a g e))


;; -----------------------------------------------------
;; HELPER FUNCTIONS

;; Function that creates the key for the provided word.
(define key_function
  (lambda (w x)
    (if (null? w) 0
    (+ (* (expt 7 (- x (length w) ) ) (ctv (car w)) ) (key_function (cdr w) x) ) )
))

;; Function that calls the head function of the hashlist over the entire dictionary.
(define bitVector2 
  (lambda (hashlist dict)
    (if (null? dict) '()
	(cons ((car hashlist) (car dict)) (bitVector2 hashlist (cdr dict)
      )))
))

;; Function calls bitvector2 over each of the functions in the function-list. 
(define completeVector
  (lambda (hashlist dict)
    (if (null? hashlist) '()
	(append (bitVector2 hashlist dict) (completeVector (cdr hashlist) dict) )
	)
))

;; Function used to sort the bit vector. 
(define sortBitVector
  (lambda (hashlist dict)
    (sort (completeVector hashlist dict) <)
))

;; Function used to remove duplicate instances from the bit vector.
;;     Not used in the program as it took too long to load with the full dictionary. 
(define singleVector
  (lambda (bitVector)
    (cond ((null? bitVector)
	   '() )
	  ((member (car bitVector) (cdr bitVector))
	   (singleVector (cdr bitVector)))
	  (else
	   (cons (car bitVector) (singleVector (cdr bitVector)))
	   )
	  )
))

;; Function used to create the vector for the word provided as input to the spell-checker. 
(define wordVector
  (lambda (hashlist x)
    (if (null? hashlist) '()
	(cons ((car hashlist) x) (wordVector (cdr hashlist) x) )
	)
))

;; Function used to check if all members of xVector are included in bitVector. 
(define isContained
  (lambda (xVector bitVector originalVector)
    (cond ((null? xVector) #t)
	  ((null? bitVector) #f) 
	  (else (if (= (car xVector) (car bitVector)) (isContained (cdr xVector) originalVector originalVector)
	   (isContained xVector (cdr bitVector) originalVector))
		)
	  )
))

;; -----------------------------------------------------
;; KEY FUNCTION
;; Function calls the helper function 'key_function' which will recursively calculate the key as a second parameter is needed.  The helper function preserves this function's interface.  
(define key
  (lambda (w)
    (key_function w (length w) )
))

;; -----------------------------------------------------
;; EXAMPLE KEY FUNCTIONS

(define key1 (key hello))   ;; ==> 40762
(define key2 (key program)) ;; ==> 1592740
(define key3 (key of))      ;; ==> 57
(define key4 (key language));; ==> 5011592


;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; Function calculates the hash value using the division method.  
(define gen-hash-division-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (x)
	(modulo (key x) size ) )
))

;; Function calculates the hash value using the multiplication method.  
(define gen-hash-multiplication-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (x) 
	 (floor (* size (- (* (key x) A) (floor (* (key x) A) ) ) ) )  ) 
))

;; Function calculates the hash value using the hybrid method. 
(define gen-hash-hybrid-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (x)
      (modulo (+ (* 2 ( (gen-hash-division-method size) x)) (* 3 ( (gen-hash-multiplication-method size) x)) ) size)
      )
))


;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS

;; ideally, size should be a prime number for division and hybrid methods

(define hash-1 (gen-hash-division-method 454711))
(define hash-2 (gen-hash-division-method 1297687))
(define hash-3 (gen-hash-multiplication-method 90000))
(define hash-4 (gen-hash-multiplication-method 180001))
(define hash-5 (gen-hash-hybrid-method 454711))
(define hash-6 (gen-hash-hybrid-method 1298687))

;; (hash-1 hello) ;; ==> 40762
;; (hash-2 hello) ;; ==> 40762
;; (hash-3 hello) ;; ==> 27130.0
;; (hash-4 hello) ;; ==> 54260.0
;; (hash-5 hello) ;; ==> 38026.0
;; (hash-6 hello) ;; ==> 1255979.0

;; (hash-1 language) ;; ==> 9771
;; (hash-2 language) ;; ==> 1118531
;; (hash-3 language) ;; ==> 17414.0
;; (hash-4 language) ;; ==> 34829.0
;; (hash-5 language) ;; ==> 283497.0
;; (hash-6 language) ;; ==> 387564.0


;; (hash-1 of) ;; ==> 57
;; (hash-2 of) ;; ==> 57
;; (hash-3 of) ;; ==> 20514.0
;; (hash-4 of) ;; ==> 41028.0
;; (hash-5 of) ;; ==> 311049.0
;; (hash-6 of) ;; ==> 888171.0


;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTION LISTS

(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
(define hashfl-2 (list hash-1 hash-3))
(define hashfl-3 (list hash-1 hash-4 hash-5 hash-6))


;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

;; Function generates a spell-checker using the provided hash function list and dictionary. 
;;    The spell-checker will return either #t or #f.  
(define gen-checker
  (lambda (hashfunctionlist dict)
   ;  (let ([vector (singleVector (sortBitVector hashfunctionlist dict) )]) ;; Had to take out as it took too long to load the file when calling singleVector with the full dictionary.
    (let ([vector (sortBitVector hashfunctionlist dict)])
       ; x will be the word we are checking
       (lambda (x) ;;  vector is the bitvector
	 (let ([xVector (wordVector hashfunctionlist x) ]) ;; calculate the vector for x
	   (isContained xVector vector vector)
	   )
	 )
       )
))


;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

(define checker-1 (gen-checker hashfl-1 dictionary))
(define checker-2 (gen-checker hashfl-2 dictionary))
(define checker-3 (gen-checker hashfl-3 dictionary))



;; EXAMPLE APPLICATIONS OF A SPELL CHECKER
;;
;;  (checker-1 '(l o h a)) ==> #t
;;  (checker-2 '(l o h a)) ==> #t
;;  (checker-2 '(h e l l o)) ==> #t
;;  (checker-3 '(o f)) ==> #t
;;  (checker-3 language)  ==> #t
;;  (checker-1 '(w h a t f u n)) ==> #f
;;  (checker-2 '(w h a t f u n)) ==> #f
;;  (checker-3 '(w h a t f u n)) ==> #f
