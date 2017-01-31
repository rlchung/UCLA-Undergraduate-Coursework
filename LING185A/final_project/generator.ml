(*  file: generator.ml
 creator: Richard Chung
    date: 2015-03-15 11:45 PDT
 purpose: simple generator that generates simple sentences from input language

	primary function "generate" takes in a input category and a probabilistic grammar
	and generates a list of strings for that input category based on given probabilities

	NOTE: Given probabilities for each given category must add up to 100
*)

type ('nonterminal, 'terminal) symbol =
	| NT of 'nonterminal
	| T of 'terminal;;

type lexical_categories = 
	| TP | CP | DP | NP | VP | AdvP | AP | PP | V | N | D | P | Adv | A;;

(*is_terminal determines if a given symbol is a terminal symbol or not*)
let is_terminal symbol =
	match symbol with
	| NT _ -> false
	| T _ -> true;;

(*get_nonterm_val returns the value of a nonterminal symbol*)
let get_nonterm_val symbol =
	match symbol with
	| NT a -> a;;

(*get_term_val returns the string value of a terminal symbol*)
let get_term_val symbol =
	match symbol with
	| T a -> a;;

(*first/second/third returns the respective nth value of a 3-item tuple*)
let first (a,_,_) = a;;
let second (_,a,_) = a;;
let third (_,_,a) = a;;

(*set_diff returns a list containing the set difference between 2 lists*)
let rec set_diff a b =
	match a with
	| [] -> []
	| _ -> if Pervasives.not(List.mem (List.hd a) b) then (List.hd a)::set_diff(List.tl a) b 
		   else set_diff(List.tl a) b;;

(*found_next_rule determines if the given 'value' resolves to a possible rule within 'rules'*)
let rec found_next_rule value rules =
	match rules with
	| [] -> false
	| _ -> if (=) (first (List.hd rules)) value then true
		else found_next_rule value (List.tl rules);;

(*return_next_rule takes in: a lexical category and a set of grammar rules; 

  returns: the next rule that satisfies the 'value' element
*)

let rec return_next_rule value rules =
	if (=) (first (List.hd rules)) value then (List.hd rules)
else (return_next_rule value (List.tl rules));;

(*is_terminal_subrule determines if the given subrule, such as N Expr, terminates*)
(*
let rec is_terminal_subrule subrule rhs rules =
	if (=) rhs [] then true
else if (is_terminal subrule) then
else if Pervasives.not(found_next_rule(extract_value_symbol subrule)) then false
else*)

(*is_terminal_rule takes in: a rhs, the rule the rhs derives from, and a set of rules. 

  returns: boolean depending on whether the given rule terminates or not
  *)
let rec is_terminal_rule rhs rule_origin rules = 
		match rhs with
	| [] -> true
	| _ -> if (is_terminal (List.hd rhs)) then is_terminal_rule (List.tl rhs) rule_origin rules
			(*This line searches the rest of the rules without the current rule (set_diff) to see if the current symbol resolves*)
			else if Pervasives.not(found_next_rule(get_nonterm_val(List.hd rhs))(set_diff rules (rule_origin::[]))) then false
										(*gets the rhs of the next rule that is found*)
			else if is_terminal_rule (second (return_next_rule (get_nonterm_val (List.hd rhs))(set_diff rules (rule_origin::[])))) (*rhs*)
										(*gets an original copy of that rule (nonterminal rhs)*)
 								  (return_next_rule (get_nonterm_val (List.hd rhs))(set_diff rules (rule_origin::[]))) (*rule_origin*)
 								  (set_diff rules (rule_origin::[])) (*rules*)
 				then is_terminal_rule (List.tl rhs) rule_origin rules
 			else false;; 


(*generate_range takes in 3 values: start [The beginning of the range, 0], in_list [A list formatted with tuples of the 
				form (Category, Prob)], and range [initial value = 0].
  returns: an int list consisting of the lower bounds of each probability specified in in_list in the form:
  		  
  		  let X = probability of a rule
			
  		  For each probability given in in_list in the form [(A1,B1,X1), (A2,B2,X2),...(AN,BN,XN)]
  		  	generate_range returns a list: [0,(0+X1),((0+X1)+X2)...,100]
*)

