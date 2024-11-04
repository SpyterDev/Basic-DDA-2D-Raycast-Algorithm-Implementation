/*
    MIT License

    Copyright (c) 2024 Nicholas A. Fraidakis

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

enum Collision_Type {
    Colliable=1, Visible=2
};

typedef uint8_t _Tile_Properties;

extern _Tile_Properties tile_index[];


typedef struct {
    float x;
    float y;
    float angle;
    float temp_float;
} _Camera;

typedef struct {
    float x;
    float y;
    float distance;
    float plane_distance; 
} _Ray;

extern uint32_t map_x; // Indicates the map's x size
extern uint32_t map_y; // Indicates the map's y size
extern uint32_t render_distance; // The target render distance
extern unsigned char ** map_tilemap; // Set to tile map (format: array (y cords) to pointer to array of rows (x cords), ex. [y][x])

extern float absf(float x); // Absolute Value of a Float
extern _Ray raycast_dda_cast(_Camera * camera, enum Collision_Type target); // raycasting the tilemap with the dda algorithm