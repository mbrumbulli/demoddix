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
 * Author: Mihal Brumbulli mbrumbulli@gmail.com
 */

#ifndef STATE_WINDOW_HPP
#define STATE_WINDOW_HPP

class State;

class StateWindow {
public:
	static void Create();
	static void Display();
	static void Reshape();
	static void OnMouseClick(int button, int state, int x, int y);
	
public:
	static int id;
	static int width;
	static int height;
	static int xPos;
	static int yPos;
	
	static int yMove;
	static unsigned long selectedState;
};

#endif
