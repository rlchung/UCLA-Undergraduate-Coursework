;
; CS161 Hw3: Sokoban
; 
; *********************
;    READ THIS FIRST
; ********************* 
;
; All functions that you need to modify are marked with 'EXERCISE' in their header comments.
; Do not modify a-star.lsp.
; This file also contains many helper functions. You may call any of them in your functions.
;
; *Warning*: The provided A* code only supports the maximum cost of 4999 for any node.
; That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your heuristic functions.
; Do not make them return anything too large.
;
; For Allegro Common Lisp users: The free version of Allegro puts a limit on memory.
; So, it may crash on some hard sokoban problems and there is no easy fix (unless you buy 
; Allegro). 
; Of course, other versions of Lisp may also crash if the problem is too hard, but the amount
; of memory available will be relatively more relaxed.
; Improving the quality of the heuristic will mitigate this problem, as it will allow A* to
; solve hard problems with fewer node expansions.
; 
; In either case, this limitation should not significantly affect your grade.
; 
; Remember that most functions are not graded on efficiency (only correctness).
; Efficiency can only influence your heuristic performance in the competition (which will
; affect your score).
;  
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
; They are not necessary for this homework.
; Use/modify them for your own convenience.
;

;
; For reloading modified code.
; I found this easier than typing (load "filename") every time. 
;
(defun reload()
  (load "hw3.lsp")
  )

;
; For loading a-star.lsp.
;
(defun load-a-star()
  (load "a-star.lsp"))

;
; Reloads hw3.lsp and a-star.lsp
;
(defun reload-all()
  (reload)
  (load-a-star)
  )

