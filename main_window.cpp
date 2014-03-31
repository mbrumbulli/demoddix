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

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <GL/freeglut.h>
#include "demoddix.hpp"

static const double PI = 4.0 * atan(1.0);

// window size & position
int MainWindow::id 		= 0;
int MainWindow::width 	= RootWindow::width - (StateWindow::width + 3 * Window::PADDING);
int MainWindow::height 	= RootWindow::height - (ProgressWindow::height + 3 * Window::PADDING);
int MainWindow::xPos 	= StateWindow::width + 2 * Window::PADDING;
int MainWindow::yPos 	= Window::PADDING;

double MainWindow::zoomFactor 	= 1.0; // zoom factor
double MainWindow::xMove 		= 0.0; // move view on x axis
double MainWindow::yMove 		= 0.0; // move view on y axis
double MainWindow::xBegin 		= 0.0; // starting x coordinate used to calculate xMove	
double MainWindow::yBegin 		= 0.0; // starting y coordinate used to calculate yMove
bool MainWindow::isMoving 		= false; // true if view is being moved

// create window
void MainWindow::Create() 
{
	id = glutCreateSubWindow(RootWindow::id, MainWindow::xPos, MainWindow::yPos, MainWindow::width, MainWindow::height);
	glClearColor(Window::BG_COLOR[0] / 255.0, Window::BG_COLOR[1] / 255.0, Window::BG_COLOR[2] / 255.0, 1.0);
	glutDisplayFunc(MainWindow::Display);
	glutSpecialFunc(MainWindow::OnSpecialKeyPress);
	glutKeyboardFunc(MainWindow::OnKeyPress);
	glutMouseFunc(MainWindow::OnMouseClick);
	glutMotionFunc(MainWindow::OnMouseMove);
}

// draw nodes and packets
void MainWindow::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	// modified zoom factor considering possible window resize
	double xZoom = (zoomFactor * Window::SIZE) / MainWindow::width;
	double yZoom = (zoomFactor * Window::SIZE) / MainWindow::height;
	
	// ratio for mapping pixels to coordinates
	double xRatio = 2.0 / (xZoom * MainWindow::width);
	double yRatio = 2.0 / (yZoom * MainWindow::height);
	
	// move and zoom
	glTranslated(MainWindow::xMove, MainWindow::yMove, 0.0);
	glScaled(xZoom, yZoom, 1.0);
	
	// draw packets
	// sent packets = normal line with an arrow head at destination
	// lost packets = dotted line with an arrow head at destination 
	for (auto& p: Demoddix::packetList) {
		// show packet?
		if (!Demoddix::messageList[p.message].show) {
			continue;
		}
		
		// coordinates and color
		double xSrc = Demoddix::nodeList[p.source].x;
		double ySrc = Demoddix::nodeList[p.source].y;
		double xDst = Demoddix::nodeList[p.destination].x;
		double yDst = Demoddix::nodeList[p.destination].y;
		unsigned int c = Demoddix::messageList[p.message].color;
		
		// draw line
		if (p.lost) {
			glEnable(GL_LINE_STIPPLE);
			glLineStipple (1, 0x0F0F);
		}
		glBegin(GL_LINES);
			glColor3ub(Window::COLOR[c][0], Window::COLOR[c][1], Window::COLOR[c][2]);
			glVertex2d(xSrc, ySrc);
			glVertex2d(xDst, yDst);
		glEnd();
		if (p.lost) {
			glDisable(GL_LINE_STIPPLE);
		}
		
		// arrow head coordinates
		double lineAngle = atan2(yDst - ySrc, xDst - xSrc);
		double xOffset = xSrc - xDst;
		double yOffset = ySrc - yDst;
		double scale = (Window::POINT / 2) * xRatio / std::hypot(xOffset, yOffset);
		double x = xOffset * scale + xDst;
		double y = yOffset * scale + yDst;
		double angle = PI / 9.0;
		double topAngle = lineAngle + PI + angle;
		double bottomAngle = lineAngle + PI - angle;
		double xTop = x + cos(topAngle) * Window::POINT * xRatio;
		double yTop = y + sin(topAngle) * Window::POINT * xRatio;
		double xBottom = x + cos(bottomAngle) * Window::POINT * xRatio;
		double yBottom = y + sin(bottomAngle) * Window::POINT * xRatio;
		
		// draw arrow head
		glBegin(GL_TRIANGLE_FAN);
			glColor3ub(Window::COLOR[c][0], Window::COLOR[c][1], Window::COLOR[c][2]);
			glVertex2d(x, y);
			glVertex2d(xTop, yTop);
			glVertex2d(xBottom, yBottom);
		glEnd();
	}
	
	// draw nodes
	glPointSize(Window::POINT);
	for (unsigned long i = 0; i < Demoddix::nodeList.size(); ++i) {
		Node& n = Demoddix::nodeList[i];
		if (n.fp != NULL) {
			unsigned int c = Demoddix::stateList[n.state].color;
			
			// draw node
			glBegin(GL_POINTS);
				glColor3ub(Window::COLOR[c][0], Window::COLOR[c][1], Window::COLOR[c][2]);
				glVertex2f(n.x, n.y);
			glEnd();
			
			// draw node id
			double x = n.x + 7.0 * xRatio;
			double y = n.y + 7.0 * yRatio;
			glColor3ub(Window::FG_COLOR[0], Window::FG_COLOR[1], Window::FG_COLOR[2]);
			glRasterPos2d(x, y);
			glutBitmapString(Window::FONT, (const unsigned char *) std::to_string(i).c_str());
		}
	}

	glutSwapBuffers();
}

