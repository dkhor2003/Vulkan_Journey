This is my first Vulkan as well as C++ program. Basically, this program creates an interactive window, in which users can render points with. However, there is a maximum number of points in which users can render on the screen. In this program, this number is hardcoded to a maximum of 100 points for simplicity. 

A sample view of the window in action can be found in window_screenshot.png 

## Compiling the program
    1. In the terminal, first type in the command: make -f .\Makefile-win to compile the .exe program
    2. Run the program by using the command: .\Assignment1.exe

## Interacting with the Window screen
    - Click on the window screen to render a point at that clicked location. At most 100 points can be rendered on the screen. 
    - Hit spacebar to clear the window screen of any points. 
    - Hit Enter to change the color of the points. There is a total of 4 colors to change into, namely red, green, blue, and dazzling. Dazzling color in this case is just randomized RGB values for each frame, resulting in a sparkling effect. 

## Creativity Features
    - Instead of each rendered point having a static color, I set up another keyboard callback (ENTER key) to change the color of the points. 