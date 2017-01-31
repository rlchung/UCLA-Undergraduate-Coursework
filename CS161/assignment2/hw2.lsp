;#1 Performs a depth first search on the given input tree: create a cond statement//return 'nil' if TREE is empty//if TREE is a single element TREE, that is, it contains only an atom or is a list of size = 1, then return that element in list fashion//else, run DFS recursively on the first left subtree of the TREE and then append that result to the DFS or the remaining subtrees.

;@TREE is a list we want to run DFS on
;@return_value is a appended list containing the DFS search order

(defun DFS(TREE)
	(cond	((and (null TREE)(listp TREE))nil)
			((atom TREE)(list TREE))
			(t (append (DFS(first TREE))(DFS(cdr TREE))))
	)	
)

;#2 Performs a depth first search that is limited to the given level as specified by the input parameter 'limit': create a cond statement//returns nil if TREE is empty or if the limit is less than 0//if the given element is a single element, print it as a list//else, append a recursive run of DFS_limit on the first branch of the tree and then a recursrive run of DFS_limit of the rest of the TREE.
;in this first element that is appended, the limit is decremented 1 as to extract out a possible leaf element that is contained within a single element list

;@TREE is the list we want to run DFS_limit on
;@limit is the given level whose nodes we want printed out
;@return_value are the nodes within level 'limit' of the TREE

(defun DFS_limit (TREE limit)
	(cond ((null TREE) nil)
		  ((< limit 0) nil)
		  ((atom TREE) (list TREE))
		  (t (append(DFS_limit(First TREE)(- limit 1))(DFS_limit(cdr TREE) limit)))
	)
)

;Performs an iterative search of 'TREE' from 'iter' to 'Max_D': create a cond statement//if the given 'iter' (the level we want to start from) is greater than the max amount of levels in 'TREE', return nil//if they are equal, it suggests that we should print out only that given level, utilizing DFS_limit on that level//else, we append iteratively deeper levels to each other using DFS_limit iteratively per level.

;@TREE is the list we want to run DFS_iter on
;@Max_D is the maximum depth of the tree
;@iter is the the level we want to start iterating from
;@return_value is the order of nodes that were searched using DFS from level iter to Max_D

(defun DFS_iter (TREE Max_D iter)
	(cond ((> iter Max_D)nil)
		  ((= iter Max_D)(DFS_limit TREE iter))
		  (t (append(DFS_limit TREE iter)(DFS_iter TREE Max_D (+ 1 iter))))
	)
)

;Performs depth-first iterative deepening utiling the helper functions defined above.

;@TREE is the list we want to run DFID on
;@Max_D is the maximum depth of the tree
;@return_value is a single top-level list of the terminal nodes in the order that they would be visited by a left-to-right DFID search

(defun DFID (TREE Max_D)
	(DFS_iter TREE Max_D 0)
)

;#3
; These functions implement a depth-first solver for the missionary-cannibal
; problem. In this problem, three missionaries and three cannibals are trying to
; go from the east side of a river to the west side. They have a single boat
; that can carry two people at a time from one side of the river to the
; other. There must be at least one person in the boat to cross the river. There
; can never be more cannibals on one side of the river than missionaries. If
; there are, the cannibals eat the missionaries.

; In this implementation, a state is represented by a single list
; (missionaries cannibals side). side represents which side the boat is
; currently on, and is T if it is on the east side and NIL if on the west
; side. missionaries and cannibals represent the number of missionaries and
; cannibals on the same side as the boat. Thus, the initial state for this
; problem is (3 3 T) (three missionaries, three cannibals, and the boat are all
; on the east side of the river) and the goal state is (3 3 NIL).

; The main entry point for this solver is the function MC-DFS, which is called
; with the initial state to search from and the path to this state. It returns
; the complete path from the initial state to the goal state: this path is a
; list of intermediate problem states. The first element of the path is the
; initial state and the last element is the goal state. Each intermediate state
; is the state that results from applying the appropriate operator to the
; preceding state. If there is no solution, MC-DFS returns NIL.

; To call MC-DFS to solve the original problem, one would call (MC-DFS '(3 3 T)
; NIL) -- however, it would be possible to call MC-DFS with a different initial
; state or with an initial path.

; Examples of calls to some of the helper functions can be found after the code.


; FINAL-STATE takes a single argument s, the current state, and returns T if it
; is the goal state (3 3 NIL) and NIL otherwise.
(defun FINAL-STATE (s)
	(cond ((and (= (first s) 3)(= (second s) 3))(null (third s)))
		  (t nil)) 
)

