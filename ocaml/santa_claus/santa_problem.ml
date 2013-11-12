open Semaphore;;
open Printf;;

type santa_counters = { mutable elves : int;
                        mutable reindeer : int;
                        santa_sem : Semaphore.semaphore;
                        reindeer_sem : Semaphore.semaphore;
                        elf_mutex : Semaphore.semaphore;
                        mutex : Semaphore.semaphore };;

let new_santa_counters () = { elves = 0;
                              reindeer = 0;
                              santa_sem = new Semaphore.semaphore 0;
                              reindeer_sem = new Semaphore.semaphore 0;
                              elf_mutex = new Semaphore.semaphore 1;
                              mutex = new Semaphore.semaphore 1 };;


let prepare_sleigh () = printf "Prepare sleigh\n"; flush stdout;;
let help_elves () = printf "Help Elves\n"; flush stdout;;
let get_hitched () = printf "Get Hitched\n"; flush stdout;;
let get_help () = printf "Get Help\n"; flush stdout;;

let santa_role_func c =
  c.santa_sem#wait;

  c.mutex#wait;

  if c.reindeer = 9 then (
    prepare_sleigh ();
    c.reindeer_sem#signal 9;    
   )
  else if c.elves = 3 then
    help_elves ();

  c.mutex#signal 1;;


let reindeer_role_func c =
  c.mutex#wait;
  c.reindeer <- c.reindeer + 1;
  if c.reindeer = 9 then c.santa_sem#signal 1;
  c.mutex#signal 1;

  c.reindeer_sem#wait;

  get_hitched ();;


let elves_role_func c =
  c.elf_mutex#wait;
  c.mutex#wait;
  c.elves <- c.elves + 1;
  if c.elves = 3 then
    c.santa_sem#signal 1
  else
    c.elf_mutex#signal 1;

  get_help ();

  c.mutex#wait;
  c.elves <- c.elves - 1;
  if c.elves = 0 then c.elf_mutex#signal 1;
  c.mutex#signal 1;;


let c = new_santa_counters () in
let santa_loop () =
  printf "Starting santa loop\n";
  flush stdout;
  while true do
    santa_role_func c;
  done
in
let santa_array = [| Thread.create santa_loop () |]
and
reindeer_array = Array.init 9 (fun _ -> Thread.create reindeer_role_func c)
and
elf_array = Array.init 20 (fun _ -> Thread.create elves_role_func c)
in
Array.iter Thread.join (Array.concat [santa_array; reindeer_array; elf_array]);;

flush_all ()