// handle window resize
void MainWindow::Reshape() 
{
	MainWindow::width = RootWindow::width - (StateWindow::width + 3 * Window::PADDING);
	MainWindow::height = RootWindow::height - (ProgressWindow::height + 3 * Window::PADDING);
	
	glutSetWindow(MainWindow::id);
	glutReshapeWindow(MainWindow::width, MainWindow::height);
	glViewport(0, 0, MainWindow::width, MainWindow::height);
	glutPostRedisplay();
}

// handle special key press
void MainWindow::OnSpecialKeyPress(int key, int x, int y) 
{
	switch (key) {
		// rewind events
		case GLUT_KEY_LEFT:
			Demoddix::Rewind();
			break;
		// next event
		case GLUT_KEY_UP:
			Demoddix::Next();
			break;
		// forward events
		case GLUT_KEY_RIGHT:
			Demoddix::Forward();
			break;
		// previous event
		case GLUT_KEY_DOWN:
			Demoddix::Previous();
			break;
		// increase step time used by forward & rewind
		case GLUT_KEY_PAGE_UP:
			Demoddix::stepTime += 1000000; // 1 ms
			break;
		// decrease step time used by forward & rewind
		case GLUT_KEY_PAGE_DOWN:
			if (Demoddix::stepTime > 1000000) Demoddix::stepTime -= 1000000; // 1 ms
			break;
		// reset view
		case GLUT_KEY_HOME:
			MainWindow::zoomFactor = 1.0;
			MainWindow::xMove = 0.0;
			MainWindow::yMove = 0.0;
			break;
		default:
			return;
	}
	
	glutSetWindow(MainWindow::id);
	glutPostRedisplay();
	glutSetWindow(ProgressWindow::id);
	glutPostRedisplay();
}

// handle key press ('p' or 'r')
void MainWindow::OnKeyPress(unsigned char key, int x, int y)
{
	switch (key) {
		// pause tracers
		case 'p':
		case 'P':
			if (Tracer::pollCommand == "resume|\n") {
				Tracer::pollCommand = "pause|\n";
			}
			else {
				Tracer::pollCommand = "resume|\n";
			}
			break;
		// reset events
		case 'r':
		case 'R':
			Demoddix::Reset();
			glutSetWindow(MainWindow::id);
			glutPostRedisplay();
			glutSetWindow(ProgressWindow::id);
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

// handle mouse click
void MainWindow::OnMouseClick(int button, int state, int x, int y) 
{
	// ctrl + left mouse on a node opens tracer
	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			// modified zoom factor considering possible window resize
			double xZoom = (MainWindow::zoomFactor * Window::SIZE) / MainWindow::width;
			double yZoom = (MainWindow::zoomFactor * Window::SIZE) / MainWindow::height;
			
			// ratio for mapping pixels to coordinates
			double xRatio = 2.0 / (xZoom * MainWindow::width);
			double yRatio = 2.0 / (yZoom * MainWindow::height);
			
			// hot zone considering POINT size
			double x_ = (x - MainWindow::width / 2.0) * xRatio - MainWindow::xMove / xZoom;
			double y_ = (MainWindow::height / 2.0 - y) * yRatio - MainWindow::yMove / yZoom;
			double xHot = (Window::POINT / 2.0) * xRatio;
			double yHot = (Window::POINT / 2.0) * yRatio;
			
			// check if any node is in the hot zone
			for (unsigned long i = 0; i < Demoddix::nodeList.size(); ++i) {
				Node& n = Demoddix::nodeList[i];
				if ((x_ >= n.x - xHot && x_ <= n.x + xHot) && (y_ >= n.y - yHot && y_ <= n.y + yHot)) {
					Tracer::Launch(i);
					break;
				}
			}
		}
	}
	// move the view by hondling left mouse button or,
	// zoom with the mouse wheel
	else {
		// move view
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				MainWindow::xBegin = (2.0 * x) / MainWindow::width - 1.0;
				MainWindow::yBegin = 1.0 - (2.0 * y) / MainWindow::height;
				MainWindow::isMoving = true;
			}
			else {
				MainWindow::xBegin = 0.0;
				MainWindow::yBegin = 0.0;
				MainWindow::isMoving = false;
			}
		}
		// zoom
		else if (!MainWindow::isMoving) {
			if (button == 3 && state == GLUT_DOWN) {
				MainWindow::zoomFactor *= 1.1;
				glutSetWindow(MainWindow::id);
				glutPostRedisplay();
			}
			else if (button == 4 && state == GLUT_DOWN) {
				MainWindow::zoomFactor /= 1.1;
				glutSetWindow(MainWindow::id);	
				glutPostRedisplay();
			}
		}
	}
}

// handle mouse move
void MainWindow::OnMouseMove(int x, int y) 
{
	if (MainWindow::isMoving) {
		// caclulate movement based on mouse position 
		double xTemp = (2.0 * x) / MainWindow::width - 1.0;
		double yTemp = 1.0 - (2.0 * y) / MainWindow::height;
		MainWindow::xMove += xTemp - MainWindow::xBegin;
		MainWindow::yMove += yTemp - MainWindow::yBegin;
		MainWindow::xBegin = xTemp;
		MainWindow::yBegin = yTemp;
		glutSetWindow(MainWindow::id);
		glutPostRedisplay();
	}
}
