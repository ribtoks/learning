open Semaphore;;
open Printf;;

let stdout_sem = new Semaphore.semaphore 1 "stdout_sem";;
let puts s =
  stdout_sem#wait;
  Printf.printf "%s\n" s;
  flush stdout;
  stdout_sem#signal ();;

type santa_counters = { mutable elves : int;
                        mutable reindeer : int;
                        santa_sem : Semaphore.semaphore;
                        reindeer_sem : Semaphore.semaphore;
                        elf_mutex : Semaphore.semaphore;
                        mutex : Semaphore.semaphore };;

let new_santa_counters () = { elves = 0;
                              reindeer = 0;
                              santa_sem = new Semaphore.semaphore 0 "santa_sem";
                              reindeer_sem = new Semaphore.semaphore 0 "reindeer_sem";
                              elf_mutex = new Semaphore.semaphore 1 "elf_mutex";
                              mutex = new Semaphore.semaphore 1 "mutex" };;


let prepare_sleigh () = puts "Prepare sleigh";;
let help_elves () = puts "Help Elves";;
let get_hitched () = puts "Get Hitched";;
let get_help () = puts "Get Help";;

let santa_role_func c =
  c.santa_sem#wait;
  c.mutex#wait;

  if c.reindeer = 9 then (
    prepare_sleigh ();
    c.reindeer_sem#signal ~n:9 ();
   )
  else if c.elves = 3 then
    help_elves ();

  c.mutex#signal ();;


let reindeer_role_func (c, i) =
  let s = Printf.sprintf "Starting reindeer (%d)" i in
  puts s;
  
  c.mutex#wait;
  c.reindeer <- c.reindeer + 1;
  if c.reindeer = 9 then c.santa_sem#signal ();
  c.mutex#signal ();

  c.reindeer_sem#wait;

  get_hitched ();;


let elves_role_func (c, i) =
  let s = Printf.sprintf "Starting elf [%d]" i in
  puts s;
  
  c.elf_mutex#wait;
  c.mutex#wait;
  c.elves <- c.elves + 1;
  if c.elves = 3 then
    c.santa_sem#signal ()
  else
    c.elf_mutex#signal ();
  c.mutex#signal ();
  
  get_help ();

  c.mutex#wait;
  c.elves <- c.elves - 1;
  if c.elves = 0 then c.elf_mutex#signal ();
  c.mutex#signal ();;


let c = new_santa_counters () in
let santa_loop () =
  puts "Starting santa loop";
  flush stdout;
  while true do
    santa_role_func c;
  done
in
let santa_array = [| Thread.create santa_loop () |]
and
reindeer_array = Array.init 9 (fun i -> Thread.create reindeer_role_func (c, i))
and
elf_array = Array.init 20 (fun i -> Thread.create elves_role_func (c, i))
in
Array.iter Thread.join (Array.concat [santa_array; reindeer_array; elf_array]);;

flush_all ()
