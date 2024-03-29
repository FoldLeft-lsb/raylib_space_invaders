# Space Invaders made with Raylib and C++ 

Learning C++, I followed this tutorial

https://github.com/educ8s/CPP-Space-Invaders-Game-with-raylib 

### Setup

Install Raylib in some way, I built from source in static mode 

For now I'm using a shell script to build, I'm using clang because the editor plugin uses it

To compile 
```
chmod +x build_linux.sh
./build_linux.sh
```

Or edit the script for your platform


To execute 
```
./main 
```

Hiscore is saved to a file "highscore.txt" 

### Controls 

subject to change 

- A and D to move left and right
- Space to unpause and to shoot 
- R to reset 

Note: If the ship and obstacles don't appear, press R to reset 