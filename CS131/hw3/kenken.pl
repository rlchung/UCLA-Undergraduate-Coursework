/**************************************************************************************************

PERFORMANCE TIME FOR KENKEN USING PREDICATE "fd_set_vector_max(255), kenken_4(N,C), kenken(N,C,T)."

Memory               limit         in use            free

   trail  stack      16383 Kb            5 Kb        16378 Kb
   cstr   stack      16383 Kb            9 Kb        16374 Kb
   global stack      32767 Kb            6 Kb        32761 Kb
   local  stack      16383 Kb            2 Kb        16381 Kb
   atom   table      32768 atoms      1803 atoms     30965 atoms

Times              since start      since last

   user   time       0.519 sec       0.519 sec
   system time       0.015 sec       0.015 sec
   cpu    time       0.534 sec       0.534 sec
   real   time     456.093 sec     456.093 sec

PERFORMANCE TIME FOR PLAIN_KENKEN USING PREDICATE "kenken_4(N,C), plain_kenken(N,C,T)."

Memory               limit         in use            free

   trail  stack      16383 Kb            7 Kb        16376 Kb
   cstr   stack      16383 Kb           10 Kb        16373 Kb
   global stack      32767 Kb            6 Kb        32761 Kb
   local  stack      16383 Kb            4 Kb        16379 Kb
   atom   table      32768 atoms      1803 atoms     30965 atoms

Times              since start      since last

   user   time       0.777 sec       0.258 sec
   system time       0.016 sec       0.001 sec
   cpu    time       0.793 sec       0.259 sec
   real   time     553.956 sec      17.585 sec

**************************************************************************************************/

%initialize KenKen table

kenken(N, C, T) :-
	start_table(N, T),
	apply_constraints(C, T),
	label(T),
	statistics.

%Verifies that there are N rows and that each row has N elements

start_table(N, Table) :-
	length(Table, N),
	start_rows(N, Table),
	distinct_table(Table).

%Verifies that each row consists of N elements and are numbered 1-N

start_rows(N, []).
start_rows(N, [H | T]) :-
	length(H, N),
	fd_domain(H, 1, N), %modify this for plain_kenken
	start_rows(N, T).

label([]).
label([H|T]) :-
	fd_labeling(H),
	label(T).

%Transpose predicate based on clpfd module (column checking)

transpose([], []).
transpose([F|Fs], Ts) :-
    transpose(F, [F|Fs], Ts).

transpose([], _, []).
transpose([_|Rs], Ms, [Ts|Tss]) :-
        lists_firsts_rests(Ms, Ts, Ms1),
        transpose(Rs, Ms1, Tss).

lists_firsts_rests([], [], []).
lists_firsts_rests([[F|Os]|Rest], [F|Fs], [Os|Oss]) :-
    lists_firsts_rests(Rest, Fs, Oss).

%distinct_table checks if each value in rows/columns are distinct

distinct_rows([]).
distinct_rows([H|T]) :-
	fd_all_different(H), %modify this for plain_kenken
	distinct_rows(T).

distinct_table(Table) :-
	distinct_rows(Table),
	transpose(Table, Transposed_table),
	distinct_rows(Transposed_table).

%retrieve gets the element from a given I-J position

get(I-J, Table, Element) :-
	nth1(I, Table, Row),
	nth1(J, Row, Element).

%apply_constraints matches and applies the correct constraint

apply_constraints([], _).
apply_constraints([Constraint|T], Table) :-
	match_constraint(Constraint, Table),
	apply_constraints(T, Table).

%Constraint matches
match_constraint(+(Sum, List), Table) :- add(Sum, List, Table, 0).
match_constraint(-(Diff, J, K), Table) :- difference(Diff, J, K, Table).
match_constraint(*(Prod, List), Table) :- multiplication(Prod, List, Table, 1).
match_constraint(/(Quotient, J, K), Table) :- division(Quotient, J, K, Table).

add(Sum, [], Table, Total) :-
	Sum #= Total.
add(Sum, [H|T], Table, Total) :-
	get(H, Table, Value),
	add(Sum, T, Table, Total + Value).

difference(Diff, J, K, Table) :-
	get(J, Table, V1),
	get(K, Table, V2),
	check_difference(V1, V2, Diff).
check_difference(V1, V2, Diff) :- Diff #= V1 - V2.
check_difference(V1, V2, Diff) :- Diff #= V2 - V1.

multiplication(Prod, [], Table, Total) :-
	Prod #= Total.
