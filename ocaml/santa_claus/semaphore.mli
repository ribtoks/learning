module Semaphore :
  sig
    class semaphore :
      int ->
      'a ->
      object
        val cond : Condition.t
        val mutable count : int
        val name : 'a
        val sync : Mutex.t
        method dec : int -> unit
        method inc : int -> unit
        method signal : ?n:int -> unit -> unit
        method wait : unit
      end
  end
