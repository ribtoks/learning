def double_to_binary(d)
  s = ""
  while d > 0
    d *= 2
    if d >= 1
      s += '1'
      d -= 1
    else
      s += '0'
    end
  end

  if d == 1
    s += '1'
  end

  s
end

p double_to_binary(0.72)
