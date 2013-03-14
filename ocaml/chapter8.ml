type 'a ref = { mutable contents : 'a };;


let ref a = { contents = a };;
let (!) a = a.contents;;
let (:=) a b = a.contents <- b;;

(*--------------------*)

[|[]|];;

type ('a, 'b) mpair = { mutable fst : 'a; snd : 'b };;

{fst = []; snd = []};;

{fst = (); snd = 2};;
{{fst = (); snd = 2 } with fst = 1};;

(*--------------------*)

type ('key, 'value) dictionary = {
    insert : 'key -> 'value -> ('key, 'value) dictionary;
      find : 'key -> 'value
  };;

let empty =
  let rec find_func items key' =
    match items with
      (key, value) :: rest when key = key' -> value
    | _ :: rest -> find_func rest key'
    | [] -> raise Not_found
  in
  let rec new_dict items = {
    insert = insert_func items;
    find = find_func items
  }
  and insert_func items key value =
    new_dict ((key, value) :: items)
  in
  new_dict [];;
      
(*--------------------*)

type blob = {
    get : unit -> float * float;
      area : unit -> float;
        set : float * float -> unit;
          move : float * float -> unit
  };;

let new_rectangle x y w h =
  let pos = ref (x, y) in
  let rec r =
    { get = (fun () -> !pos);
      area = (fun () -> w *. h);
        set = (fun p -> pos := p);
          move = (fun (dx, dy) ->
            let (x, y) = r.get () in
            r.set (x +. dx, y +. dy))
    }
  in r;;

let rect1 = new_rectangle 0.0 0.0 1.0 1.0 in
rect1.move (1.2, 3.4);;

let rect2 = {rect1 with set = (fun _ -> ())};; (* set context is old *)
let () = rect2.move (4., 5.);;
rect2.get();;

(*--------------------*)

let string_reverse str =
  let len = String.length str in
  for i = 0 to len / 2 - 1 do
    let c = str.[i] in
    str.[i] <- str.[len - 1 - i];
    str.[len - 1 - i] <- c
  done;;

let str = "abcd";;
string_reverse str;;
str;;

(*--------------------*)

let blit src src_off dst dst_off len =
  for i = 0 to len - 1 do
    dst.(dst_off + i) <- src.(src_off + i)
  done;;


(*problem if overlaps in same array*)

let blit src src_off dst dst_off len =
  if dst_off >= src_off then
    for i = len - 1 downto 0 do
      dst.(dst_off + i) <- src.(src_off + i)
    done
  else
    for i = 0 to len - 1 do
      dst.(dst_off + i) <- src.(src_off + i)
    done;;

(*--------------------*)

let find_next_max_index array i = 
  let j = ref (i + 1) in
  let max_index = ref i in
  let max = ref array.(i) in
  while !j < (Array.length array) do
    if !max < array.(!j) then begin
      max := array.(!j);
      max_index := !j
    end;
    j := !j + 1
  done;
  !max_index;;

let insert_sort array =
  for i = 0 to (Array.length array) - 2 do
    let j = find_next_max_index array i
    in
    let x = array.(j) in
    array.(j) <- array.(i);
    array.(i) <- x
  done;;

let arr = [|4;7;2;2;24;6;7|];;
  insert_sort arr;;
arr;;
