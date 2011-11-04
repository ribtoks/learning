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
