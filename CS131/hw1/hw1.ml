(*Warmup 1: Write a function 'subset a b' that returns true iff a is a subset of b*)
let rec subset a b = 
	match a with
	| [] -> true
	| _ -> if List.mem (List.hd a) b then subset (List.tl a) b else false;;

(*Warmup 2: Write a function 'proper_subset a b' that returns true iff a is a proper subset of b*)

let proper_subset a b = 
	match a with
	| _ -> if subset b a then false else subset a b;;

(*Warmup 3: Write a function 'equal_sets a b' returns true iff the represented sets are equal*)

let equal_sets a b =
	match a with
	| _ -> if subset a b then subset b a else false;;

(*Warmup 4: Write a function 'set_diff a b' that returns a list representing a-b, that is, the set of all
	members of a that are not also members of b*)

let rec set_diff a b =
	match a with
	| [] -> []
	| _ -> if Pervasives.not(List.mem (List.hd a) b) then (List.hd a)::set_diff(List.tl a) b 
		   else set_diff(List.tl a) b;;

(*Warmup 5: Write a function 'computed_fixed_point eq f x' that returns the computed fixed point for f with
	respect to x, assuming that eq is the equality predicate for f's domain.*)

let rec computed_fixed_point eq f x =
	if eq x (f x) then x
	else computed_fixed_point eq f (f x);;

(*Warmup 6: Write a function 'computed_periodic_point eq f p x' that returns the computed periodic point
	for f with period p and with respect to x, assuming that eq is the equality predicate for f's domain.*)

let rec computed_periodic_point eq f p x =
	match p with
	| 0 -> x
	| _ -> if eq x (computed_periodic_point eq f (p-1) (f x)) then x
		   else computed_periodic_point eq f (p) (f x);;

(*FINAL_FUNCTION: Write a function "filter_blind_alleys g" that returns a copy of the grammar g with all the 
	blind-alleys removed. This function should preserve the order of rules: that is, all rules that are returned
	should be in the same order as the rules in g.*)

type ('nonterminal, 'terminal) symbol =
	| N of 'nonterminal
	| T of 'terminal;;

let is_terminal symbol =
	match symbol with
	| N _ -> false
	| T _ -> true

let get_sym_val symbol =
	match symbol with
	| N a -> a;;

(*found_next_rule determines if the given 'value' resolves to a possible rule within 'rules'*)

let rec found_next_rule value rules =
	match rules with
	| [] -> false
	| _ -> if (=) (fst (List.hd rules)) value then true
		else found_next_rule value (List.tl rules);;

(*Used when found_next_rule is true; returns the next rule that satisfies the 'value' element*)

let rec return_next_rule value rules =
	if (=) (fst (List.hd rules)) value then (List.hd rules)
else (return_next_rule value (List.tl rules));;

(*is_terminal_subrule determines if the given subrule, such as N Expr, terminates*)
(*
let rec is_terminal_subrule subrule rhs rules =
	if (=) rhs [] then true
else if (is_terminal subrule) then
else if Pervasives.not(found_next_rule(extract_value_symbol subrule)) then false
else*)

(*is_terminal_rule takes in: a rhs, the rule the rhs derives from, and a set of rules. Returns true or false
depending on whether the given rule terminates or not*)
let rec is_terminal_rule rhs rule_origin rules = 
		match rhs with
	| [] -> true
	| _ -> if (is_terminal (List.hd rhs)) then is_terminal_rule (List.tl rhs) rule_origin rules
			(*This line searches the rest of the rules without the current rule (set_diff) to see if the current symbol resolves*)
			else if Pervasives.not(found_next_rule(get_sym_val(List.hd rhs))(set_diff rules (rule_origin::[]))) then false
										(*gets the rhs of the next rule that is found*)
			else if is_terminal_rule (snd (return_next_rule (get_sym_val (List.hd rhs))(set_diff rules (rule_origin::[])))) (*rhs*)
										(*gets an original copy of that rule (nonterminal rhs)*)
 								  (return_next_rule (get_sym_val (List.hd rhs))(set_diff rules (rule_origin::[]))) (*rule_origin*)
 								  (set_diff rules (rule_origin::[])) (*rules*)
 				then is_terminal_rule (List.tl rhs) rule_origin rules
 			else false;; 

(*create_nonterminals takes in a set of rules and returns a list that contains all non-terminals*)
let rec create_nonterminals rules rules_copy =
		match rules with
	| [] -> []
	| _ -> if is_terminal_rule (snd (List.hd rules)) (List.hd rules) rules_copy then create_nonterminals (List.tl rules) rules_copy
		else (List.hd rules)::(create_nonterminals (List.tl rules) rules_copy);;

(*filter_blind_alleys takes in a pair, 'g', and returns a grammar whose non-terminals are filtered out*)
let filter_blind_alleys g =
	let x = create_nonterminals (snd g) (snd g) in
	let y = set_diff (snd g) x in
	(fst g, y);;