type ('nonterminal, 'terminal) symbol =
	| N of 'nonterminal
	| T of 'terminal;;

(*Warmup1: Write a function 'convert_grammar gram1' that returns a Homework 2-style grammar,which is converted
from the Homework 1-style grammar 'gram1'*)

let rec prod_func rule_list non_term =
	match rule_list with
	| [] -> [] (*if rule_list is empty, return empty list*)
	| (nt,rhs)::tail -> if (=) nt non_term then rhs::(prod_func tail non_term) (*recursively append*)
					else prod_func tail non_term;;

let convert_grammar gram1 = 
	match gram1 with
	(start, rules) -> (start, (prod_func rules));;

(*MAIN_FUNCTION: Write a function 'parse_prefix gram' that returns a matcher for the grammar 'gram'*)

(*Retrieves the given value for any input symbol*)

 (*let get_val symbol =
  	match symbol with
  	| (N x) -> x
  	| (T y) -> y;;

 let is_terminal symbol =
	match symbol with
	| N _ -> false
	| T _ -> true
*)

(*Validates the input fragment to make sure each element reaches a terminal
given based on the rules it is given via the grammar and production function*)

let rec valid_frag prod_rules rule_rhs acceptor deriv frag =
	match rule_rhs with
	| [] -> acceptor deriv frag
	| _ -> match frag with
			| [] -> None (*valid_frag has iterated through input frag and failed to return any acceptor*)
			| _ -> (*if Pervasives.not(is_terminal (List.hd rule_rhs))
						then parser (get_val(List.hd rule_rhs)) prod_rules (valid_frag prod_rules 
						(List.tl rule_rhs) acceptor) deriv frag
					else (if get_val(List.hd rule_rhs) = List.hd frag (*first terminal of frag resolves*)
						then valid_frag prod_rules (List.tl rule_rhs) acceptor deriv (List.tl frag)
						else None
						 )*)
					match (List.hd rule_rhs) with
					| (N x) -> (parser x prod_rules (valid_frag prod_rules (List.tl rule_rhs) acceptor) deriv frag)
					| (T y) -> if y = (List.hd frag) then (valid_frag prod_rules (List.tl rule_rhs) acceptor deriv (List.tl frag))
								else None
					

(*Goes through the production rules of a given nonterminal to see if it returns
a valid acceptor or not.*)

and iterate_rules nonterm prod_rules rhs_list acceptor deriv frag =
	match rhs_list with
	| [] -> None
	| h::t -> match (valid_frag prod_rules h acceptor (deriv@[(nonterm,h)]) frag) with
				| Some (x,y) -> Some (x,y)
				| None -> iterate_rules nonterm prod_rules t acceptor deriv frag

and parser nonterm prod_rules acceptor deriv frag =
	iterate_rules nonterm prod_rules (prod_rules nonterm) acceptor deriv frag;;

let rec parse_prefix gram acceptor frag =
	parser (fst gram) (snd gram) acceptor [] frag;;