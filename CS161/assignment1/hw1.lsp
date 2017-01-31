;#1 create a cond statement//if TREE is an empty list, return nil//if TREE is single object list, compare TREE and N and return t if =//if TREE is atom and TREE = N, return t//if TREE is atom and TREE != N, return nil//if N < second TREE (or m) apply TREE-CONTAINS N (first TREE)//if N > second TREE (or m) apply TREE-CONTAINS N (third TREE)//if N = (second TREE), return t
;@N is the number that is checked within TREE
;@TREE is an ordered tree that N is checked within
;@return_value is a boolean value depending on if N appears in TREE

(defun TREE-CONTAINS (N TREE)
	(cond ((null TREE)nil)
		  ((and(listp TREE)(= 1 (length TREE))(= N(first TREE)))t)
		  ((and(atom TREE)(= N TREE))t)
		  ((and(atom TREE)(not (= N TREE)))nil)
	      ((< N (second TREE))(TREE-CONTAINS N (first TREE)))
		  ((> N (second TREE))(TREE-CONTAINS N (third TREE)))
		  ((= N (second TREE))t)
	)
)

;#2 create a cond statement//if TREE is an empty list, return nil//if TREE is a single object list, return that object//if TREE is a single atom parameter, return that atom//ELSE, recursively apply TREE-MAX to the third object
;@TREE is the ordered TREE that is searched
;@return_Value is the max number of the TREE


(defun TREE-MAX (TREE)
	(cond ((null TREE)nil)
		  ((and (listp TREE)(= 1 (length TREE)))(first TREE))
		  ((and (atom TREE)(not (listp TREE)))TREE)
		  ((and (listp TREE)(not (= 1(length TREE))))(TREE-MAX (third TREE)))
	)
)

;#3 create a cond statement//if TREE is an empty list, return nil//if TREE is a single list TREE, return TREE//if TREE is a single atom, return atom as a list//if the first and third elements of TREE are atoms, return the TREE as a list//Else, Perform TREE-ORDER recursively on the first and third elements, appending the first element as a list to a list created from the second element cons the third list element.
;@TREE is the tree to be ordered
;@return_value is an ordered list of TREE

(defun TREE-ORDER (TREE)
	(cond ((null TREE)nil)
		  ((and (listp TREE)(= 1(length TREE)))TREE)
		  ((and (atom TREE)(not (listp TREE)))(list TREE))
		  ((and (atom(first TREE))(atom(third TREE))) TREE)
		  ((or (listp (first TREE))(listp (third TREE)))
		  	(append (TREE-ORDER(first TREE))(cons(second TREE)(TREE-ORDER(third TREE)))))
	)
)

;#4 create a cond statement//if L is empty, return nil//if START is greater than total length of L, return nil//if LEN is = 0, return nil//if START != 0, recursively call SUB-LIST on (cdr L) until START = 0//if START = 0, create a list, starting from (first L), adding in elements one at a time as LEN decreases.
;@L is a list
;@START is the start number of the Sub-List
;@LEN is the length of the Sub-list
;@return_value is a sub-list of L starting from item START of size LEN

(defun SUB-LIST (L START LEN)
	(cond ((null L)nil)
		  ((> START(length L))nil)
		  ((= 0 LEN)nil)
		  ((not (= 0 START)) (SUB-LIST(cdr L)(- START 1)LEN))
		  ((= 0 START) (cons(first L)(SUB-LIST(cdr L) Start (- LEN 1))))
	)
)

;#5 create a cond statement//if L is empty, return nil//if one element list, return that one element as a list//if L is of even length, create a list consisting of L1, the first half of the list, and L2, the second half of the list//if odd, create a list consisting of L1, the first half of List (excluding the last element)and L2, the second half of the list including the last element.
;@L is the list to be split
;@return_value is a list containing L split into 2 lists

(defun SPLIT-LIST (L)
	(cond ((null L)nil)
		  ((and(listp L)(= 1 (length L))) (list (first L)))
		  ((evenp(length L)) (list (SUB-LIST L 0 (/ (length L) 2)) (SUB-LIST L (/ (length L) 2) (/ (length L) 2))))
		  ((oddp(length L)) (list (SUB-LIST L 0 (/(- (length L)1)2)) (append (SUB-LIST L (/(-(length L)1)2) (/(-(length L)1)2) ) (last L))))
	)
)

;#6 create a cond statement//if there is only 1 atom within LEAVES, return that element//if there are 2 elements in LEAVES, return both within the same leaf//Otherwise, run LIST2BTREE recursively on equally smaller halves of LEAVES using SPLIT-LIST and join at each recursive loop.
;@LEAVES is a non-empty list of atoms
;@return_value is a binary tree

(defun LIST2BTREE (LEAVES)
	(cond ((= 1 (length LEAVES))(first LEAVES))
		  ((null LEAVES)nil)
		  ((= 2 (length LEAVES))LEAVES)
		  (t (list (LIST2BTREE(first(SPLIT-LIST LEAVES))) (LIST2BTREE(second(SPLIT-LIST LEAVES)))))
	)
)

;#7 create a cond statement//if there are no elements in TREE, return nil//if there is only one atom in TREE, return TREE as a list//Else, recursively append BTREELIST applied to the first element of TREE and to the rest of the TREE
;@TREE is the binary tree to be converted to a list
;@return_value is a list version of the binary tree

(defun BTREE2LIST (TREE)
	(cond ((null TREE)nil)
		  ((atom TREE)(list TREE))
		  ((not (atom TREE)) (append (BTREE2LIST (first TREE))(BTREE2LIST (cdr TREE))))
	)
)