This is an implementation of John Conway's Game of Life.  
It was made to help teach me about using FLTK for graphical interfaces.  

John Conway's Game of Life is what's known as a zero-player game.  
Cells are only dead or alive.  
You only enter the initial position and a set of simple rules will decide the outcome.

### The rules are as follows:  
- Rule 1: any cell with less than 2 neighbors dies, as if by underpopulation  
- Rule 2: any cell with more than 3 neighbors dies, as if by overpopulation  
- Rule 3: any cell with exactly 2 or 3 neighbors survives  
- Rule 4: any cell with exactly 3 neighbors comes alive, as if by reproduction  

This game was made by the mathematician John Conway and shows how simple rules can lead to complex behaviors.  
The exact fate of how a pattern will end, if it will be stable, die, flip between states, or expand forever, is impossible to find out.  
The only way to do so would be to run it and see, but this could take a theoretically infinite time.  
The Game of Life is actually Turing complete, with the right pattern you can simulate any other system, like a computer.  
You can even run the Game of Life inside the Game of Life.  

## Features
This program supports the gui creation and editing of grids.  
Grids of arbitrary size can be made and it's sate can be esaly set via the gui.  
The speed at witch the simulation runs can also be set.  
Along with this the ablity to save game states and load them allows you to not remake the same patterns over and over.  
Other then that, their's really not much that this program would need.  

## Dependencies
This program only has 1 dependency.  
It relies on FLTK and should be compatible with all platforms.  
Other than that, it's written in pure C++.  

## Compiling
I didn't use any build system, I used only GCC directly.  
The command I used looked like this:  
` g++ life.cpp main.cpp program.cpp -o {output file path} -L"{FLTK parent directory}" -lfltk -I"{FLTK include path}"  `  
and if on Windows add the following to prevent command prompt opening:  
` -Wl,-subsystem,windows  `


## File format
The file format is fairly simple, consisting of 2 characters and new lines.  
Every line should have the same length, a '1' and '0' represent an alive and dead cell respectively.  
A line can be any length, and you can have any number of lines; the program will calculate its size at runtime.  
There should be an extra new line at the end of the file so the size can be properly read.  
You should also make sure to use Linux line endings because if Windows line endings are used ("\n\r"), it will overestimate the file size.  
I will hopefully fix that later.  
The grid is stored sideways, with the top-left corner being in the bottom-left corner.  