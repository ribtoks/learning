module Semaphore = struct
  class semaphore initial_count initial_name =
    object (self)
      val mutable count = initial_count
      val name = initial_name
      val sync = Mutex.create()
      val cond = Condition.create()
          
      method inc n = count <- count + n
      method dec n = count <- count - n

      method signal n =
        self#log "signal";
        Mutex.lock sync;
        self#inc n;
        Condition.signal cond;
        Mutex.unlock sync

      method wait =
        self#log "wait";
        Mutex.lock sync;
        while count = 0 do
          Condition.wait cond sync
        done;
        self#dec 1;
        Mutex.unlock sync

      method log log_message =
        let _ = Printf.sprintf ("%s: %s\n") name log_message in
        flush stdout
    end
end;;
