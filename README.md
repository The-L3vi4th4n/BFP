# BFP
A brainf*ck inspired esolang which is designed to mess with you head (and for me to make a language which is complex but codeable for a beginner). The interpreter is programmed pretty poorly, which might affect performance but I have not tested that as that is not the purpose of it (which is to mess with the user's head). BFPH is also there just for the funny and it is meant to be unprogrammable(without a lot of time and effort on your hands).



# 1 : Syntax


The syntax for BFP is intended to be weird. You can only use symbols and numbers in the language.
The list goes as follows:


## 1) Multiply 
### Multiply is used to pass on a number to the symbols below. It is unique in the fact that it starts out as 1 and adds every number given individually so:
0  -->  is 1
1  -->  is 2
2  -->  is 3
4  -->  5
...
9  -->  10
10  -->  2
11  -->  3
12 --> 4
### As you may have noticed, this causes it to be impossible to make multiply 0 so some symbols  1 from multiply. Multiply resets every non-number symbol.


## 2) < and >
### >  :  moves the location you are on the strip to the right by the value given by multiply
### <  :  moves the location you are on the strip to the left by the value given by multiply

e.g.
9>  -->  moves the location you are on the strip 10 places to the right
9<  -->  moves the location you are on the strip 10 places to the left

They wraps around if the number is less than 0 or greater than the specified size of the strip.


## 3) - and +
### -  :  subtracts the value given by multiply from the current location
### +  :  adds the value given by multiply to the current location

e.g.
9+  -->  the value at the current location on the strip increases by 10
23-  -->  the value at the current location on the strip decreases by 6


## 4) . and ,
### . : prints the value at the current location on the strip as an ascii character and can print multiple times based on the multiply
### , : prints the value at the current location on the strip as a number and can print multiple times based on the multiply

e.g.:
99999991+.  -->  A
99999992+.  -->  B
99999992+1.  -->  BB
...

99999991+,  -->  65
99999992+,  -->  66
99999992+1,  -->  6666
...


## 5) ; and : and ~
### :  :  saves your current position and jumps to the start of the strip
### ;  :  saves your current position and jumps to the end of the strip
### ~  :  returns to the last saved position

e.g.:
;+,~,  -->  10
9<3+,:+,~,  -->  414


## 6) $ and &
### $  :  inputs a ...

## --UNDER CONSTRUCTION--

# Credits:
## credits to the getch(void) function for UNIX like devices:
// Source for the function - https://stackoverflow.com/a/912796
// Posted by anon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-27, License - CC BY-SA 3.0
