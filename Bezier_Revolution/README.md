# Bezier Revolution
This program provides an interface to create hollow 3D models through the concept of bezier curve and surface of revolution. The details of how to use the interface to create, edit, and view your model is described in the [Interacting with the Program](#interacting-with-the-program) section.

## Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/ccc6a3b1-a4cf-492c-a04e-7db097db6194

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command `.\Bezier_Revolution.exe`

## Interacting with the Program
- There are 2 modes of interacting with the program: **editing** and **viewing** mode, in which you can switch between the two modes using the `SPACE` key. 

    1. **Editing** mode (Mode for creating the 3D model)
        - Use mouse to click on the window screen to render a point, which serves as a control point for constructing the bezier curve. Note that only the screen above the center line can be clicked to generate points. Each succesive points generated will be connected by a line. When three or more points are generated, the bezier curve will be presented to give a guideline for how the surface of the 3D model will look like. 
        - Use mouse to click and drag on an existing control point to alter the bezier curve based on your liking. Note that you cannot move the point to the bottom half of the window. 

    2. **Viewing** mode (Mode for inspecting the 3D model by controlling the camera)
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

- Hit `B` key to hide/show the surface generated using the constructed bezier curve.
- Hit `N` key to hide/show the normal vectors of the generated surface. 
- Hit `M` key to clear everything (control points, lines, bezier curve, surface, normal vectors) on the interface. 
- Hit `C` key to switch between perspective and orthographics perspectives. 
- Hit `ESC` key to quit the program