type 'some_type tree =
    Node of 'some_type * 'some_type tree * 'some_type tree
  | Leaf;;

let rec count = function
    Leaf -> 0
  | Node (_, left, right) -> (count left) + (count right) + 1;;

let empty = Leaf;;

let rec insert x = function
    Leaf -> Node (x, Leaf, Leaf)
  | Node (y, left, right) as node ->
      if x < y then
        Node (y, (insert x left), right)
      else if x > y then
          Node (y, left, (insert x right))
      else
        (* if such node already exists *)
        node;;


let rec set_from_list = function
    [] -> empty
  | (curr :: rest) -> insert curr (set_from_list rest);;

let set = set_from_list [5;3;67;7;3;2;12;56;7];;

let rec is_in x = function
    Leaf -> false
  | Node (y, left, right) -> 
      if y = x then
        true
      else
        (is_in x left) || (is_in x right);;

let is = is_in 3 set;;

(* ------------------------------ *)

type color =
    Black
  | Red;;

type 'some_type rbtree =
    Node of color * 'some_type * 'some_type rbtree * 'some_type rbtree
  | Leaf;;

let rec rbmember x = function
    Leaf -> false
  | Node (_, v, left, right) ->
      (x = v) || (x < v && rbmember x left) || (x > v && rbmember x right);;

let balance = function
    Black, z, Node (Red, y, Node (Red, x, a, b), c), d
  | Black, z, Node (Red, x, a, Node (Red, y, b, c)), d
  | Black, x, a, Node (Red, z, Node (Red, y, b, c), d)
  | Black, x, a, Node (Red, y, b, Node (Red, z, c, d)) ->
      Node (Red, y, Node (Black, x, a, b), Node (Black, z, c, d))
  | a, b, c, d ->
      Node (a, b, c, d);;

let insertrb x s =
  let rec ins = function
      Leaf ->
        Node (Red, x, Leaf, Leaf)
    | Node (color, y, a, b) as s ->
      if x < y then balance (color, y, ins a, b)
      else if x > y then balance (color, y, a, ins b)
      else s
  in
  match ins s with (* guaranteed to be nonempty
                    *)
    Node (_, y, a, b) ->
      Node (Black, y, a, b)
  | Leaf ->
    raise (Invalid_argument "insert");;


(* ------------------------------ *)

type 't mylist = Nil | Cons of 't * 't mylist;;

let myreverse l =
  let rec inner_reverse reversed = function
      Nil -> reversed
    | Cons (head, rest) -> inner_reverse (Cons (head, reversed)) rest
  in
  inner_reverse Nil l;;

let rec mymap f l =
  let rec inner_map buffer = function
      Nil -> buffer
    | Cons (head, rest) -> inner_map (Cons(f head, buffer)) rest
  in
  myreverse(inner_map Nil l);;

(* append to the beginning of l2 *)
let rec myappend l1 l2 =
  let rec inner_append list = function
      Nil -> list
    | Cons (head, rest) -> inner_append (Cons (head, list)) rest
  in inner_append l2 (myreverse l1);;

let rec list_to_mylist = function    
    [] -> Nil
  | curr :: rest -> Cons (curr, list_to_mylist rest);;

(* ------------------------------ *)

type unary_number = Z | S of unary_number;;

let rec add u = function
    Z -> u
  | S j -> add (S u) j;;

let unary_to_int a =
  let rec inner_to_int inc = function
      Z -> inc
    | S j -> inner_to_int (inc + 1) j
  in inner_to_int 0 a;;

let mul a b =
  let rec sum accum a = function
      Z -> accum
    | S rest -> sum (add accum a) a rest
  in sum Z a b;;

(* ------------------------------ *)

type small = Four | Three | Two | One;;
  
let small_to_int = function
    Four -> 4
  | Three -> 3
  | Two -> 2
  | One -> 1;;

let lt_small s1 s2 =
  small_to_int s1 < small_to_int s2;;

(* ------------------------------ *)

type unop = Neg;;
type binop = Add | Sub | Mul | Div;;

type exp =
    Constant of int
  | Unary of unop * exp
  | Binary of exp * binop * exp;;

let rec eval = function
    Constant(s) -> s
  | Unary(op, expr) -> -1 * (eval expr)
  | Binary(expr1, op, expr2) ->
      let val1 = eval expr1 in
      let val2 = eval expr2 in
      match op with
        Add -> val1 + val2
      | Sub -> val1 - val2
      | Mul -> val1 * val2
      | Div -> val1 / val2;;
            
let x = eval (Binary(Unary(Neg, Binary(Constant(4), Add, Constant(3))), Mul, Constant(2)));;

(* ------------------------------ *)

type ('key, 'value) dictionary =
    Node of 'key * 'value * ('key, 'value) dictionary * ('key, 'value) dictionary
  | Leaf;;

let empty = Leaf;;
let rec find dict key =
  match dict with
    Leaf -> raise Not_found
  | Node (key', value, left, right) ->
      if key' = key then
        value
      else
        if (key' < key) then
          find left key
        else
          find right key;;

let rec add dict key value =
  match dict with
    Leaf -> Node(key, value, Leaf, Leaf)
  | Node (key', value, left, right) ->
      if key' < key then
        Node (key', value, add left key value, right)
      else if key' > key then
        Node (key', value, left, add right key value)
      else
        Node (key, value, left, right);;

(* ------------------------------ *)
