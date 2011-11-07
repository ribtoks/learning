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

