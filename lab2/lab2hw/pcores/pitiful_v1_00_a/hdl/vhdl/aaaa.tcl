## test for lab 6

#Signals: clk, ctrl, delay


#puts "running test..."
## Control Register Pin Layout
# 0 : allows the counter to decrement if set to '1', holds the counter at its current value if set to a '0'.
# 1 : enables interrupts if set to a '1', disables interrupts if set to a '0'.
# 2 :  if set to '1', the timer-counter reloads with the contents of the delay-value register when it reaches 0.
#			 No reload is performed if the bit is '0'.
# 3 : Reset

# force the clock to repeat
isim force add clk 1 -value 0 -time 0 -value 1 -time 10 ns -repeat 20ns
isim force add ctrl 0
isim force add delay 140
# test case: what does it look like before you load a value?
run 1ms 
# basically goes on for an eternity, since default at max value

# test case: will it generate a pulse when it shouldn't?
isim force add ctrl 1
run 1ms

# Load the delay value
isim force add ctrl 4
run 1ms

# test case: interrupt active, without increment
isim force add ctrl 2
run 1ms

#activate increment
isim force add ctrl 3
run 1ms

# Should hold value when loading under all circumstances
isim force add ctrl 7
run 200us
isim force add delay 400
run 500us
isim force add delay 140
run 300us
# note: when ctrl=7, it will still decrement as the value is being #changed. Not sure if we want that, but easy fix
isim force add ctrl 5 
#interrupt off
run 1ms

isim force add ctrl 0 
#everything off; should just chill
run 1ms
# test case: does it reset?
isim force add ctrl 8 
run 1ms
#





 

