type santa_counters = {
  mutable elves : int;
  mutable reindeer : int;
  santa_sem : Semaphore.Semaphore.semaphore;
  reindeer_sem : Semaphore.Semaphore.semaphore;
  elf_mutex : Semaphore.Semaphore.semaphore;
  mutex : Semaphore.Semaphore.semaphore;
}
val new_santa_counters : santa_counters
val prepare_sleigh : unit -> unit
val help_elves : unit -> unit
val get_hitched : unit -> unit
val get_help : unit -> unit
val santa_role_func : santa_counters -> unit
val reindeer_role_func : santa_counters -> unit
val elves_role_func : santa_counters -> unit
