This is an implementation of john conway's game of life.  
It was made to help teach me about using fltk for graphical interfaces.  
It uses the GameOfLife class I made in "life.cpp" and "life.hpp", the Program class handles the generation of UI.  
It not crash, and has a small amount of error handling  

## dependencies
This program only has 1 dependency,  
It relies on FLTK, it should be compatible with all platforms  
Other then that it's wrriten in pure c++  

## compiling
I didn't use any build system, I used only gcc directly  
The command I used looked like this:  
` g++ life.cpp main.cpp program.cpp -o {output filepath} -L"{fltk parent directory}" -lfltk -I"{fltk include path}"  
and if on windows add the following to prevent command prompt opening  
` -Wl,-subsystem,windows  


## file format
The file format is fairly simple, consisting of 2 charetors and new lines  
every line should have the same length, a '1' and '0' represent an alive and dead cell respectivly  
A line can be any length and you can have anynumber of lines, the program will calcuate it's size at run time  
Their should be an extra new line at the end of the file so the size can be properly read  
You should also make sure to use linux line endings because if window line endings are used("\n\r") it will over estamte the file size  
I will hopefuly fix that latter.  