let rec generate_range start in_list range =
	match in_list with
	| [] -> range
	| h::t -> start::(generate_range ((+) start (third h)) t range);;


(*prob_sum takes in: an input list of rules 

  returns: the sum total of all the given probabilities
*)

let rec prob_sum in_list =
	match in_list with
	| [] -> 0
	| h::t -> (+) (third (h)) (prob_sum t);;

(*group_cat takes in: a lexical category (DP,NP,etc.), and a set of grammar rules
  
  returns: a list containing all the rules grouped under the lexcial category, cat
*)

let rec group_cat cat rules =
	match rules with
	| [] -> []
	| h::t -> if (=) cat (first h) then h::(group_cat cat t)
			  else group_cat cat t;;


(*prob_selector_helper takes in: a random number, a list of probability ranges, and a list containing
								 possible rule choices.
  returns: a rule to return based on given probabilities
 *)

let rec prob_selector_helper value range cat_rule_list =
	match cat_rule_list with
	| [] -> failwith "Invalid cat_rule_list arg"
	| h::t -> if (>=) value (List.hd range) &&
				 (<=) value ((+) (List.hd range) (third h))
			  then h
			  else prob_selector_helper value (List.tl range) t;;

(*prob_selector: takes in an input cat_rule_list [A list of rules sharing a single category formatted
				by group_cat] 
	returns: a rule to return based on given probabilities
*)

let prob_selector cat_rule_list =
	let x = (Random.int 100)
	and y = (generate_range 0 cat_rule_list [])
		in if not ((=) (prob_sum cat_rule_list) 100) then failwith "Invalid input probabilities: Does not sum to 100" 
		   else prob_selector_helper x y cat_rule_list;;

(*expand takes in: a right-hand-side consisting of lexical categories and a set of grammar rules

  returns: a list of strings generated based on given probabilities
*)

let rec expand rhs prob_grammar_rules =
	match rhs with
	| [] -> []
	| h::t -> if (is_terminal h) then (get_term_val h)::(expand t prob_grammar_rules)
				else List.append (expand_helper (get_nonterm_val h) prob_grammar_rules) (expand t prob_grammar_rules)

and expand_helper cat prob_grammar_rules =
 	let x = (second (prob_selector (group_cat cat prob_grammar_rules)))
 	in expand x prob_grammar_rules;;

(*generate takes in: a start category and a set of grammar_rules

  returns: a list of strings generated from the start_category based on given probabilities
*)
let generate start_cat prob_grammar_rules =
	let x = (second (prob_selector (group_cat start_cat prob_grammar_rules)))
	in expand x prob_grammar_rules;;

(*SAMPLE TESTS
let lang1 =
   [TP, [NT DP; NT VP], 100;
    DP, [NT D; NT NP], 100;
    NP, [NT N], 80;
    NP, [NT AP; NT NP], 20;
    AP, [NT A], 100;
    VP, [NT V], 60;
    VP, [NT VP; NT AdvP], 40;
    AdvP, [NT Adv], 100;
    D, [T "The"], 50;
    D, [T "A"], 50;
    N, [T "child"], 10;
    N, [T "wolf"], 30;
    N, [T "monster"], 25;
    N, [T "mother"], 35;
    V, [T "runs"], 30;
	V, [T "jumps"], 30;
	V, [T "eats"], 20;
	V, [T "sleeps"], 20;
	A, [T "golden"], 40;
	A, [T "godly"], 20;
	A, [T "beautiful"], 40;
	Adv, [T "quickly"], 35;
	Adv, [T "quietly"], 35;
	Adv, [T "meticulously"], 30];;

generate TP lang1;;
generate TP lang1;;
generate TP lang1;;
generate TP lang1;;
generate TP lang1;;

generate DP lang1;;
generate DP lang1;;

generate VP lang1;;
generate VP lang1;;

generate AdvP lang1;;
generate AdvP lang1;;

generate AP lang1;;
generate AP lang1;;
*)