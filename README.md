
int i = 10;
int x = 20;
int c = x + i;


load 10i; // push int 10 on stack.
push %i; // push int 10 as variable i on stack and pop 10 itself
load 20i;
push %x;
add; // add and pop out current two vals  in a stack and store result on stack
push %c; 


------
casting

float x
int y

? z = x + y

if x is float, then init z as float
   if y is long, then add long y to z
   if y is short, then add short y to z
   if y is int, then add int y to z

if x is int,then wait for z
   if y is float, then init z as float add float y to z
   

if x is int, then wait for z
   if y is long, then init z as long and add long y to x


z = CAST(x, y)


Type(z) = WhatType(x) + WhatType(y)

