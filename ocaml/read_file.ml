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

let split_list delimiter list =
  let rec split chunks curr = function
      [] -> List.rev (curr :: chunks)
    | head :: tail when head <> delimiter ->
        split chunks (head :: curr) tail
    | head :: tail ->
        split (curr :: chunks) [] tail
  in
  split [] [] list;;

let file_channel = open_in("sample_file");;
let all_lines = input_lines file_channel;;
let chunks = split_list "#" all_lines;;
let () = close_in file_channel;;
