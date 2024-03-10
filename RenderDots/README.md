# Render Dots
This is my first Vulkan as well as C++ program. Basically, this program creates an interactive window, in which users can render points with. However, there is a maximum number of points in which users can render on the screen. In this program, this number is hardcoded to a maximum of 100 points for simplicity. 

# Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/c5350150-5fd8-4511-924d-a8b173f5b3e4

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command `.\RenderDot.exe`

## Interacting with the Window screen
- Click on the window screen to render a point at that clicked location. At most 100 points can be rendered on the screen. 
- Hit `SPACE` to clear the window screen of any points. 
- Hit `ENTER` to change the color of the points. There is a total of 4 colors to change into, namely red, green, blue, and dazzling. Dazzling color in this case is just randomized RGB values for each frame, resulting in a sparkling effect. 