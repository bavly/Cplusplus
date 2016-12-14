# Cplusplus
C++ Tasks 

# First Problem

Consider the following recursively defined function

f(0) = 1
f(1) = 1
f(2n) = f(n)
f(2n+1) = f(n) + f(n-1)

Write a program in C++ that computes f(n) for a given n by directly
applying the recursion relation. Compute f(123456789012345678). Pay
attention to not doing integer overflows.

Measure the wall-clock time it takes the program to run. Use e.g.
the command 'time'. On my machine when compiled with -O3, the program
takes around 13 secs.

Optimize the code so that it runs faster. It's ok to make a reasonable
tradeoff with memory. The runtime goal should be in the millisecond
regime.

# Solution First Problem
 
You must see the input and think about it first Compute f(123456789012345678)

when you See something like this and you want to solve it in C++ , You will

implement Big data To accept Large inputs, then you have to think about 

how to optimized it by DP (making a map)for saving memory .

Then think about how to simplicity of these inputs 

f(0) = 1
f(1) = 1
f(2n) = f(n)
f(2n+1) = f(n) + f(n-1)

I solved it from a long time , I could not remember the simplicity form we got from these functions 

but you will see That there is pattern When you use Odd numbers , Even numbers

Solution on Recursion.CPP File


# Second Problem

Implement a thread-safe hashmap including the following features:

Constructing the map with fixed table size given upfront , Thread-safe support lookup, insertion, and deletion.

Do not use a global lock to ensure thread-safety: 
multiple writers should be able to access one instance of the map at the same time.

You are not allowed to use STL containers.

bonus points for:
   - table resizing
   - genericity (not bound to specific types)
   - good test coverage

# Solution Second Problem

My first time to see this kind of Problem I see code on internet Tried to understand it 

But need some time to solve and understood it 
