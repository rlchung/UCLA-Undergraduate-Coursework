let accept_all derivation string = Some (derivation, string);;

let accept_empty_suffix derivation = function
   | [] -> Some (derivation, [])
   | _ -> None;;

type dancers_nonterminals =
	| Hiphop | Madrid | Kuang | Martin;;

let dancer_grammar =
	(Hiphop, 
	function
		| Hiphop ->
			[[N Madrid; N Kuang; N Martin];
			[N Madrid]]
		| Madrid ->
			[[T"Latch"];
			 [T"Stay With Me"]]
		| Kuang ->
			[[T"Real Love"]]
		| Martin ->
			[[T"773"];
			 [T"Cocoa"]]
	);;

type food_dishes =
	| Start | Korean | Taiwanese | American | Japanese | Mexican;;


let food_grammar =
	(Start,
	function
		| Start ->
			[[N Korean; N Taiwanese; N American];
			 [N Japanese; N Mexican]]
		| Korean ->
			[[T "Kalbi"];[T "Kimchi"];[T "Soondubu"];
			 [T "Ramyun"];
			 [T "Chapchae"]
			]
		| Taiwanese ->
			[[T "Beef Noodle Soup"; T "Dandan Mein"];
			 [T "Beef Bao"; T "Chasu Bao"; T "Xiaolong Bao"];
			 [T "Scallion Pancake"; T "Stinky Tofu"]
			]
		| American ->
			[[T "Hamburger"; T "Pizza"];
			 [T "Jambalaya"; T "Gumbo"];
			 [T "Onion Rings"; T "Fries"]
			]
		| Japanese ->
			[[T "Sukiyaki"; T "Donburi"; T "Gyudon"];
			 [T "Takoyaki"; T "Onigiri"; T "Tempura"];
			 [T "Ramen"; T "Udon"]
			]
		| Mexican ->
			[[T "Tacos"; T "Burritos"; T "Tortas"];
			 [T "Menudo"];
			 [T "Horchata"; T "Mexi-Coke"]
			]
	);;

let test0 =
	((parse_prefix dancer_grammar accept_all ["Talk Dirty"]) = None);; 

let test1 =
	((parse_prefix food_grammar accept_empty_suffix ["Kalbi";"Stinky Tofu";"Gumbo"])
	= None);;
