# Camera Manipulation
This program renders a teapot and cup (available as `.obj` files [here](https://free3d.com/3d-models/obj)) in 3d space, in which users can manipulate the camera to view the scene at different perspectives. The details of how to interact with the camera is described in the [Interacting with the Program](#interacting-with-the-program) section. 

## Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/adea5511-5280-4e1b-86cc-0d4e6f96dc23

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command `.\Camera_Manipulation.exe`

## Interacting with the Program
- There are five modes of manipulating the camera view: 

    1. <ins>**Fit All**</ins>
        - Hit `F` key to center the scene on the window so that the whole scene can be viewed clearly while maintaining the scene's orientation.

    2. <ins>**Rotate**</ins>
        - Hold `R` key while dragging the mouse to rotate the scene around the axis perpendicular to the drag direction.

    3. <ins>**Pan**</ins>
        - Hold `P` key while dragging the mouse to move the entire scene towards the drag direction.

    4. <ins>**Zoom**</ins>
        - Hold `Z` key while dragging the mouse up and down the window to zoom in or out the scene respectively.

    5. <ins>**Twist**</ins>
        - Hold `T` key while dragging the mouse in the direction you want to rotate the scene around the z-axis.

- Hit `ESC` key to quit the program