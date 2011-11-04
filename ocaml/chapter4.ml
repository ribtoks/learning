match 1 with
  1 -> 2
|  _ -> 3;;

match 2 with
  1 + 1 -> 3
| _ -> 4;;

let rec fib = function
  i when i < 2 -> i
| i -> fib (i - 1) + fib (i - 2);;

(* ---------------------------------------- *)

let encode_char = function
    'A' -> 'C'
  | 'B' -> 'A'
  | 'C' -> 'D'
  | 'D' -> 'B'
  | 'A' .. 'Z' as c -> c
  | _ -> raise (Invalid_argument "Not a letter!");;

let encode str =
  let rec process str i =
    let () = str.[i] <- encode_char str.[i] in
    if i > 0 then
      process str (i - 1)
    else
      str
  in
  process str (String.length str - 1);;

let str = encode "BAD1";;

(* ---------------------------------------- *)

let rec is_in list element =
  match list with
    [] -> false
  | curr :: rest -> curr = element || is_in rest element;;

let l = [1;3;4;67;84;7];;

let is = is_in l 7;;

let rec reverse buffer = function
    curr :: rest -> reverse (curr :: buffer) rest
  | [] -> buffer;;

let a = reverse [] [1;2;3;4];;

let rec reverse_map f buffer = function
    curr :: rest -> reverse_map f (f curr :: buffer) rest
  | [] -> buffer;;


let f x = 2*x;;

let map f list = reverse [] (reverse_map f [] list);;

let x = map f l;;

(* ---------------------------------------- *)

let nth i (x, y, z) =
  match i with
    1 -> x
  | 2 -> y
  | 3 -> z
  | _ -> raise (Invalid_argument "nth");;

(* ---------------------------------------- *)

let db = ["John", "x3456", 50.1;
 "Jane", "x1234", 107.3;
 "Joan", "unlisted", 12.7];;


let find_salary str =
  let rec find = function
      (name, _, salary) :: rest when name = str -> salary
    | _ :: rest -> find rest
    | [] -> raise (Invalid_argument "no such name")
  in
  find db;;

find_salary "Joan";;


let select f =
  let rec get_all buffer = function
      (name, stuff, salary) :: rest when f (name, stuff, salary) ->
        get_all ((name, stuff, salary) :: buffer) rest
    | (_,_,_) :: rest -> get_all buffer rest
    | [] -> buffer
  in
  reverse [] (get_all [] db);;

let select_func = fun (_, _, salary) -> salary < 100.0;;

select select_func;;

(* ---------------------------------------- *)

let rec append_old l1 l2 =
  match l1 with
    curr :: rest -> curr :: append_old rest l2
  | [] -> l2;;

let append l1 l2 =
  let rec inner_append l2 = function
      curr :: rest -> inner_append (curr :: l2) rest
    | [] -> l2
  in inner_append l2 (List.rev l1);;

let result = append [1;2;3] [4;5;6];;
