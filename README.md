
Simple stack based virtual machine which has GC and SymbolTable.  

interprets instructions via shell.


load 10i; // push int 10 onto the stack.
push %i; // push int 10 as variable i on stack and pop 10 itself
load 20i;
push %x;
add; 
push %c; 
load 10;
load 5;
mul; 
