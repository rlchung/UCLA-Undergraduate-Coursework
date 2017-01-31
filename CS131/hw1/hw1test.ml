let subset_test0 = subset ['a'] ['a';'b';'c'];;
let subset_test1 = subset ['a';'a'] ['a';'b';'c'];;

let proper_subset_test0 = proper_subset [] ["a";"c";"d"];;
let proper_subset_test1 = proper_subset ["a";"a";"a";"c"] ["a";"c";"d"];;

let equal_sets_test0 = equal_sets [10;11;12] [11;11;12;12;10;10];;
let equal_sets_test1 = not (equal_sets [10;10;10;10;11;11] [10;11;12]);;

let set_diff_test0 = equal_sets (set_diff ['a';'b'] ['a';'b';'c';'c']) [];;
let set_diff_test1 = equal_sets (set_diff ['c';'a';'a';'c'] ['a';'b']) ['c'];;

let computed_fixed_point_test0 =
  computed_fixed_point (=) (fun z -> z*z - 3*z + 4) 2 = 2;;

let computed_periodic_point_test0 =
  computed_periodic_point (=) (fun x -> x / 2 + 1) 0 (-1) = -1;;

type dancers_nonterminals =
	| Hiphop | Madrid | Kuang | Martin;;

let dancer_rules =
	[Hiphop, [N Madrid; N Kuang; N Martin];
	Madrid, [T"Latch"];
	Madrid, [T"Manolo"];
	Kuang, [T"Real_love"];
	Martin, [T"773"];
	Martin, [T"Cater"];
	Martin, [T"Coco"]];;

let dancer_grammar = (Hiphop, dancer_rules);;

let dance_test0 = filter_blind_alleys dancer_grammar = dancer_grammar;;