# Scene Graph Node
This program renders 4 cubes in 3d space, in which users can interact with the cubes by moving or rotating them. It also has a feature where users can move the camera to look at different perspectives of the four cubes. 

## Program Demo
https://github.com/dkhor2003/Vulkan_Journey/assets/120704027/9e300a94-312f-4c45-955e-2f60240648e0 

## Compiling the program
1. Use command `make -f .\Makefile-win shader` to compile the shader program
2. Use command: `make -f .\Makefile-win` to compile the .exe program
3. Run the program by using the command: `.\SceneGraphNode.exe`

## Interacting with the Program
- The cubes are ordered in terms of this scene graph at the start of the program
                   
                                                     *ROOT
                                                    /      \
                                                GROUP1     GROUP2
                                               /     \     /    \
                                          CUBE1   CUBE2  CUBE3  CUBE4

    where the * sign indicates the current scene graph node. Applying transformation on the current scene graph node applies the transformation on all the leaf nodes (cubes) that are descendants of the current scene graph node. Hit the `N` key to traverse and assign the next node to be the current scene graph node in a pre-order manner. 

- Hit `P` key to print the current scene graph setting in the console. 

- Hit `C` key to change views. There are two views available: perspective and orthographic. 

- There are two modes of moving objects, you can switch the modes by hitting the `SPACE` key.  

    1) Move the camera
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

    2) Move the cubes
        -> Hit the &larr; arrow key to move the objects at the current scene graph node to the left
        -> Hit the &rarr; arrow key to move the objects at the current scene graph node to the right
        -> Hit the &uarr; arrow key to move the objects at the current scene graph node to the top
        -> Hit the &darr; arrow key to move the objects at the current scene graph node to the bottom
        -> Hit the `1` key to rotate the objects at the current scene graph node respective to the x-axis of the objects
        -> Hit the `2` key to rotate the objects at the current scene graph node respective to the y-axis of the objects
        -> Hit the `3` key to rotate the objects at the current scene graph node respective to the z-axis of the objects

- Hit `ESC` key to quit the program
