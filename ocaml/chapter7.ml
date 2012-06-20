type 'a queue = ('a list * 'a list) ref;;

let create () =
  ref ([], []);;

let add queue x =
  let (front, back) = !queue in
  queue := (x :: front, back);;

let rec take queue =
  match !queue with
    (front, x :: back) ->
      queue := (front, back);
      x
  | ([], []) ->
      raise (Invalid_argument "queue is empty")
  | (front, []) ->
      queue := ([], List.rev front);
      take queue;;

(* ------------------------------ *)

let memo f =
  let table = ref [] in
  let rec find_or_apply entries x =
    match entries with
      (x', y) :: _ when x' = x -> y
    | _ :: entries ->
        find_or_apply entries x
    | [] ->
        let y = f x in
        table := (x, y) :: !table;
        y
  in
  (fun x -> find_or_apply !table x);;

(* ------------------------------ *)

let rec fib = function
    0 | 1 as i -> i
  | i -> fib (i 1) + fib (i 2);;

let time f x =
  let start = Sys.time () in
  let y = f x in
  let finish = Sys.time () in
  Printf.printf "Elapsed time: %f seconds\n" (finish -. start);
  y;;

(* ------------------------------ *)

type 'a deferred_value =
    Defered of (unit -> 'a)
  | Forsed of 'a;;

type 'a deferred = 'a deferred_value ref;;

let defer f = ref (Defered f);;

let force d =
  match !d with
    Forsed a -> a
  | Defered f ->
      let value = f () in
      d := Forsed value;
      value;;

(* ------------------------------ *)

type 'a lazy_list =
    Nil
  | Cons of 'a * 'a lazy_list
  | LazyCons of 'a * 'a lazy_list deferred;;

let nil_list = Nil;;
let cons a list =
  Cons (a, list);;

let lazy_cons a f =
  LazyCons (a, defer f);;

let is_empty = function
    Nil -> true
  | _ -> false;;

let head = function
    Nil -> raise (Invalid_argument "head")
  | Cons(h, _) | LazyCons(h, _) -> h;;

let tail = function
    Nil -> raise (Invalid_argument "tail")
  | Cons(_, next) -> next
  | LazyCons(_, lazy_next) -> force lazy_next;;

let rec (@@) l1 l2 =
  match l1, l2 with
    Nil, l | l, Nil -> l
  | _ -> lazy_cons (head l1) (fun () -> tail l1 @@ l2);;

(* ------------------------------ *)

type ('key, 'value) memo = ('key * 'value) list;;

let create_memo () = [];;

let rec memo_find table x =
  match table with
    (key', value) :: _ when key' = x -> value
  | _ :: rest -> memo_find rest x
  | [] -> None;;

let memo_add m key value =
  m := (key, value) :: !m;;

let rec memo_fib table i =
  match memo_find table i with
    Some j -> j
  | None ->
      match i with
        0 | 1 -> i
      | _ ->
          let j = memo_fib table (i - 1) + memo_fib table (i - 2) in
          memo_add table i j;
          j;;

(* ------------------------------ *)