multiplication(Prod, [H|T], Table, Total) :-
	get(H, Table, Value),
	multiplication(Prod, T, Table, Total * Value).

division(Quotient, J, K, Table) :-
	get(J, Table, V1),
	get(K, Table, V2),
	check_division(V1, V2, Quotient).
check_division(V1, V2, Diff) :- Diff #= V1 / V2.
check_division(V1, V2, Diff) :- Diff #= V2 / V1.

%DEFINE plain_kenken

plain_kenken(N, C, T) :-
	start_table(N, T),
	maplist(permutation(L),T),
	%plain_apply_constraints(C, T).
	apply_constraints(C,T),
	statistics.

%plain_start initiates the table

plain_start_table(N, Table) :-
	length(Table, N),
	plain_start_rows(N, Table),
	plain_distinct_table(Table).

%Checks if each element of an input list is between 1 and N

check_domain([], Upper_bound).
check_domain([H|T], Upper_bound) :-
	between(1,Upper_bound,H),
	check_domain(T, Upper_bound).

plain_start_rows(N, []).
plain_start_rows(N, [H | T]) :-
	length(H, N),
	check_domain(H, N),
	plain_start_rows(N, T).

%plain_distinct_table checks if each value in rows/columns are distinct

plain_all_different([]).
plain_all_different([H|T]) :-
	\+ member(H,T),
	plain_all_different(T).

plain_distinct_rows([]).
plain_distinct_rows([H|T]) :-
	plain_all_different(H),
	plain_distinct_rows(T).

plain_distinct_table(Table) :-
	plain_distinct_rows(Table),
	transpose(Table, Transposed_table),
	plain_distinct_rows(Transposed_table).

%plain_apply_contraints performs just like apply_constraints

plain_apply_constraints([], _).
plain_apply_constraints([Constraint|T], Table) :-
	plain_match_constraint(Constraint, Table),
	plain_apply_constraints(T, Table).

%Plain Constraint matches
plain_match_constraint(+(Sum, List), Table) :- plain_add(Sum, List, Table, 0).
plain_match_constraint(-(Diff, J, K), Table) :- plain_difference(Diff, J, K, Table).
plain_match_constraint(*(Prod, List), Table) :- plain_multiplication(Prod, List, Table, 1).
plain_match_constraint(/(Quotient, J, K), Table) :- plain_division(Quotient, J, K, Table).

plain_add(Sum, [], Table, Total) :-
	Sum is Total.
plain_add(Sum, [H|T], Table, Total) :-
	get(H, Table, Value),
	plain_add(Sum, T, Table, Total + Value).

plain_difference(Diff, J, K, Table) :-
	get(J, Table, V1),
	get(K, Table, V2),
	plain_check_difference(V1, V2, Diff).
plain_check_difference(V1, V2, Diff) :- Diff is V1 - V2.
plain_check_difference(V1, V2, Diff) :- Diff is V2 - V1.

plain_multiplication(Prod, [], Table, Total) :-
	Prod is Total.
plain_multiplication(Prod, [H|T], Table, Total) :-
	get(H, Table, Value),
	plain_multiplication(Prod, T, Table, Total * Value).

plain_division(Quotient, J, K, Table) :-
	get(J, Table, V1),
	get(K, Table, V2),
	plain_check_division(V1, V2, Quotient).
plain_check_division(V1, V2, Diff) :- Diff is div(V1,V2).
plain_check_division(V1, V2, Diff) :- Diff is div(V2,V1).

%KENKEN TESTS

kenken_testcase(
  6,
  [
   +(11, [1-1, 2-1]),
   /(2, 1-2, 1-3),
   *(20, [1-4, 2-4]),
   *(6, [1-5, 1-6, 2-6, 3-6]),
   -(3, 2-2, 2-3),
   /(3, 2-5, 3-5),
   *(240, [3-1, 3-2, 4-1, 4-2]),
   *(6, [3-3, 3-4]),
   *(6, [4-3, 5-3]),
   +(7, [4-4, 5-4, 5-5]),
   *(30, [4-5, 4-6]),
   *(6, [5-1, 5-2]),
   +(9, [5-6, 6-6]),
   +(8, [6-1, 6-2, 6-3]),
   /(2, 6-4, 6-5)
  ]
).

kenken_4(
  4,
  [
   +(6, [1-1, 1-2, 2-1]),
   *(96, [1-3, 1-4, 2-2, 2-3, 2-4]),
   -(1, 3-1, 3-2),
   -(1, 4-1, 4-2),
   +(8, [3-3, 4-3, 4-4]),
   *(2, [3-4])
  ]
).
