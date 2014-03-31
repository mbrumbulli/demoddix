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

#include <climits>
#include <string>
#include <algorithm>
#include <GL/freeglut.h>
#include "demoddix.hpp"

// window size & position
int StateWindow::id = 0;
int StateWindow::width = 300;
int StateWindow::height = (RootWindow::height - (ProgressWindow::height + 4 * Window::PADDING)) / 2;
int StateWindow::xPos = Window::PADDING;
int StateWindow::yPos = Window::PADDING;

int StateWindow::yMove 						= 0; // move view on y axis (scroll through states)
unsigned long StateWindow::selectedState 	= ULONG_MAX; // index of selected state used by color chooser

// create window
void StateWindow::Create() 
{
	StateWindow::id = glutCreateSubWindow(RootWindow::id, StateWindow::xPos, StateWindow::yPos, StateWindow::width, StateWindow::height);
	glClearColor(Window::BG_COLOR[0] / 255.0, Window::BG_COLOR[1] / 255.0, Window::BG_COLOR[2] / 255.0, 1.0);
	glutDisplayFunc(StateWindow::Display);
	glutMouseFunc(StateWindow::OnMouseClick);
	
	StateColorWindow::Create();
}

// draw states
void StateWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// ratio for mapping pixels to coordinates
	double xRatio = 2.0 / StateWindow::width;
	double yRatio = 2.0 / StateWindow::height;
	
	// window border for color chooser
	glLineWidth(2.0);
	if (StateWindow::selectedState != ULONG_MAX) {
		glColor3ub(Window::BD_COLOR[0], Window::BD_COLOR[1], Window::BD_COLOR[2]);
		glBegin(GL_LINE_LOOP);
			glVertex2d(-1.0 + StateColorWindow::xPos * xRatio, 1.0 - (StateColorWindow::yPos + StateColorWindow::height) * yRatio); // bottom left
			glVertex2d(-1.0 + (StateColorWindow::xPos + StateColorWindow::width) * xRatio, 1.0 - (StateColorWindow::yPos + StateColorWindow::height) * yRatio); // bottom right
			glVertex2d(-1.0 + (StateColorWindow::xPos + StateColorWindow::width) * xRatio, 1.0 - StateColorWindow::yPos * yRatio); // top right
			glVertex2d(-1.0 + StateColorWindow::xPos * xRatio, 1.0 - StateColorWindow::yPos * yRatio); // top left
		glEnd();
	}
	
	// scroll
	glTranslated(0.0, StateWindow::yMove * yRatio, 0.0);
	
	// draw states
	int y = Window::PADDING;
	glPointSize(Window::POINT);
	for (auto& s: Demoddix::stateList) {
		if (!s.name.empty()) {
			// color
			glBegin(GL_POINTS);
				glColor3ub(Window::COLOR[s.color][0], Window::COLOR[s.color][1], Window::COLOR[s.color][2]);
				glVertex2d(-1.0 + (Window::PADDING + Window::POINT / 2.0) * xRatio, 1.0 - (y + Window::POINT / 2.0) * yRatio);
			glEnd();
			// state name and priority
			glColor3ub(Window::FG_COLOR[0], Window::FG_COLOR[1], Window::FG_COLOR[2]);
			glRasterPos2d(-1.0 + (2 * Window::PADDING + Window::POINT) * xRatio, 1.0 - (y + 4 + Window::POINT / 2.0) * yRatio);
			char str[256];
			sprintf(str, " (%2d) %s", s.priority, s.name.c_str());
			glutBitmapString(Window::FONT, (const unsigned char*) str);
			s.position = y;
			y += Window::POINT + Window::PADDING;
		}
	}
	
	glutSwapBuffers();
}

// handle window resize
void StateWindow::Reshape() 
{
	StateWindow::yMove = 0;
	StateWindow::height = (RootWindow::height - (ProgressWindow::height + 4 * Window::PADDING)) / 2;
	
	StateColorWindow::Reshape();
	
	glutSetWindow(StateWindow::id);
	glutReshapeWindow(StateWindow::width, StateWindow::height);
	glViewport(0, 0, StateWindow::width, StateWindow::height);
	glutPostRedisplay();
}

// handle mouse click
void StateWindow::OnMouseClick(int button, int state, int x, int y) 
{
	// do nothing if there is a selected state
	if (StateWindow::selectedState != ULONG_MAX) {
		return;
	}
	// if there is no selected state and,
	// if ctrl + mouse left, then increment priority
	// if mouse left, then select that state and show color chooser
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		y += StateWindow::yMove;
		for (unsigned long i = 0; i < Demoddix::stateList.size(); ++i) {
			State& s = Demoddix::stateList[i];
			if (!s.name.empty()) {
				if (y >= s.position && y <= s.position + Window::POINT) {
					if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
						if (s.priority < 99) {
							++s.priority;
						}
					}
					else {
						StateWindow::selectedState = i;
						glutSetWindow(StateColorWindow::id);
						glutShowWindow();
					}
					break;
				}
			}
		}
	}
	// if ctrl + mouse right, then decrement priority
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			y += StateWindow::yMove;
			for (auto& s: Demoddix::stateList) {
				if (!s.name.empty()) {
					if (y >= s.position && y <= s.position + Window::POINT) {
						if (s.priority > 0) {
							--s.priority;
						}
						break;
					}
				}
			}
		}
	}
	// scroll up
	else if (button == 3 && state == GLUT_DOWN) {
		StateWindow::yMove -= 4 * (Window::PADDING + Window::POINT);
		if (StateWindow::yMove < 0) {
			StateWindow::yMove = 0;
		}
	}
	// scroll down
	else if (button == 4 && state == GLUT_DOWN) {
		int space = Window::POINT + Window::PADDING;
		int required = Demoddix::stateList.size() * space + Window::PADDING;
		int available = StateWindow::height;
		if (required > available) {
			StateWindow::yMove += 4 * space;
			if (StateWindow::yMove > required - available) {
				StateWindow::yMove = required - available;
			}
		}
	}
	
	glutSetWindow(StateWindow::id);
	glutPostRedisplay();
}
