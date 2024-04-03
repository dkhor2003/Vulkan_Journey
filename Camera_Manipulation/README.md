# Camera Manipulation
This program renders a teapot and cup (available as `.obj` files [here](https://free3d.com/3d-models/obj)) in 3d space, in which users can manipulate the camera to view the scene at different perspectives. The details of how to interact with the camera is described in the [Interacting with the Program](#interacting-with-the-program) section. 

## Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/9e300a94-312f-4c45-955e-2f60240648e0 

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command `.\Camera_Manipulation.exe`

## Interacting with the Program
- There are five modes of manipulating the camera view: 

    1. <ins>**Fit All**</ins>
        - Hit the `A` key to move the camera to the left 
        - Hit the `D` key to move the camera to the right
        - Hit the `W` key to move the camera forward 
        - Hit the `S` key to move the camera backward 
        - Hit the `E` key to move the camera upward
        - Hit the `Q` key to move the camera downward 
        - Hit the &larr; arrow key to turn the camera view towards the left
        - Hit the &rarr; arrow key to turn the camera view towards the right
        - Hit the &uarr; arrow key to turn the camera view towards the top
        - Hit the &darr; arrow key to turn the camera view towards the bottom

    2. <ins>**Move the cubes**</ins>
        - Hit the &larr; arrow key to move the objects at the current scene graph node to the left
        - Hit the &rarr; arrow key to move the objects at the current scene graph node to the right
        - Hit the &uarr; arrow key to move the objects at the current scene graph node to the top
        - Hit the &darr; arrow key to move the objects at the current scene graph node to the bottom
        - Hit the `1` key to rotate the objects at the current scene graph node respective to the x-axis of the objects
        - Hit the `2` key to rotate the objects at the current scene graph node respective to the y-axis of the objects
        - Hit the `3` key to rotate the objects at the current scene graph node respective to the z-axis of the objects

- Hit `ESC` key to quit the program