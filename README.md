# BFP [Brainf*ck plus]
## [!WARNING¡] I can't guarantee that the interpreter's logic is perfect, so there could be bugs within. Also I might forget to update something after changing it, though the code as of 1/4/2026 is prbly the final version I will publish myself [¡GNINRAW!]
<p>A brainf*ck inspired esolang which is designed to mess with your head (and for me to make a language which is complex enough to have a lot of features to make it worthy for making text based stories, but codeable enough for a beginner). The interpreter is programmed pretty poorly, which might affect performance but I have not tested that as that is not the purpose of it (which is to mess with the user's head). BFPH is also there just for the funny and it is meant to be unprogrammable(without a lot of time and effort on your hands). BFPL on the otherhand is BFP but it removes a lot of the extra features and leaves only the basic brainf*ck features(moving on the strip, adding and substracting from the current location, printing out the value at the current location as a character and loops) with the added benefit of multiply which can only be accesed by loops.</p>



# 1 : Compiling to an application like thing
(note compiling can be way simpler but this turns it into an executable so it's easier to run as you only have to type ./code_name rather than ./ver "name.bfp" STRIP_SIZE MAX_PTR "output.bin" every time you run the code where ver is bfpl.c/bfp.c/bfph.c)

<p>replace:

compiler with clang/gcc<br>
<b><i>name with '"name.bfp"' with the name of your code file(doesn't even have to be a .bfp extention file)<br>
STRIP_SIZE with the size you want the strip to be<br>
MAX_PTR with the maximum amount of pointers you would like<br>
'"output.bin"' with the file which get's the location of strip[0] in memory<br>
ver with bfpl.c/bfp.c/bfph.c<br></i></b>
name with the name of your code/filename of your code<br>

the lines in italic are optional and the output file is not an option with BFPL to make it smaller and the only reason it exists is so you can do stuff like use another program to access the data and, for example, draw it on a screen using a graphics library.

compiler -DDEFAULT_PROGRAM='"name.bfp"' -DDEFAULT_STRIP=STRIP_SIZE -DDEFAULT_PTR_MAX=MAX_PTR -DDEFAULT_OUTPUT='"output.bin"' ver -o name</p>

e.g.<br>
clang -DDEFAULT_PROGRAM='".bfp"' bfp.c -o name<br>
gcc -DDEFAULT_PROGRAM='"code.txt"' -DDEFAULT_STRIP=500 -DDEFAULT_PTR_MAX=MAX_25 bfph.c -o code<br>
clang -DDEFAULT_PROGRAM='"main.bfp"' -DDEFAULT_STRIP=10000 -DDEFAULT_PTR_MAX=MAX_100 -DDEFAULT_OUTPUT='"output.bin"' bfpl.c -o main<br>

# 2 : Syntax


<p>The syntax for BFP is intended to be weird. You can only use symbols and numbers in the language.<br>
For example 'hello world' in the languages will look like:<br>
bfp/bfpl  -->  >99999998+.1'999999999991+.1'999999999998+.1'999999999998+.1'9999999999992+.
bfph   -->   unknown
The list goes as follows:</p>


## 1) Multiply 
<p>Multiply is used to pass on a number to the symbols below. It is unique in the fact that it starts out as 1 and adds every number given individually.<br>

e.g.<br>
0  -->  is 1<br>
1  -->  is 2<br>
2  -->  is 3<br>
4  -->  5<br>
...<br>
9  -->  10<br>
10  -->  2<br>
11  -->  3<br>
12 --> 4<br>

As you may have noticed, this causes it to be impossible to make multiply 0 so some symbols subtract 1 from multiply. Multiply resets every non-number symbol.</p>


## 2) < and >
<p>>  :  moves the location you are on the strip to the right by the value given by multiply<br>
<  :  moves the location you are on the strip to the left by the value given by multiply<br>

e.g.<br>
9>  -->  moves the location you are on the strip 10 places to the right<br>
9<  -->  moves the location you are on the strip 10 places to the left<br>

They wraps around if the number is less than 0 or greater than the specified size of the strip.


## 3) - and +
-  :  subtracts the value given by multiply from the current location<br>
+  :  adds the value given by multiply to the current location<br>

e.g.<br>
9+  -->  the value at the current location on the strip increases by 10<br>
23-  -->  the value at the current location on the strip decreases by 6</p>


## 4) . and ,
<p>. : prints the value at the current location on the strip as an ascii character and can print multiple times based on the multiply<br>
, : prints the value at the current location on the strip as a number and can print multiple times based on the multiply<br>

e.g.:<br>
99999991+.  -->  A<br>
99999992+.  -->  B<br>
99999992+1.  -->  BB<br>
...<br>

99999991+,  -->  65<br>
99999992+,  -->  66<br>
99999992+1,  -->  6666<br>
...</p>


## 5) ; and : and ~
<p>:  :  saves your current position and jumps to the start of the strip<br>
;  :  saves your current position and jumps to the end of the strip<br>
~  :  returns to the last saved position<br>

<p>e.g.:</p>
<p>p;+,~,  -->  10</p>
<p>9<3+,:+,~,  -->  414</p>


## 6) $ and &
<p>$  :  inputs numbers from the user based on the value of multiply<br>
&  :  inputs letter from the user based on the value of multiply in ascii<br></p>


## 7) ^ and *
<p>^  :  creates a pointer to the current line of code you are at with a pointer number set by multiply<br>
*  :  goes to a pointer number based on the multiply<br></p>


## <p>8) ? and #</p>
<p>?  :  sets a value called dottp to the current multiply value. It is not reset at all but can be changed with a new call<br>
#  :  uses the dottp value to know which pointer to go to and multiply is the amount of times the program loops</p>

e.g.:<br>
^9+>>5-<?10#

## 9) %
% : prints a newline (multiply times)<br>

e.g.:<br>
9%  → prints 10 newlines


## 10) _
`_` : flushes the output buffer (forces the screen to display the text if needed)<br>

e.g.:<br>
9999993+._11$  -->  shows ':' and asks for 3 characters/numbers


## 11) @
@ : pauses execution for multiply × 1000 microsecond<br>

e.g.<br>
<p>99999999999@  -->  pauses for 100*1000 microsecond so 0.1 second<br></p>
<p>99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@  -->  1000*1000 microsecond so 1 second</p>


## 12) !
!  -->  clears the screen by printing "\033[2J\033[H" and flushing the buffer<br>

e.g.:<br>
9999993+9.%9,99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999@!  -->  prints '::::::::::' followed by a newline and '58585858585858585858' then clears the buffer after 1 second<br>
<details>
<summary>Click to see example: prints '::::::::::' followed by a newline and '58585858585858585858' then clears after 5 seconds(writen in 1's as I was too lazy to actually calculate the number with 9)</summary>

9999993+9.%9,1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111@! </details>


## 13) =
=  :  generates a random number from zero to multiply-1(if x was the number, 0<=x<10) and sets it to the current location on the strip<br> 

e.g.:<br>
99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999=,  -->  undefined behaviour(could return any number in the range)


## 14) ' and "
'  :  fills every location on the strip from the current one up till the one multiply-1 infront of it with 0. If multiply is 0, it clears the entire strip<br>
"  :  fills every location on the strip from the current one up till the one multiply-1 infront of it with the value given to dottp. If multiply is 0, it clears the entire strip<br>

e.g.:<br>
999999993+.'999999996+.'999999993+.'9994+.'999999998+.'9999999995+.'999999996+.'999999997+.  -->  prints 'LOL QWOP' using only the first location on the strip<br>
9999993+.1?"2+,  -->  prints ':3' using only the first location on the strip


## --UNDER CONSTRUCTION--

## 15) / and | and  \

## 16) ( and )

## 17) [ and ]

## 18) { and }

## 19) `

## 20) {} as functions using pointers
## --UNDER CONSTRUCTION--

# Credits:
## credits to the getch(void) function for UNIX like devices:
<p>// Source for the function - https://stackoverflow.com/a/912796<br>
// Posted by anon, modified by community. See post 'Timeline' for change history<br>
// Retrieved 2026-03-27, License - CC BY-SA 3.0<br></p>


# Challenges:
Challenges if you are up for it.
## Medium:
<ul>
  <i>make a text based game(rpg/story/whatever you desire).</i><br>
  <i>make a simple calculator.</i><br>
  <i>bring something from brainf*uck over to BFP/BFPL/BFPH.</i><br>
</ul>

## Hard:
<ul>
  <i>make an esolang inside this esolang.</i><br>
  <i>create a game using output and another program to take the data from it and drawing that on the screen. To make it harder, you have to use BFP/BFPL/BFPH for the drawing and logic to the greatest extent possible.</i><br>
</ul> 


# End
Thanks for visiting, I hope you tried and enjoyed my esolanguages. I will try and add shout-outs to cool projects down here, but I might forget, not have time, not notice them or not be alive.
