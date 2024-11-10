/*
    MIT License

    Copyright (c) SpyterDev

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

#include "Raycast.h"

uint32_t map_x = 10;
uint32_t map_y = 10;
uint32_t render_distance = 10;

unsigned char ** map_tilemap;

float absf(float x) {
   *(uint32_t *)(&x) &= 0x7FFFFFFF; // Disable sign bit making float positive
    return x; 
}
_Ray raycast_dda_cast(_Camera * camera, enum Collision_Type target) {
    // Trig Function Def

    register float tan_theta = tanf(camera -> temp_float);
    register float sine_theta = sinf(camera -> temp_float);
    register float cos_theta = cosf(camera -> temp_float);

    // Vertical Line Check

    float adjusted_sine_angle = camera -> temp_float - 1.570795F;
    if (adjusted_sine_angle < 0) adjusted_sine_angle += 6.28318F;

    float x_increment = sinf(adjusted_sine_angle) / absf(sinf(adjusted_sine_angle)); // Indicates whether 
    float y_increment = tan_theta;

    float nearest_x_int = x_increment > 0 ? 1 - (camera -> x-(int)camera -> x) : (camera -> x-(int)camera -> x);
    float nearest_y_int = y_increment > 0 ? 1 - (camera -> y-(int)camera -> y) : (camera -> y-(int)camera -> y);

    float x1 = x_increment > 0 ? camera -> x + x_increment * nearest_x_int : (int)camera -> x;
    float y1 = camera -> y + nearest_x_int * tan_theta;

    int8_t collision_offset_1 = x_increment != 1 ? -1 : 0;

    float distance_1 = absf(nearest_x_int / cos_theta);
    float distance_1_increment = absf(y_increment / sine_theta);

    while (distance_1 < render_distance) {
        if (tile_index[map_tilemap[(int) y1][(int) x1 + collision_offset_1]] & target) break;
        x1 += x_increment;
        y1 += y_increment;
        distance_1 += distance_1_increment;
    }

    // Horizontal Line Check

    y_increment = sine_theta / absf(sine_theta);
    x_increment = 1 / tan_theta;

    float x2 = camera -> x + nearest_y_int / tan_theta;
    float y2 = y_increment ? camera -> y + nearest_y_int : camera -> y;

    float distance_2 = absf(nearest_y_int / sine_theta);
    float distance_2_increment = absf(1 / sine_theta);

    int8_t collision_offset_2 = y_increment != 1 ? -1 : 0;

    while (distance_2 < render_distance) {
        if (tile_index[map_tilemap[(int)y2+collision_offset_2][(int)x2]] & target) break;
        x2 += x_increment;
        y2 += y_increment;
        distance_2 += distance_2_increment;
    }

    // Formating and Returning Casted Ray

    return (_Ray){distance_1 < distance_2 ? x1 : x2, 
                  distance_1 < distance_2 ? y1 : y2,
                  distance_1 < distance_2 ? distance_1 : distance_2,
                  distance_1 < distance_2 ? cosf(distance_1 * camera -> angle - camera -> temp_float) : 
                  cosf(distance_2 * camera -> angle-camera -> temp_float)};
}
