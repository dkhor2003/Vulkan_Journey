# Pong
This program reproduces the very first video game introduced by Atari in 1972 called "<a href="https://en.wikipedia.org/wiki/Pong">Pong</a>", but in a much more simpler version. Basically, there are two paddles and a ball. The ball will bounce around the window, and the user control the paddles using the keyboard button described in the [Interacting with the Program](#interacting-with-the-program) section. To increase difficulty, the the ball speed increases by 20% every 5 seconds. To compensate for it, both player's paddle movement speed is increased by 5% each time the ball speed increases.

## Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/00b4250c-4a9c-41ec-86b3-367c4486ac6e 

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command `.\Pong.exe`

## Interacting with the Program
- Player 1 (red paddle) can control the paddle horizontally by using the &larr; and &rarr; arrow keyboard buttons. 
- Player 2 (blue paddle) can control the paddle horizontally by using the `A` and `D` keyboard buttons. 
- Hit `SPACE` to place the ball back in the middle in case the ball bounces out of range. This action also places the paddles back to their original coordinates. Basically, this feature resets the game.  
- Hit `ESC` to exit the program. 