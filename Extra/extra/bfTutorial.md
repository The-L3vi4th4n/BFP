<!DOCTYPE html>

<h1> A very simple brainfuck tutorial </h1>
<p>a supper simple brainfuck tutorial. Please note I am in no shape or form one of the best brainfuck programmers out there, so beware the quality of the content in this file. Also some stuff are implementation dependent, so they might not run everywhere but most brainfuck tools should be able to support them.</p>
<hr>
<h2>Syntax</h2>
<p>+  :  adds 1 to the current location on the strip</p>
<p>-  :  sbstracts 1 to the current location on the strip</p>
<p>>  :  increases the current location on the strip by 1</p>
<p><  :  decreases the current location on the strip by 1</p>
<p>.  :  prints the character stored on in the current location on the strip</p>
<p>,  :  takes 1 character input and places it in the current location on the strip</p>
<p>[  :  goes to it's respective ] when the data in the current location is 0, otherwise continue</p>
<p>]  :  goes to it's respective [ when the data in the current location isn't 0, otherwise continue</p>
<hr>

<h2>Conventions</h2>
<p>Brainfuck probably has many conventions but here is my aproach</p>
<ul>
    <li>make the first 31 locations in the strip scratch values or do that while following the C calling convention for registers</li>
    <li>make the program as sections where each section can interact with the others, making the code easier to write</li>
    <li>this is just a thought experiment I never used(as of yet) so I can't prove it's usefullness. My idea was making 0 a flag value and not allowing it to be stored as data directly in the strip. Instead you have to store values 1 to 255 which corresponds to values 0 to 254. While this means you can no longer store a 255 and forces you to write more code, it also lets you find things more easily in your program. You can also add a flag to make it more precise, e.g. [0][1] could represent the start of the strip and [0][2] could represent temporary storage. You can also use something like [0][255][1] where the 255 itself just means the next value is still part of the flag to extend the values beyond 1 to 255 and this can theoretically be nested for the entire strip but at that point it's useless. This means you can turn your data into sections rather than one gigantic blob. There are probably other methods that could work but are hard to actually implement, like using a page and table system like in SQL. Finally, I would like to note this would be very hard to implement with(or easy if you are good at planning and visualizing your code) because you might accidentally let something turn into a 0, so you might want a custom compiler which can ensure that this never happens but that's overkill, especially if you are coding in brainfuck just for fun.</li>
</ul>
<p>I would recomend trying out another convention/aproach and taking the things you like the most as my version isn't the best at least not for every scenario and every person</p>
<hr>

<h2>Common programs</h2>
<p>clear data  :  [-] and [+] clear the current cell(however it should be noted that [+] might not work in some brainfuck versions, but that's an implementation specific problem) and set them to 0. It is rather usefull in brainfuck.</p>
<p>transfer data  :  [->+<] is a simple example where the data is transfered to the next cell by repeatedly adding to the next cell while the data in the cell being transfered is not 0. Another example is [->+>+<<]>>[-<<+>>] which does the same thing as the previous one but ensures the original value stays the same in the cell being transfered by transfering the data back through making an extra clone of the data. This requires 3 free cells to do. There are too many variations to list so it's best to play about and learn them,</p>
<p>add/substract 2 numbers  :  the transfer system does exactly that, for example +++>++++++<[->+<] adds 3 to 6 while +++>++++++<[->-<] substracts 3 from 6. If a number goes below 0 or above 255, it overflows and has different output based on ther brainfuck implenetation(should usually be an overflow with no errors)</p>
<p>Simple multiplication  :  the transfer system can also be used to make simple multiplication like [->++<] multiples a number by 2. You can also [->[->+>+<<]>>[-<<+>>]<<<] to multiply like [x][y][x*y] (if I am not wrong). I wouldn't relly on this at all however as it limits the value from 0 - 255 max and will overflow.</p>
<p>Check if a cell is 0  :  [->+>+<<]>>[-<<+>>]+<[>-<[-]] checks if the current cell is 0 by turning a neighbouring cell into 0 if it isn't and 1 if it is. >+<[>-<[-]] can also work if you won't need the data being checked again and needs 1 cell less to use than the other version, which makes it faster overall but at the coat that the thing isn't stored.</p>
<p></p>
<hr>
