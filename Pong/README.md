This program reproduces the very first video game introduced by Atari in 1972 called "Pong". Basically, there are two paddles and a ball. The ball will bounce around the window, and the user control the paddles using the keyboard button described in the "Interacting with the Program" section.

A sample view of the program in action can be found in pong_screenshot.png 

## Compiling the program
    1. In the terminal, first type in the command: `make -f .\Makefile-win shader` to compile the shader program
    2. In the terminal, type in the command: `make -f .\Makefile-win` to compile the .exe program
    3. Run the program by using the command: .\Assignment2.exe

## Interacting with the Program
    - Player 1 (red paddle) can control the paddle horizontally by using the "LEFT" and "RIGHT" arrow keyboard buttons. 
    - Player 2 (blue paddle) can control the paddle horizontally by using the "A" and "D" keyboard buttons. 
    - Hit SPACE to place the ball back in the middle in case the ball bounces out of range. This action also places the paddles back to their original coordinates. Basically, this feature resets the game.  
    - Hit ESC to exit the program. 

## Creativity Features
    - To make the bouncing of ball of the edges more realistic, the resulting reflected vector of the ball direction once it hits an edge is calculated using the formula described in slide 40 of "Week4-BasicLinearAlgebra.pdf".
    - To make the game more challenging, the ball speed increases by 20% every 5 seconds. To compensate for it, both player's paddle movement speed is increased by 5% each time the ball speed increases. 