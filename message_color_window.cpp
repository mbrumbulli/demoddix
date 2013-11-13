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

#include <climits>
#include <string>
#include <GL/freeglut.h>
#include "demoddix.hpp"

// window size & position
int MessageColorWindow::id = 0;
int MessageColorWindow::width = 8 * Window::POINT;
int MessageColorWindow::height = 13 * Window::POINT;
int MessageColorWindow::xPos = MessageWindow::width - (MessageColorWindow::width + Window::PADDING);
int MessageColorWindow::yPos = Window::PADDING;

// create window
void MessageColorWindow::Create() 
{
	MessageColorWindow::id = glutCreateSubWindow(MessageWindow::id, MessageColorWindow::xPos, MessageColorWindow::yPos, MessageColorWindow::width, MessageColorWindow::height);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(MessageColorWindow::Display);
	glutMouseFunc(MessageColorWindow::OnMouseClick);
	glutHideWindow();
}

// draw color chooser
void MessageColorWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// ratio for mapping pixels to coordinates
	double yRatio = 2.0 / MessageColorWindow::height;
	double xRatio = 2.0 / MessageColorWindow::width;
	
	// draw colors
	glPointSize(Window::POINT);
	glBegin(GL_POINTS);
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 13; ++y) {
			int c = x * 13 + y;
			glColor3ub(Window::COLOR[c][0], Window::COLOR[c][1], Window::COLOR[c][2]);
			glVertex2d(-1.0 + (x + 0.5) * Window::POINT * xRatio, 1.0 - (y + 0.5) * Window::POINT * yRatio);
		}
	}
	glEnd();
	
	glutSwapBuffers();
}

// handle window resize
void MessageColorWindow::Reshape() 
{
	glutSetWindow(MessageColorWindow::id);
	glutPostRedisplay();
}

// handle mouse click
void MessageColorWindow::OnMouseClick(int button, int state, int x, int y) 
{
	// left mouse sets color to selected message and hides color chooser
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x /= Window::POINT;
		y /= Window::POINT;
		if (MessageWindow::selectedMessage != ULONG_MAX) {
			Demoddix::messageList[MessageWindow::selectedMessage].color = x * 13 + y;
			MessageWindow::selectedMessage = ULONG_MAX;
		}
		glutHideWindow();
	}
	// right mouse hides color chooser
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		MessageWindow::selectedMessage = ULONG_MAX;
		glutHideWindow();
	}
	
	glutPostRedisplay();
	glutSetWindow(MessageWindow::id);
	glutPostRedisplay();
	glutSetWindow(MainWindow::id);
	glutPostRedisplay();
}
