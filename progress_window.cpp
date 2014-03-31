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
#include "demoddix.hpp"

// window size & position
int ProgressWindow::id = 0;
int ProgressWindow::width = RootWindow::width - 2 * Window::PADDING;
int ProgressWindow::height = 20;
int ProgressWindow::xPos = Window::PADDING;
int ProgressWindow::yPos = RootWindow::height - (ProgressWindow::height + Window::PADDING);

// create window
void ProgressWindow::Create() 
{
	ProgressWindow::id = glutCreateSubWindow(RootWindow::id, ProgressWindow::xPos, ProgressWindow::yPos, ProgressWindow::width, ProgressWindow::height);
	glClearColor(Window::BG_COLOR[0] / 255.0, Window::BG_COLOR[1] / 255.0, Window::BG_COLOR[2] / 255.0, 1.0);
	glutDisplayFunc(ProgressWindow::Display);
}

// draw progress bar
void ProgressWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// ratio for mapping pixels to coordinates
	double xRatio = 2.0 / ProgressWindow::width;
	double yRatio = 2.0 / ProgressWindow::height;
	
	// times in milliseconds
	unsigned long long now = (Demoddix::currentTime - Demoddix::beginTime) / 1000000; // in ms
	unsigned long long end = (Demoddix::endTime - Demoddix::beginTime) / 1000000; // in ms
	unsigned long long step = Demoddix::stepTime / 1000000; // in ms
	
	// calculate progress bar value and position for text showing times
	double progress = -1.0 + (2.0 * now) / end;
	char message[64];
	sprintf(message, "time = %llu (%llu) | step = %llu", now, end, step);
	double w = glutBitmapLength(Window::FONT, (const unsigned char*) message) * xRatio;
	double h = glutBitmapHeight(Window::FONT) * yRatio;
	double x = -1.0 * w / 2.0;
	double y = -1.0 * h / 2.0;
	
	// draw progress
	glColor3ub(Window::BD_COLOR[0], Window::BD_COLOR[1], Window::BD_COLOR[2]);
	glRectd(-1.0, 1.0, progress, -1.0);
	
	// draw text
	glColor3ub(Window::FG_COLOR[0], Window::FG_COLOR[1], Window::FG_COLOR[2]);
	glRasterPos2d(x, y + 4 * yRatio);
	glutBitmapString(Window::FONT, (const unsigned char*) message);
	
	glutSwapBuffers();
}

// handle window resize
void ProgressWindow::Reshape() 
{
	ProgressWindow::width = RootWindow::width - 2 * Window::PADDING;
	ProgressWindow::yPos = RootWindow::height - (ProgressWindow::height + Window::PADDING);
	
	glutSetWindow(ProgressWindow::id);
	glutPositionWindow(ProgressWindow::xPos, ProgressWindow::yPos);
	glutReshapeWindow(ProgressWindow::width, ProgressWindow::height);
	glViewport(0, 0, ProgressWindow::width, ProgressWindow::height);
	glutPostRedisplay();
}