;
; A shortcut function.
; goal-test and next-states stay the same throughout the assignment.
; So, you can just call (sokoban <init-state> #'<heuristic-name>).
; 
;
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end general utility functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We now begin actual Sokoban code
;

; Define some global variables
(setq blank 0)
(setq wall 1)
(setq box 2)
(setq keeper 3)
(setq star 4)
(setq boxstar 5)
(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

;
; Helper function of getKeeperPosition
;
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
	(t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
	       col
	     (getKeeperColumn (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

;
; getKeeperPosition (s firstRow)
; Returns a list indicating the position of the keeper (c r).
; 
; Assumes that the keeper is in row >= firstRow.
; The top row is the zeroth row.
; The first (right) column is the zeroth column.
;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
	(t (let ((x (getKeeperColumn (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getKeeperPosition (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

;
; cleanUpList (l)
; returns l with any NIL element removed.
; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
;
(defun cleanUpList (L)
  (cond ((null L) nil)
	(t (let ((cur (car L))
		 (res (cleanUpList (cdr L)))
		 )
	     (if cur 
		 (cons cur res)
		  res
		 )
	     );end let
	   );end t
	);end cond
  );end 

; EXERCISE: Modify this function to return true (t)
; if and only if s is a goal state of a Sokoban game.
; (no box is on a non-goal square)
;
; Currently, it always returns NIL. If A* is called with
; this function as the goal testing function, A* will never
; terminate until the whole search space is exhausted.
;

;boxinRow is a helper function that iterates recursively throughout a given row and checks whether each element is a box element or not
;EXPLANATION: recusively iterate through the given row to see if the element is a box (and not a box+goal element)
;@row is a list representing a row of a given state
;@return_value: returns T if it is a box, NIL otherwise

(defun boxinRow (row)
	(cond ((null row) nil)
		  ((isBox(car row)) t)
		  (t (boxinRow(cdr row)))	
	)
)

;goal-test tests s to see if it is the goal-state
;EXPLANATION: Uses function "boxinRow" recusively throughout each row in the state
;@s is the state
;@return_value is a boolean

(defun goal-test (s)
	(cond ((and (= 1 (length s))(not (boxinRow (car s)))) t)
		  ((boxinRow (car s)) nil)
		  (t (goal-test (cdr s)))
	)
);end defun

; EXERCISE: Modify this function to return the list of 
; sucessor states of s.
;
; This is the top-level next-states (successor) function.
; Some skeleton code is provided below.
; You may delete them totally, depending on your approach.
; 
; If you want to use it, you will need to set 'result' to be 
; the set of states after moving the keeper in each of the 4 directions.
; A pseudo-code for this is:
; 
; ...
; (result (list (try-move s UP) (try-move s DOWN) (try-move s LEFT) (try-move s RIGHT)))
; ...
; 
; You will need to define the function try-move and decide how to represent UP,DOWN,LEFT,RIGHT.
; Any NIL result returned from try-move can be removed by cleanUpList.
; 
;

;get-square retrieves the object at the (r,c) coordinate
;EXPLANATION: if either of r or c are negative, they will be reset to the border of the frame, that is the coordinate (0,0) respectively. Else, retrieve coordinates by obtaining the first elements of repeated nthcdr operations
;@s is the state
;@r is the row
;@c is the column
;@return_value is the object

(defun get-square (s r c)
	(cond ((< r 0) (get-square s 0 c))
		  ((< c 0) (get-square s r 0))
		  (t (car(nthcdr c(car(nthcdr r s)))))
	)
)


;set-square sets the object at the (r,c) coordinate to v
;EXPLANATION: create a copy of the row consisting of: (1) the first half of the row before the element (2) the second half of the row after the element (3) the element itself// then, append the respective elements together to form the new list
;@s is the state
;@r is the row
;@c is the column
;@v is the object that the object at (r,c) will be set to
;@return_value is the altered state

(defun set-square (s r c v)
	(let ((output s)
		 (rowfirsthalf (butlast (car (nthcdr r s))(- (length (car s)) c)))
		 (rowsecondhalf (nthcdr (+ 1 c)(car(nthcdr r s))))
		 (statefirsthalf (butlast s (- (length s) r)))
		 (statesecondhalf (nthcdr (+ 1 r) s))
		 )  
		 
		 (append statefirsthalf (list (append rowfirsthalf (append (list v) rowsecondhalf))) statesecondhalf)
	)
)

;valid-move checks whether or not the move from (r1,c1) to (r2,c2) is valid or not
;if the destination is a wall, return nil//In each given direction, if: (1) the destination contains a box or not [either 2 or 5] (2) the square after the destination in the direction of travel is another box, wall, or goal w/ a box, return nil. Otherwise, return true
;@s is the state
;@r1 is the row of origin
;@r2 is the row of dest
;@c1 is the column of origin
;@c2 is the column of dest
;@return_value is a boolean

(defun valid-move (s r1 c1 r2 c2)
	(cond ((null s) nil)
		  ((= 1 (get-square s r2 c2)) nil)
		  ((and (or (= 2 (get-square s r2 c2)) (= 5 (get-square s r2 c2)))
		  		(equal 1 (- r1 r2)) (not(and(not(= 2 (get-square s (- r2 1) c2)))
		  					           (not(= 5 (get-square s (- r2 1) c2)))
		  					           (not(= 1 (get-square s (- r2 1) c2))))))
		  		nil)
		  ((and (or (= 2 (get-square s r2 c2)) (= 5 (get-square s r2 c2)))
		  		(equal -1 (- r1 r2)) (not(and(not(= 2 (get-square s (+ r2 1) c2)))
		  					             (not(= 5 (get-square s (+ r2 1) c2)))
		  					             (not(= 1 (get-square s (+ r2 1) c2))))))
		  		nil)
		  ((and (or (= 2 (get-square s r2 c2)) (= 5 (get-square s r2 c2)))
		  		(equal 1 (- c1 c2)) (not(and(not(= 2 (get-square s r2 (- c2 1))))
		  					             (not(= 5 (get-square s r2 (- c2 1))))
		  					             (not(= 1 (get-square s r2 (- c2 1)))))))
		  		nil)
		  ((and (or (= 2 (get-square s r2 c2)) (= 5 (get-square s r2 c2)))
		  		(equal -1 (- c1 c2)) (not(and(not(= 2 (get-square s r2 (+ c2 1))))
		  					              (not(= 5 (get-square s r2 (+ c2 1))))
		  					              (not(= 1 (get-square s r2 (+ c2 1)))))))
		  		nil)
		  (t t)
	)
)

;move returns a state after the given move is made
;create separate variables indicated the: (1) origin (2) destination (3) the destination of 2 after the move (if applicable)//create a series of conditions that determine the relevant outcomes of the moves
;@s is the state
;@r1 is the row of origin
;@r2 is the row of dest
;@c1 is the column of origin
;@c2 is the column of dest
;@return_value is a list depicting the resulting state

(defun move (s r1 c1 r2 c2)
	(let ((origin (get-square s r1 c1))
	      (dest1 (get-square s r2 c2))
	      (dest2 (get-square s (+ (- r2 r1) r2)(+ (- c2 c1) c2))))
	
		(cond ((null s) nil)
			  ((not (valid-move s r1 c1 r2 c2)) nil)
			  ((and (= origin 3)(= dest1 0))
			  		(set-square (set-square s r1 c1 0) r2 c2 3))
			  ((and (= origin 6)(= dest1 0))
			  		(set-square (set-square s r1 c1 4) r2 c2 3))
			  ((and (= origin 3)(= dest1 4))
			  		(set-square (set-square s r1 c1 0) r2 c2 6))
			  ((and (= origin 6)(= dest1 4))
			  		(set-square (set-square s r1 c1 4) r2 c2 6))

			  ((and (= origin 3)(= dest1 2)(= dest2 0))
			  		(set-square(set-square(set-square s r1 c1 0)r2 c2 3)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)2))
			  ((and (= origin 6)(= dest1 2)(= dest2 0))
			  		(set-square(set-square(set-square s r1 c1 4)r2 c2 3)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)2))
			  ((and (= origin 3)(= dest1 2)(= dest2 4))
			  		(set-square(set-square(set-square s r1 c1 0)r2 c2 3)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)5))
			  ((and (= origin 6)(= dest1 2)(= dest2 4))
			  		(set-square(set-square(set-square s r1 c1 4)r2 c2 3)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)5))

			  ((and (= origin 3)(= dest1 5)(= dest2 0))
			  		(set-square(set-square(set-square s r1 c1 0)r2 c2 6)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)2))
			  ((and (= origin 6)(= dest1 5)(= dest2 0))
			  		(set-square(set-square(set-square s r1 c1 4)r2 c2 6)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)2))
			  ((and (= origin 3)(= dest1 5)(= dest2 4))
			  		(set-square(set-square(set-square s r1 c1 0)r2 c2 6)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)5))
			  ((and (= origin 6)(= dest1 5)(= dest2 4))
			  		(set-square(set-square(set-square s r1 c1 4)r2 c2 6)
			  				   (+(- r2 r1)r2)(+(- c2 c1)c2)5))
		)
	)
)


;next-states creates a list containing the states that would result from successful moves
;appends together a list containing the results from the function "move" in all 4 directions
;@s is the state
;@return_value is a list of states

(defun next-states (s)
  (let* ((pos (getKeeperPosition s 0))
	 (y (car pos))
	 (x (cadr pos))
	 ;x and y are now the coordinate of the keeper in s.
	 ;(result nil)
	 )

	 (cleanUpList(append (list (move s x y (- x 1) y)) (list (move s x y x (+ y 1))) (list (move s x y (+ x 1) y)) (list(move s x y x (- y 1)))))

    ;(cleanUpList result);end
   );end let
);

; EXERCISE: Modify this function to compute the trivial 
; admissible heuristic.
;
(defun h0 (s)
	0
)

; EXERCISE: Modify this function to compute the 
; number of misplaced boxes in s.
; Yes, it is admissible; the 

;h1-helper is a helper function for h1 to check the count of each row
;@row is the row to be checked
;@return_value is the interger count of misplaced boxes

(defun h1-helper(row)
	(cond ((null row) 0)
		  (t (count 2 row))
	)
)

;h1 is function that counts the total amount of misplaced boxes
;EXPLANATION: uses h1-helper recursively throughout each row and calculates the sum total of mispalced boxes
;@s is the state
;@return_value is the interger count of misplaced boxes

(defun h1 (s)
	(cond ((null s) 0)
		  (t (+ (h1-helper(car s))(h1(cdr s))))
	)  
)

; EXERCISE: Change the name of this function to h<UID> where
; <UID> is your actual student ID number. Then, modify this 
; function to compute an admissible heuristic value of s. 
; 
; This function will be entered in the competition.
; Objective: make A* solve problems as fast as possible.
; The Lisp 'time' function can be used to measure the 
; running time of a function call.
;
(defun hUID (s)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Some predefined problems.
 | Each problem can be visualized by calling (printstate <problem>). For example, (printstate p1).
 | Problems are ordered roughly by their difficulties.
 | For most problems, we also privide 2 additional number per problem:
 |    1) # of nodes expanded by A* using our next-states and h0 heuristic.
 |    2) the depth of the optimal solution.
 | These numbers are located at the comments of the problems. For example, the first problem below 
 | was solved by 80 nodes expansion of A* and its optimal solution depth is 7.
 | 
 | Your implementation may not result in the same number of nodes expanded, but it should probably
 | give something in the same ballpark. As for the solution depth, any admissible heuristic must 
 | make A* return an optimal solution. So, the depths of the optimal solutions provided could be used
 | for checking whether your heuristic is admissible.
 |
 | Warning: some problems toward the end are quite hard and could be impossible to solve without a good heuristic!
 | 
 |#

;(80,7)

(setq p1 '((1 1 1 1 1 1)
	   (1 0 3 0 0 1)
	   (1 0 2 0 0 1)
	   (1 1 0 1 1 1)
	   (1 0 0 0 0 1)
	   (1 0 0 0 4 1)
	   (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 1) 
	   (1 0 0 0 0 0 1) 
	   (1 0 0 2 1 4 1) 
	   (1 3 0 0 1 0 1)
	   (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 2 0 3 4 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
	   (0 0 0 0 0 1 4)
	   (0 0 0 0 0 0 0)
	   (0 0 1 1 1 0 0)
	   (0 0 1 0 0 0 0)
	   (0 2 1 0 0 0 0)
	   (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
	   (1 1 0 0 1 1)
	   (1 0 0 0 0 1)
	   (1 4 2 2 4 1)
	   (1 0 0 0 0 1)
	   (1 1 3 1 1 1)
	   (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 4 1)
	   (1 0 0 0 2 2 3 1)
	   (1 0 0 1 0 0 4 1)
	   (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
	   (0 0 1 1 1 1 0 0 0 3)
	   (0 0 0 0 0 1 0 0 0 0)
	   (0 0 0 0 0 1 0 0 1 0)
	   (0 0 1 0 0 1 0 0 1 0)
	   (0 2 1 0 0 0 0 0 1 0)
	   (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
	   (1 4 0 0 4 1)
	   (1 0 2 2 0 1)
	   (1 2 0 1 0 1)
	   (1 3 0 0 4 1)
	   (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
	   (1 1 1 0 0 1 1 1 1) 
	   (1 0 0 0 0 0 2 0 1) 
	   (1 0 1 0 0 1 2 0 1) 
	   (1 0 4 0 4 1 3 0 1) 
	   (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
	    (1 0 0 0 1 1 0)
	    (1 3 2 0 0 1 1)
	    (1 1 0 2 0 0 1)
	    (0 1 1 0 2 0 1)
	    (0 0 1 1 0 0 1)
	    (0 0 0 1 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
	    (1 4 0 0 0 4 1)
	    (1 0 2 2 1 0 1)
	    (1 0 2 0 1 3 1)
	    (1 1 2 0 1 0 1)
	    (1 4 0 0 4 0 1)
	    (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
	    (1 1 1 1 1 0 0 0 1 1 1 1)
	    (1 0 0 0 2 0 0 0 0 0 0 1)
	    (1 3 0 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 2 1 1 1 0 0 0 1)
	    (1 0 0 0 0 1 0 1 4 0 4 1)
	    (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
	    (1 4 0 0 0 0 0 2 0 1)
	    (1 0 2 0 0 0 0 0 4 1)
	    (1 0 3 0 0 0 0 0 2 1)
	    (1 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 0 0 0 0 4 1)
	    (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
	    (0 2 1 4 0 0 0)
	    (0 2 0 4 0 0 0)	   
	    (3 2 1 1 1 0 0)
	    (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
	    (1 0 0 0 0 0 1)
	    (1 0 0 2 2 0 1)
	    (1 0 2 0 2 3 1)
	    (1 4 4 1 1 1 1)
	    (1 4 4 1 0 0 0)
	    (1 1 1 1 0 0 0)
	    ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
	    (1 0 0 0 1 0 0 0)
	    (1 2 1 0 1 1 1 1)
	    (1 4 0 0 0 0 0 1)
	    (1 0 0 5 0 5 0 1)
	    (1 0 5 0 1 0 1 1)
	    (1 1 1 0 3 0 1 0)
	    (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
	    (1 3 0 0 1 0 0 0 4 1)
	    (1 0 2 0 2 0 0 4 4 1)
	    (1 0 2 2 2 1 1 4 4 1)
	    (1 0 0 0 0 1 1 4 4 1)
	    (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)	    
	    (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
	    ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 0 1 0 0 1 0 0 0 0)
	    (0 0 0 0 0 0 3 0 0 0 2 0)
	    (0 0 0 0 1 0 0 1 0 0 0 4)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
	    (1 1 1 1 0 0 1 1 0)
	    (1 0 0 0 2 0 0 1 0)
	    (1 0 0 5 5 5 0 1 0)
	    (1 0 0 4 0 4 0 1 1)
	    (1 1 0 5 0 5 0 0 1)
	    (0 1 1 5 5 5 0 0 1)
	    (0 0 1 0 2 0 1 1 1)
	    (0 0 1 0 3 0 1 0 0)
	    (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
	    (1 1 1 0 0 1 1 1 1 0)
	    (1 0 0 2 0 0 0 1 1 0)
	    (1 3 2 0 2 0 0 0 1 0)
	    (1 1 0 2 0 2 0 0 1 0)
	    (0 1 1 0 2 0 2 0 1 0)
	    (0 0 1 1 0 2 0 0 1 0)
	    (0 0 0 1 1 1 1 0 1 0)
	    (0 0 0 0 1 4 1 0 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 0 1 4 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
	    (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
	    (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
	    (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
	    (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
	    (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
	    (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
	    (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

;
; Helper function of prettyMoves
; from s1 --> s2
;
(defun detectDiff (s1 s2)
  (let* ((k1 (getKeeperPosition s1 0))
	 (k2 (getKeeperPosition s2 0))
	 (deltaX (- (car k2) (car k1)))
	 (deltaY (- (cadr k2) (cadr k1)))
	 )
    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
	  (t (if (> deltaX 0) 'RIGHT 'LEFT))
	  );end cond
    );end let
  );end defun

;
; Translates a list of states into a list of moves.
; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
;
(defun prettyMoves (m)
  (cond ((null m) nil)
	((= 1 (length m)) (list 'END))
	(t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
	);end cond
  );

;
; Print the content of the square to stdout.
;
(defun printSquare (s)
  (cond ((= s blank) (format t " "))
	((= s wall) (format t "#"))
	((= s box) (format t "$"))
	((= s keeper) (format t "@"))
	((= s star) (format t "."))
	((= s boxstar) (format t "*"))
	((= s keeperstar) (format t "+"))
	(t (format t "|"))
	);end cond
  )

;
; Print a row
;
(defun printRow (r)
  (dolist (cur r)
    (printSquare cur)    
    )
  );

;
; Print a state
;
(defun printState (s)
  (progn    
    (dolist (cur s)
      (printRow cur)
      (format t "~%")
      )
    );end progn
  )

;
; Print a list of states with delay.
;
(defun printStates (sl delay)
  (dolist (cur sl)
    (printState cur)
    (sleep delay)
    );end dolist
  );end defun
