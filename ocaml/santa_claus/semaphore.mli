module Semaphore :
  sig
    class semaphore :
      int ->
      string ->
      object
        val cond : Condition.t
        val mutable count : int
        val name : string
        val sync : Mutex.t
        method dec : int -> unit
        method inc : int -> unit
        method log : string -> unit
        method signal : ?n:int -> unit -> unit
        method wait : unit
      end
  end
