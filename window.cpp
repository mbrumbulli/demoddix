/* Copyright 2013 Humboldt University of Berlin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Mihal Brumbulli <mbrumbulli@gmail.com>
 */

#include <GL/freeglut.h>
#include "window.hpp"

void* Window::FONT 		= GLUT_BITMAP_8_BY_13; // GLUT font
int Window::POINT 		= 13; // OpenGL point size in px
int Window:: PADDING 	= 4; // pading in px
int Window:: SIZE 		= 500; // size for main_window in px

// color table 13x8
unsigned char Window::COLOR[104][3] = {
	{102, 0, 0}, {102, 51, 0}, {102, 102, 0}, {51, 102, 0}, {0, 102, 0}, {0, 102, 51}, {0, 102, 102}, {0, 51, 102}, {0, 0, 102}, {51, 0, 102}, {102, 0, 102}, {102, 0, 51}, {32, 32, 32}, 
	{153, 0, 0}, {153, 76, 0}, {153, 153, 0}, 	{76, 153, 0}, {0, 153, 0}, {0, 153, 76}, {0, 153, 153}, {0, 76, 153}, {0, 0, 153}, {76, 0, 153}, {153, 0, 153}, {153, 0, 76}, {64, 64, 64}, 
	{204, 0, 0}, {204, 102, 0}, {204, 204, 0}, {102, 204, 0}, {0, 204, 0}, {0, 204, 102}, {0, 204, 204}, {0, 102, 204}, {0, 0, 204}, {102, 0, 204}, {204, 0, 204}, {204, 0, 102}, {96, 96, 96}, 
	{255, 0, 0}, {255, 128, 0}, {255, 255, 0}, {128, 255, 0}, {0, 255, 0}, {0, 255, 128}, {0, 255, 255}, {0, 128, 255}, {0, 0, 255}, {127, 0, 255}, {255, 0, 255}, {255, 0, 127}, {128, 128, 128}, 
	{255, 51, 51}, {255, 153, 51}, {255, 255, 51}, {153, 255, 51}, {51, 255, 51}, {51, 255, 153}, {51, 255, 255}, {51, 153, 255}, {51, 51, 255}, {153, 51, 255}, {255, 51, 255}, {255, 51, 153}, {160, 160, 160}, 
	{255, 102, 102}, {255, 178, 102}, {255, 255, 102}, {178, 255, 102}, {102, 255, 102}, {102, 255, 178}, {102, 255, 255}, {102, 178, 255}, {102, 102, 255}, {178, 102, 255}, {255, 102, 255}, {255, 102, 178}, {192, 192, 192}, 
	{255, 153, 153}, {255, 204, 153}, {255, 255, 153}, {204, 255, 153}, {153, 255, 153}, {153, 255, 204}, {153, 255, 255}, {153, 204, 255}, {153, 153, 255}, {204, 153, 255}, {255, 153, 255}, {255, 153, 204}, {224, 224, 224}, 
	{255, 204, 204}, {255, 229, 204}, {255, 255, 204}, {229, 255, 204}, {204, 255, 204}, {204, 255, 229}, {204, 255, 255}, {204, 229, 255}, {204, 204, 255}, {229, 204, 255}, {255, 204, 255}, {255, 204, 229}, {255, 255, 255},
};
