module Semaphore :
  sig
    class semaphore :
      object
        val cond : Condition.t
        val mutable count : int
        val sync : Mutex.t
        method dec : unit
        method inc : unit
        method signal : unit
        method wait : unit
      end
    val sem : semaphore
  end
