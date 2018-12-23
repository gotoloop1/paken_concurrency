defmodule Concurrency do
  def counter(state) do
    receive do
      {:add, x} ->
        counter(state + x)
      {:get, sender} ->
        send sender, {:ok, state}
        counter state
    end
  end
  
  def main(_) do
    pid = spawn Concurrency, :counter, [0]
    
    send pid, {:get, self()}
    receive do
      {:ok, x} ->
        IO.puts x
    end
    
    send pid, {:add, 2}
    
    send pid, {:add, 3}
    
    send pid, {:get, self()}
    receive do
      {:ok, x} ->
        IO.puts x
    end
  end
end
