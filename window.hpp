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

#ifndef WINDOW_HPP
#define WINDOW_HPP

class Window {
public:
	static void* FONT;
	static int POINT;
	static int PADDING;
	static int SIZE;
	static unsigned char BG_COLOR[3];
	static unsigned char FG_COLOR[3];
	static unsigned char BD_COLOR[3];
	static unsigned char COLOR[104][3];
};

#endif
