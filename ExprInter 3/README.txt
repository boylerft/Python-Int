README

Hard but fun project, really tested my coding skills

What I implemented:
    Everything on the where to start page on canvas, except boolean expressions.
    
What might be broken:
    I implemented if, elif, and else last, so I did not have suffiecent time to
    debug it fully. To my knowledge it is working fully.
    
    Comments and blank lines might not work with weird spacing.
    
    Blank lines with indents on them will possibly break the program.
    
    A comment that has an indent in front of it does not work. It stops 
    parsing the whole file. I didnt know if this was supposed to happen,
    or if it was supposed to keep moving on through the file.
    
    Indents and dedent error throwing is not very efficient.
    
    Indents on blank lines is weird, sometimes it works, sometimes the tokenizer
    does not recognize the token, so it throws an error.
    
    I did not have time to implement type checking on expressions. So if you try
    to do operations on a string and an int in the same expression, it will give you
    a weird output, or possibly a seg fault.
    
    Lastly, the output to the terminal when you run is not the best output. 
    It prints all of the correct print statements, but it might also print out
    unnecessary things.
    
To my knowledge, these are the things that might be broken.

I have included test cases that I know works, and ones that will not work.

Test Cases that work:
    elseTest.txt
    mInpt.txt
    inputExp.txt
Test Cases that break it:
    myInput.txt
    

// Important \\
To make sure it runs, you have to have an empty line at the bottom of the file, 
with no indents.


    
    