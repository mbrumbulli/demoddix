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
#include <algorithm>
#include <GL/freeglut.h>
#include "demoddix.hpp"

// window size & position
int MessageWindow::id = 0;
int MessageWindow::width = StateWindow::width;
int MessageWindow::height = (RootWindow::height - (ProgressWindow::height + 4 * Window::PADDING)) / 2;
int MessageWindow::xPos = Window::PADDING;
int MessageWindow::yPos = RootWindow::height - (ProgressWindow::height + MessageWindow::height + 2 * Window::PADDING);

int MessageWindow::yMove 						= 0; // move view on y axis (scroll through messages)
unsigned long MessageWindow::selectedMessage 	= ULONG_MAX; // index of selected message used by color chooser

// create window
void MessageWindow::Create() 
{
	MessageWindow::id = glutCreateSubWindow(RootWindow::id, MessageWindow::xPos, MessageWindow::yPos, MessageWindow::width, MessageWindow::height);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(MessageWindow::Display);
	glutMouseFunc(MessageWindow::OnMouseClick);
	
	MessageColorWindow::Create();
}

// draw messages
void MessageWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// ratio for mapping pixels to coordinates
	double xRatio = 2.0 / MessageWindow::width;
	double yRatio = 2.0 / MessageWindow::height;
	
	// window border for color chooser
	glLineWidth(2.0);
	if (MessageWindow::selectedMessage != ULONG_MAX) {
		glColor3ub(100, 100, 100);
		glBegin(GL_LINE_LOOP);
			glVertex2d(-1.0 + MessageColorWindow::xPos * xRatio, 1.0 - (MessageColorWindow::yPos + MessageColorWindow::height) * yRatio); // bottom left
			glVertex2d(-1.0 + (MessageColorWindow::xPos + MessageColorWindow::width) * xRatio, 1.0 - (MessageColorWindow::yPos + MessageColorWindow::height) * yRatio); // bottom right
			glVertex2d(-1.0 + (MessageColorWindow::xPos + MessageColorWindow::width) * xRatio, 1.0 - MessageColorWindow::yPos * yRatio); // top right
			glVertex2d(-1.0 + MessageColorWindow::xPos * xRatio, 1.0 - MessageColorWindow::yPos * yRatio); // top left
		glEnd();
	}
	
	// scroll
	glTranslated(0.0, MessageWindow::yMove * yRatio, 0.0);
	
	// draw messages
	int y = Window::PADDING;
	glPointSize(Window::POINT);
	for (auto& m: Demoddix::messageList) {
		// color
		glBegin(GL_POINTS);
			glColor3ub(Window::COLOR[m.color][0], Window::COLOR[m.color][1], Window::COLOR[m.color][2]);
			glVertex2d(-1.0 + (Window::PADDING + Window::POINT / 2.0) * xRatio, 1.0 - (y + Window::POINT / 2.0) * yRatio);
		glEnd();
		// message name and active symbol
		glColor3ub(255, 255, 255);
		glRasterPos2d(-1.0 + (2 * Window::PADDING + Window::POINT) * xRatio, 1.0 - (y + 4 + Window::POINT / 2.0) * yRatio);
		char str[256];
		if (m.active) sprintf(str, " [X] %s", m.name.c_str());
		else sprintf(str, " [ ] %s", m.name.c_str());
		glutBitmapString(Window::FONT, (const unsigned char*) str);
		m.position = y;
		y += Window::POINT + Window::PADDING;
	}
	
	glutSwapBuffers();
}

// handle window resize
void MessageWindow::Reshape() 
{
	MessageWindow::yMove = 0;
	MessageWindow::height = (RootWindow::height - (ProgressWindow::height + 4 * Window::PADDING)) / 2;
	MessageWindow::yPos = RootWindow::height - (ProgressWindow::height + MessageWindow::height + 2 * Window::PADDING);
	
	MessageColorWindow::Reshape();
	
	glutSetWindow(MessageWindow::id);
	glutPositionWindow(MessageWindow::xPos, MessageWindow::yPos);
	glutReshapeWindow(MessageWindow::width, MessageWindow::height);
	glViewport(0, 0, MessageWindow::width, MessageWindow::height);
	glutPostRedisplay();
}

// handle mouse click
void MessageWindow::OnMouseClick(int button, int state, int x, int y) 
{
	// do nothing if there is a selected message
	if (MessageWindow::selectedMessage != ULONG_MAX) {
		return;
	}
	// if there is no selected message and,
	// if ctrl + mouse left, show/hide message
	// if mouse left, then select that message and show color chooser
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		y += MessageWindow::yMove;
		for (unsigned long i = 0; i < Demoddix::messageList.size(); ++i) {
			Message& m = Demoddix::messageList[i];
			if (y >= m.position && y <= m.position + Window::POINT) {
				if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
					m.active = !m.active;
					glutSetWindow(MainWindow::id);
					glutPostRedisplay();
				}
				else {
					MessageWindow::selectedMessage = i;
					glutSetWindow(MessageColorWindow::id);
					glutShowWindow();
				}
				break;
			}
		}
	}
	// scroll up
	else if (button == 3 && state == GLUT_DOWN) {
		MessageWindow::yMove -= 4 * (Window::PADDING + Window::POINT);
		if (MessageWindow::yMove < 0) {
			MessageWindow::yMove = 0;
		}
	}
	// scroll down
	else if (button == 4 && state == GLUT_DOWN) {
		int space = Window::POINT + Window::PADDING;
		int required = Demoddix::messageList.size() * space + Window::PADDING;
		int available = StateWindow::height;
		if (required > available) {
			MessageWindow::yMove += 4 * space;
			if (MessageWindow::yMove > required - available) {
				MessageWindow::yMove = required - available;
			}
		}
	}
	
	glutSetWindow(MessageWindow::id);
	glutPostRedisplay();
}