; NEXT-STATE returns the state that results from applying an operator to the
; current state. It takes three arguments: the current state (s), a number of
; missionaries to move (m), and a number of cannibals to move (c). It returns a
; list containing the state that results from moving that number of missionaries
; and cannibals from the current side of the river to the other side of the
; river. If applying this operator results in an invalid state (because there
; are more cannibals than missionaries on either side of the river, or because
; it would move more missionaries or cannibals than are on this side of the
; river) it returns NIL.
;
; NOTE that next-state returns a list containing the successor state (which is
; itself a list); the return should look something like ((1 1 T)).
(defun NEXT-STATE (s m c)
	(let* ((new_state (list (+ m (- 3 (first s)))
						   (+  c (- 3 (second s)))
						   (not (third s))))
		   (new_m (first new_state))
		   (new_c (second new_state))
		   (opp_m (- 3 new_m))
		   (opp_c (- 3 new_c)))

		   (cond ((or (> (+ m c) 2)(<= (+ m c) 0)) nil) ;if moved more than 2 cannibals and missionaries total or negative values
		   	     ((and (> new_m 0)(> new_c new_m)) nil) ;if moved more cannibals than missionaries over
		   	     ((and (> opp_m 0)(> opp_c opp_m)) nil) ;if we left more cannibals than missionaries on one side
		   	     ((or (> new_m 3)(< new_m 0)) niL)	    ;if there are no more than 3 or less than 0 missionaries in new state
		   	     ((or (> new_c 3)(< new_c 0)) nil)      ;if there are no more than 3 or less than 0 cannibals in new state
		   	     ((= m c) (list new_state)) ;if an equal amount is moved
		   	     (t (list new_state))
		   )
	)
)

; SUCC-FN returns all of the possible legal successor states to the current
; state. It takes a single argument (s), which encodes the current state, and
; returns a list of each state that can be reached by applying legal operators
; to the current state.
(defun SUCC-FN (s)
  (append (NEXT-STATE s 0 1)
  		  (NEXT-STATE s 0 2)
  		  (NEXT-STATE s 1 0)
  		  (NEXT-STATE s 2 0)
  		  (NEXT-STATE s 1 1)
  )
)

; ON-PATH checks whether the current state is on the stack of states visited by
; this depth-first search. It takes two arguments: the current state (s) and the
; stack of states visited by MC-DFS (states). It returns T if s is a member of
; states and NIL otherwise.
(defun ON-PATH (s states)
  (cond  ((null states) nil)
  		 ((null s) nil)
  		 (t (or (equal s (car states))(ON-PATH s (rest states))))
  )
)

; MULT-DFS is a helper function for MC-DFS. It takes two arguments: a stack of
; states from the initial state to the current state (path), and the legal
; successor states to the last state on that stack (states). states is a
; first-in first-out list of states; that is, the first element is the initial
; state for the current search and the last element is the most recent state
; explored. MULT-DFS does a depth-first search on each element of states in
; turn. If any of those searches reaches the final state, MULT-DFS returns the
; complete path from the initial state to the goal state. Otherwise, it returns
; NIL.
(defun mult-dfs (states path)
	(cond  ((NULL states) NIL)
		   ((FINAL-STATE (car states)) (append path (list (car states))))
		   ((and (not (ON-PATH (car states) path)) (not (NULL (MULT-DFS (SUCC-FN (car states))
				(append path (list (car states))))))) (MULT-DFS (SUCC-FN (car states))
				(append path (list (car states)))))
		(t (MULT-DFS (cdr states) path))
	)
)


; MC-DFS does a depth first search from a given state to the goal state. It
; takes two arguments: a state (S) and the path from the initial state to S
; (PATH). If S is the initial state in our search, PATH should be NIL. MC-DFS
; performs a depth-first search starting at the given state. It returns the path
; from the initial state to the goal state, if any, or NIL otherwise. MC-DFS is
; responsible for checking if S is already the goal state, as well as for
; ensuring that the depth-first search does not revisit a node already on the
; search path.
(defun mc-dfs (s path)
	(cond ((ON-PATH s path) nil)
		  ((null s) nil)
		  ((and (FINAL-STATE s)(not (null path)))(append path (list s)))
		  (t (MULT-DFS (SUCC-FN s)(append path(list s))))
    )
)

; Function execution examples

; Applying this operator would result in an invalid state, with more cannibals
; than missionaries on the east side of the river.
; (next-state '(3 3 t) 1 0) -> NIL

; Applying this operator would result in one cannibal and zero missionaries on
; the west side of the river, which is a legal operator. (NOTE that next-state
; returns a LIST of successor states, even when there is only one successor)
; (next-state '(3 3 t) 0 1) -> ((0 1 NIL))

; succ-fn returns all of the legal states that can result from applying
; operators to the current state.
; (succ-fn '(3 3 t)) -> ((0 1 NIL) (1 1 NIL) (0 2 NIL))
; (succ-fn '(1 1 t)) -> ((3 2 NIL) (3 3 NIL))

