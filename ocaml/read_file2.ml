let split_str separator s =
  let list = ref [] in
  let start = ref 0 in
  let () = try
    while true do
      let index = String.index_from s !start separator in
      list := (String.sub s !start (index - !start)) :: !list;
      start := index + 1
    done
  with Not_found -> list := (String.sub s !start ((String.length s) - !start)) :: !list
  in List.rev !list;;

let maybe_input_line stdin =
  try Some (input_line stdin) with
    End_of_file -> None;;

let input_lines stdin =
  let rec input lines =
    match maybe_input_line stdin with
      Some line -> input (line :: lines)
    | None -> List.rev lines
  in
  input [];;



let rec parse_list_line delim line =
  if (String.length line) > 0 then
    let parts = split_str delim line in
    parse_tokens parts
  else
    []

and parse_tokens tokens =
  let rec inner_parse_tokens buffer = function
      [] -> List.rev buffer
    | h :: t ->
        let parsed = parse_line h in
        inner_parse_tokens (parsed :: buffer) t
  in
  inner_parse_tokens [] tokens

and parse_line str =
  if (String.length str) > 1 then
    if str.[0] = '(' && str.[(String.length str) - 1] = ')' then
      let substr = String.sub str 1 ((String.length str) - 2) in
      split_str ',' substr
    else
      str :: []
  else
    str :: []
           
and parse_lines lines =
  let rec inner_parse chunks = function
      [] -> List.rev chunks
    | head :: rest ->
        let parsed = parse_list_line ';' head in
        inner_parse (parsed :: chunks) rest
  in
  inner_parse [] lines;;

let file_channel = open_in("read_file2");;
let all_lines = input_lines file_channel;;
let chunks = parse_lines all_lines;;
let () = close_in file_channel;;
