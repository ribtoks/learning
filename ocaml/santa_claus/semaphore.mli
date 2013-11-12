module Semaphore :
  sig
    class semaphore :
      int ->
      object
        val cond : Condition.t
        val mutable count : int
        val sync : Mutex.t
        method dec : int -> unit
        method inc : int -> unit
        method signal : int -> unit
        method wait : unit
      end
    val sem : semaphore
  end
