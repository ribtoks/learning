let f x = 2*x + 1;;

let rec sum m n f =
  if m <= n then
      f m + (sum (m + 1) n f)
  else
    0;;

sum 0 1 f;;

(* ---------------------------------------- *)

let rec (%%) a b =
  if b = 0 then
    a
  else
    if a > b then
      (a - b) %% b
    else
      a %% (b - a);;

21 %% 6;;

(* ---------------------------------------- *)

let f x = x*x - 2;;

let search f n =
  let rec bisect f a b =
    let x = (a + b) / 2 in
    if x = a then
      b
    else
      if f x < 0 then
        bisect f x b
      else
        bisect f a x
  in
  bisect f 0 n;;

let x = search f 200;;

(* ---------------------------------------- *)

let empty =
  fun key -> 0;;

let find dict key =
  dict key;;

let add dict key v =
  fun key' ->
    if key' = key then
        v
    else
      dict key'

let d = empty;;
let d = add d "key1" 1;;
let v = find d "key1";;
let d = add d "key2" 2;;
let v2 = find d "key2";;
let v1 = find d "key1";;

(* ---------------------------------------- *)

let r a b c =
  let sqr_b = b *. b in
  let c4 = 4.0 *. c in
  fun a -> sqrt (sqr_b -. c4 *. a) -. b;;

let r = r 5.0 6.0 7.0;;
let v = r 1.0;;

(* ---------------------------------------- *)

let hd s = s 0;;
let tl s = fun i -> s (i + 1);;

let s i = 2*i - 1;;

let (+:) s c =
  fun i -> s i + c;;

let (-|) x y =
  fun i -> x i - y i;;

let map s f =
  fun i -> f (s i);;

let derivative s = tl s -| s;;

let integral s =
  let rec sum i =
    if i = 0 then
      0
    else
      if i = 1 then
        hd s
      else
        s i + sum (i - 1)
  in sum;;

let s_c = integral (derivative s);;
let x = hd s_c;;
let x = hd (tl s_c);;
let x = hd (tl (tl s_c));;
let x = hd (tl (tl (tl s_c)));;
