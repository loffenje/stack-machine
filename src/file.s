

main begin_func:
    load 4   ; sp -> 4, <stack 1>
    store a ; sp -> null, loc a <stack 1> DISCARD 4 and STORE IT IN a
    add ; sp -> 4, loc a <stack 1> ADD CURRENT SP AND LOAD ON STACK
    load 5; sp -> 5, loc a <stack 2> LOAD 5 ON STACK
    add ; sp -> 5, loc a <stack 2> ADD CURRENT SP
    store a; sp -> null, loc a = 5 <stacl 2> STORE VAL OF ADD STORE IT IN A AND LOAD ON STACK
    load 4 ; sp -> 4, loc a = 5 <stack 3>
    load 8 ; sp -> 8, loc a = 5 <stack 4>
    store b; sp -> 8, loc a = 5 <stack 3>
    load 4; sp -> 4, loc a = 5 <stack 3>
    load 8; sp -> 8, loc a = 5 <stack 4>
    add ; sp -> 12, loc a = 5 <stack 3>
    store b ; <stack 3>
    add 5, 5
    store a
    
    
end





