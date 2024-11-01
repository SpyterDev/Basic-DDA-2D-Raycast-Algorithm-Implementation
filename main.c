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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define map_x 10
#define map_y 10
#define render_distance 10

enum Collision_Type {
    Colliable=1, Visible=2
};

typedef uint8_t _Tile_Properties;

_Tile_Properties tile_index[] = {0, Visible};


typedef struct {
    float x;
    float y;
    float angle;
    float temp_float;
} _Camera;

typedef struct {
    float distance;
    float plane_distance; 
} _Ray;

unsigned char map_tilemap[map_x][map_y] = { {1,1,1,1,1,1,1,1,1,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,0,0,0,0,0,0,0,0,1},
                                            {1,1,1,1,1,1,1,1,1,1} };

float absf(float x) {
    return x < 0 ? -x : x; 
}
_Ray raycast_dda_cast(_Camera * camera, enum Collision_Type target) {
    // Trig Def
    register float tan_theta = tanf(camera -> temp_float);
    register float sine_theta = sinf(camera -> temp_float);
    register float cos_theta = cosf(camera -> temp_float);
    // Vertical Line Check
    float adjusted_sine_angle = camera->temp_float - 1.570795F;
    if (adjusted_sine_angle < 0) adjusted_sine_angle += 6.28318F;
    float x_increment = sinf(adjusted_sine_angle) / absf(sinf(adjusted_sine_angle));
    float y_increment = tan_theta;
    float nearest_x_int = x_increment > 0 ? 1 - (camera -> x-(int)camera -> x) : (camera -> x-(int)camera -> x);
    
    float nearest_y_int = y_increment > 0 ? 1 - (camera -> y-(int)camera -> y) : (camera -> y-(int)camera -> y);
    float x = x_increment > 0 ? camera -> x + x_increment : (int)camera -> x;
    float y = camera -> y + nearest_x_int * tan_theta;
    int8_t collision_offset_1 = x_increment != 1 ? -1 : 0;
    
    float distance_1 = absf(nearest_x_int / cos_theta);
    float distance_1_increment = absf(y_increment / sine_theta);
    
    while (distance_1 < render_distance) {
        if (tile_index[map_tilemap[(int)x+collision_offset_1][(int)y]] & target) break;
        x += x_increment;
        y += y_increment;
        distance_1 += distance_1_increment;
    }

    // Horizontal Line Check
    y_increment = sine_theta / absf(sine_theta) > 0 ? 1 : -1;
    x_increment = 1 / tan_theta;
    
    x = camera -> x + nearest_y_int / tan_theta;
    y = y_increment ? camera -> y + nearest_y_int : camera -> y;
    float distance_2 = absf(nearest_y_int / sine_theta);
    float distance_2_increment = absf(1 / sine_theta);
    int8_t collision_offset_2 = y_increment != 1 ? -1 : 0;
    while (distance_2 < render_distance) {
        if (tile_index[map_tilemap[(int)x][(int)y+collision_offset_2]] & target) break;
        x += x_increment;
        y += y_increment;
        printf("Part 2: %f, %f\n", x, y);
        distance_2 += distance_2_increment;
    }
    return (_Ray){distance_1 < distance_2 ? distance_1 : distance_2, 
                  distance_1 < distance_2 ? cosf(distance_1*camera->angle-camera->temp_float) : 
                  cosf(distance_2*camera->angle-camera->temp_float)};
}


int main(void) {
    _Camera cam_test = {2.5,2,0,3.14};
    raycast_dda_cast(&cam_test, Visible);
    return EXIT_SUCCESS;
}