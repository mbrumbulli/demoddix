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

#include <GL/freeglut.h>
#include "demoddix.hpp"

// window size & position
int RootWindow::id = 0;
int RootWindow::width = StateWindow::width + Window::SIZE + 3 * Window::PADDING;
int RootWindow::height = Window::SIZE + ProgressWindow::height + 3 * Window::PADDING;
int RootWindow::xPos = 0;
int RootWindow::yPos = 0;

// create window
void RootWindow::Create() 
{
	glutInitWindowPosition(RootWindow::xPos, RootWindow::yPos);
	glutInitWindowSize(RootWindow::width, RootWindow::height);
	RootWindow::id = glutCreateWindow("demoddix");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(RootWindow::Display);
	glutReshapeFunc(RootWindow::Reshape);
	
	StateWindow::Create();
	MessageWindow::Create();
	ProgressWindow::Create();
	MainWindow::Create();
}

// draw borders for all other windows
void RootWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// ratio for mapping pixels to coordinates
	double xRatio = 2.0 / RootWindow::width;
	double yRatio = 2.0 / RootWindow::height;
	
	// border color & width
	glColor3ub(100, 100, 100);
	glLineWidth(2.0);
	
	// state window border
	glBegin(GL_LINE_LOOP);
		glVertex2d(-1.0 + StateWindow::xPos * xRatio, 1.0 - (StateWindow::yPos + StateWindow::height) * yRatio); // bottom left
		glVertex2d(-1.0 + (StateWindow::xPos + StateWindow::width) * xRatio, 1.0 - (StateWindow::yPos + StateWindow::height) * yRatio); // bottom right
		glVertex2d(-1.0 + (StateWindow::xPos + StateWindow::width) * xRatio, 1.0 - StateWindow::yPos * yRatio); // top right
		glVertex2d(-1.0 + StateWindow::xPos * xRatio, 1.0 - StateWindow::yPos * yRatio); // top left
	glEnd();
	
	// message window border
	glBegin(GL_LINE_LOOP);
		glVertex2d(-1.0 + MessageWindow::xPos * xRatio, 1.0 - (MessageWindow::yPos + MessageWindow::height) * yRatio); // bottom left
		glVertex2d(-1.0 + (MessageWindow::xPos + MessageWindow::width) * xRatio, 1.0 - (MessageWindow::yPos + MessageWindow::height) * yRatio); // bottom right
		glVertex2d(-1.0 + (MessageWindow::xPos + MessageWindow::width) * xRatio, 1.0 - MessageWindow::yPos * yRatio); // top right
		glVertex2d(-1.0 + MessageWindow::xPos * xRatio, 1.0 - MessageWindow::yPos * yRatio); // top left
	glEnd();
	
	// progress window border
	glBegin(GL_LINE_LOOP);
		glVertex2d(-1.0 + ProgressWindow::xPos * xRatio, 1.0 - (ProgressWindow::yPos + ProgressWindow::height) * yRatio); // bottom left
		glVertex2d(-1.0 + (ProgressWindow::xPos + ProgressWindow::width) * xRatio, 1.0 - (ProgressWindow::yPos + ProgressWindow::height) * yRatio); // bottom right
		glVertex2d(-1.0 + (ProgressWindow::xPos + ProgressWindow::width) * xRatio, 1.0 - ProgressWindow::yPos * yRatio); // top right
		glVertex2d(-1.0 + ProgressWindow::xPos * xRatio, 1.0 - ProgressWindow::yPos * yRatio); // top left
	glEnd();
	
	// main window border
	glBegin(GL_LINE_LOOP);
		glVertex2d(-1.0 + MainWindow::xPos * xRatio, 1.0 - (MainWindow::yPos + MainWindow::height) * yRatio); // bottom left
		glVertex2d(-1.0 + (MainWindow::xPos + MainWindow::width) * xRatio, 1.0 - (MainWindow::yPos + MainWindow::height) * yRatio); // bottom right
		glVertex2d(-1.0 + (MainWindow::xPos + MainWindow::width) * xRatio, 1.0 - MainWindow::yPos * yRatio); // top right
		glVertex2d(-1.0 + MainWindow::xPos * xRatio, 1.0 - MainWindow::yPos * yRatio); // top left
	glEnd();
	
	glutSwapBuffers();
}

// handle window resize
void RootWindow::Reshape(int w, int h) 
{
	// window size cannot be less than its initial size
	static int W = RootWindow::width;
	static int H = RootWindow::height;
	if (w < W) {
		RootWindow::width = W;
	}
	else {
		RootWindow::width = w;
	}
	if (h < H) {
		RootWindow::height = H;
	}
	else {
		RootWindow::height = h;
	}
	
	StateWindow::Reshape();
	MessageWindow::Reshape();
	ProgressWindow::Reshape();
	MainWindow::Reshape();

	glutSetWindow(RootWindow::id);
	glutReshapeWindow(RootWindow::width, RootWindow::height);
	glViewport(0, 0, RootWindow::width, RootWindow::height);
	glutPostRedisplay();
}
