module Semaphore = struct
  class semaphore =
    object (self)
      val mutable count = 0
      val sync = Mutex.create()
      val cond = Condition.create()
          
      method inc = count <- count + 1
      method dec = count <- count - 1

      method signal =
        Mutex.lock sync;
        self#inc;
        Condition.signal cond;
        Mutex.unlock sync

      method wait =
        Mutex.lock sync;
        while count = 0 do
          Condition.wait cond sync
        done;
        self#dec;
        Mutex.unlock sync
    end

  let sem = new semaphore
end;;
