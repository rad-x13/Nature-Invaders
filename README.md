# Nature Invaders

`Nature Invaders` is a shoot 'em video game developed in C, for Linux and inspired by `Space Invaders`. The purpose of this project is to code with the programming language C. The basis of the program come from the great SDL2 tutorial web site, [Parallel Realities](https://www.parallelrealities.co.uk/tutorials/).

## About the game

When the rainforest is attacked by modern civilisation, it's up to you as a virus to defend it and protect the nature on so save the humanity from itself.

## How to play

The game will open a 960x720 window with a forest background. A title screen will appear at first. Press the left control key to start. The player will be shown as a stain virus. Move it right or left using the responding right and left arrow keys. You can spread out virus by holding down the left control key. Enemies will be appear grouped. They will move alternatively from right to left and from left to right and going down between. Infect enemies to destroy them. Enemies will use vaccine back and possibly destroy you. You will score points each time you will hit an enemy. There are three kinds of enemies and each one give you a different number of points: small enemies give 10 points, medium enemies give 20 points, and large enemies give 30 points. A highscore table is shown upon the player's death. If the player has earned a highscore, he will be promoted to enter his name. The highscore table is then shown and the game can be played again. Close the window by clicking on the window's close button.      

## How to build and launch the game

The game can only be build on Linux.

The dependencies required to build the game are:
* `gcc`: language C compiler 
* `make`: utility to compile and link the program.
* `SDL2`: library that provide access to audio, keyboard, mousse, and graphics hardware. [SDL2 Library](https://www.libsdl.org/index.php)
* `SDL2_image`: image file loading library. [SDL2 Image](https://www.libsdl.org/projects/SDL_image/)`
* `SDL2_mixer`: multi-channel audio mixer library. [SDL2 Mixer](https://www.libsdl.org/projects/SDL_mixer/)

So, enter the following command in your terminal: 

    sudo apt install build-essential   
    sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev


Then, go to game directory and build it with:

    make

Finally, enjoy it with:

    ./natureinvader


## Coding

This project deals with the following coding concepts:
* Use SDL2 external library
* Use code files organisation
* Use structures
* Use array, linked list, and matrix
* Use texture cache management
* Use extern and static
* Use variable arguments function
* Use sort algorithm