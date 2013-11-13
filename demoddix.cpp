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

#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include <algorithm>
#include <unistd.h>
#include <GL/freeglut.h>
#include "demoddix.hpp"

const unsigned int Demoddix::bufferSize 			= 1024; // buffer size for reading
const char* Demoddix::iniFileName					= "RTDS_gen.ini"; // configuration file containing list of states, processes, semaphores, and messages

FILE* Demoddix::iniFile								= NULL; // file pointer to iniFileName
std::vector<FILE*> Demoddix::fileList				= {}; // list of trace files

std::vector<State> Demoddix::stateList				= {}; // list of states found in iniFile
std::vector<Process> Demoddix::processList			= {}; // list of processes found in iniFile 
std::vector<Semaphore> Demoddix::semaphoreList		= {}; // list of semaphores found in iniFile
std::vector<Message> Demoddix::messageList			= {}; // list of messages found in iniFile
std::vector<Node> Demoddix::nodeList				= {}; // list of nodes to be drawn
std::vector<Packet> Demoddix::packetList			= {}; // list of packets (line with arrow) to be drawn

std::vector<Event> Demoddix::eventList				= {}; // list of events		
unsigned long Demoddix::currentEvent				= 0; // index to the current event 

unsigned long long Demoddix::currentTime			= 0; // current time in ns
unsigned long long Demoddix::beginTime				= 0; // begin time in ns
unsigned long long Demoddix::endTime				= 0; // end time in ns
unsigned long long Demoddix::stepTime				= 1000000; // time to step forward (or rewind) in ns

// format for reading values from iniFile
static const char* stateF		= "%s = %u";
static const char* processF		= "%s = %u";
static const char* semaphoreF	= "%s = %u";
static const char* messageF		= "%s = %u";

// format for reading values from trace files
static const char* nodeF				= "<demoddix p2pId=\"n%lu\" x=\"%lf\" y=\"%lf\" >";
static const char* traceF				= "<%*s p2pId=\"n%lu\" time=\"%llu\"";
static const char* nodeChangedStateF	= "<nodeChangedState p2pId=\"n%lu\" time=\"%llu\" stateName=\"s%u\" prevStateName=\"s%u\" />";
static const char* packetSentF			= "<packetSent p2pId=\"n%lu\" time=\"%llu\" p2pReceiver=\"n%lu\" pktName=\"m%u\" />";
static const char* packetReceivedF		= "<packetReceived p2pId=\"n%lu\" time=\"%llu\" p2pSender=\"n%lu\" pktName=\"m%u\" />";
static const char* packetLostF			= "<packetLost p2pId=\"n%lu\" time=\"%llu\" p2pReceiver=\"n%lu\" pktName=\"m%u\" />";

// read configuration and trace files
void Demoddix::Open()
{
	char buffer[Demoddix::bufferSize];
	
	// read iniFile and fill corresponding lists
	// colors for states and messages are randomly generated [0, 104)
	while (fgets(buffer, Demoddix::bufferSize, iniFile) != NULL) {
		unsigned int id;
		char name[128];
		if (strstr(buffer, "[States]") != NULL) {
			while (fgets(buffer, Demoddix::bufferSize, iniFile) != NULL && sscanf(buffer, stateF, name, &id) == 2) {
				if (Demoddix::stateList.size() < id + 1) {
					Demoddix::stateList.resize(id + 1, State(std::string(name), rand() % 104, 1));
				}
				else {
					Demoddix::stateList[id] = State(std::string(name), rand() % 104, 1);
				}
			}
		}
		else if (strstr(buffer, "[Processes]") != NULL) {
			while (fgets(buffer, Demoddix::bufferSize, iniFile) != NULL && sscanf(buffer, processF, name, &id) == 2) {
				if (Demoddix::processList.size() < id) {
					Demoddix::processList.resize(id, Process(std::string(name)));
				}
				else {
					Demoddix::processList[id - 1] = Process(std::string(name));
				}
			}
		}
		else if (strstr(buffer, "[Semaphores]") != NULL) {
			while (fgets(buffer, Demoddix::bufferSize, iniFile) != NULL && sscanf(buffer, semaphoreF, name, &id) == 2) {
				if (Demoddix::semaphoreList.size() < id) {
					Demoddix::semaphoreList.resize(id, Semaphore(std::string(name)));
				}
				else {
					Demoddix::semaphoreList[id - 1] = Semaphore(std::string(name));
				}
			}
		}
		else if (strstr(buffer, "[Messages]") != NULL) {
			while (fgets(buffer, Demoddix::bufferSize, iniFile) != NULL && sscanf(buffer, messageF, name, &id) == 2) {
				if (Demoddix::messageList.size() < id) {
					Demoddix::messageList.resize(id, Message(std::string(name), rand() % 104));
				}
				else {
					Demoddix::messageList[id - 1] = Message(std::string(name), rand() % 104);
				}
			}
		}
	}
	
	// read trace files and fill node and event lists
	// calculate min & max coordinates needed for transformation  
	Demoddix::nodeList.resize(Demoddix::fileList.size(), Node(0, 0, 0));
	double xMin = DBL_MAX, yMin = DBL_MAX, xMax = DBL_MIN, yMax = DBL_MIN;
	for (auto fp: Demoddix::fileList) {
		fpos_t pos;
		fgetpos(fp, &pos);
		unsigned long id;
		unsigned long long time;
		double x, y;
		while (fgets(buffer, Demoddix::bufferSize, fp) != NULL) {
			if (sscanf(buffer, traceF, &id, &time) == 2) {
				Demoddix::eventList.push_back(Event(time, id, pos));
			}
			else if (sscanf(buffer, nodeF, &id, &x, &y) == 3) {
				// default state for nodes is 0
				Demoddix::nodeList[id] = Node(x, y, 0);
				xMin = fmin(x, xMin);
				yMin = fmin(y, yMin);
				xMax = fmax(x, xMax);
				yMax = fmax(y, yMax);
			}
			fgetpos(fp, &pos);
		}
		
	}
	
	// sort events by time maintaining relative position of events with the same time 
	std::stable_sort(Demoddix::eventList.begin(), Demoddix::eventList.end(), [](const Event& e1, const Event& e2) { return e1.time < e2.time; });

	// initialize times
	if (!Demoddix::eventList.empty()) {
		Demoddix::currentEvent = 0;
		Demoddix::beginTime = Demoddix::eventList.front().time;
		Demoddix::endTime = Demoddix::eventList.back().time;
		Demoddix::currentTime = Demoddix::beginTime;
	}
	
	// transform coordinates to fit screen
	double r = 2.0 / Window::SIZE; // ratio for mapping pixels to coordinates
	double p = 4.0 * Window::POINT * r; // padding
	for (auto& n: Demoddix::nodeList) {
		n.x = 2.0 * (1.0 - p) * (n.x - xMin) / (xMax - xMin) - (1.0 - p);
		n.y = 2.0 * (1.0 - p) * (n.y - yMin) / (yMax - yMin) - (1.0 - p);
	}
}

// close files
void Demoddix::Close() 
{	
	// close iniFile
	if (Demoddix::iniFile != NULL) {
		fclose(Demoddix::iniFile);
	}
	
	// close trace files
	for (auto fp: Demoddix::fileList) {
		if (fp != NULL) {
			fclose(fp);
		}
	}
}

// handle events forward until time is reached
void Demoddix::Forward() 
{
	// update current time
	Demoddix::currentTime += Demoddix::stepTime;
	if (Demoddix::currentTime > Demoddix::endTime) Demoddix::currentTime = Demoddix::endTime;
	
	// handle events until current time is reached
	char buffer[Demoddix::bufferSize];
	while (Demoddix::currentEvent < Demoddix::eventList.size()) {
		Event& e = Demoddix::eventList[Demoddix::currentEvent];
		if (e.time > Demoddix::currentTime) {
			break;
		}
		FILE* fp = Demoddix::fileList[e.id];
		fpos_t pos = e.pos;
		fsetpos(fp, &pos);
		if (fgets(buffer, Demoddix::bufferSize, fp) != NULL) {
			Demoddix::Front(e, buffer);
		}
		++Demoddix::currentEvent;
	}
}
 // handle events backward until time is reached
void Demoddix::Rewind() 
{
	// update current time
	Demoddix::currentTime -= Demoddix::stepTime;
	if (Demoddix::currentTime < Demoddix::beginTime) Demoddix::currentTime = Demoddix::beginTime;
	
	// handle events until current time is reached
	char buffer[Demoddix::bufferSize];
	while (Demoddix::currentEvent > 0) {
		--Demoddix::currentEvent;
		Event& e = Demoddix::eventList[Demoddix::currentEvent];
		if (e.time != Demoddix::beginTime && e.time <= Demoddix::currentTime) {
			++Demoddix::currentEvent;
			break;
		}
		FILE* fp = Demoddix::fileList[e.id];
		fpos_t pos = e.pos;
		fsetpos(fp, &pos);
		if (fgets(buffer, Demoddix::bufferSize, fp) != NULL) {
			Demoddix::Back(e, buffer);
		}
	}
}

// handle next event
void Demoddix::Next() 
{
	// handle next event and update current time
	char buffer[Demoddix::bufferSize];
	while (Demoddix::currentEvent < Demoddix::eventList.size()) {
		Event& e = Demoddix::eventList[Demoddix::currentEvent];
		Demoddix::currentTime = e.time;
		++Demoddix::currentEvent;
		FILE* fp = Demoddix::fileList[e.id];
		fpos_t pos = e.pos;
		fsetpos(fp, &pos);
		if (fgets(buffer, sizeof(buffer), fp) != NULL) {
			if (Demoddix::Front(e, buffer)) {
				break;
			}
		}
	}
}

// handle previous event
void Demoddix::Previous()
{
	// handle previous event and update current time
	char buffer[Demoddix::bufferSize];
	while (Demoddix::currentEvent > 0) {
		--Demoddix::currentEvent;
		Event& e = Demoddix::eventList[Demoddix::currentEvent];
		Demoddix::currentTime = e.time;
		FILE* fp = Demoddix::fileList[e.id];
		fpos_t pos = e.pos;
		fsetpos(fp, &pos);
		if (fgets(buffer, sizeof(buffer), fp) != NULL) {
			if (Demoddix::Back(e, buffer)) {
				break;
			}
		}
	}
}

// reset
void Demoddix::Reset() 
{
	Demoddix::currentEvent = 0;
	Demoddix::packetList.clear();
	for (auto& n: Demoddix::nodeList) {
		n.state = 0;
	}
	Demoddix::currentTime = Demoddix::beginTime;
}

// handle one event forward 
bool Demoddix::Front(const Event& e, const char* buffer) 
{	
	unsigned long p2pId, p2pSender, p2pReceiver;
	unsigned int stateName, prevStateName, pktName;
	unsigned long long time;
	// handle state change by changing node color (if priority criteria are met) 
	if (sscanf(buffer, nodeChangedStateF, &p2pId, &time, &stateName, &prevStateName) == 4) {
		int oldPrio = Demoddix::stateList[Demoddix::nodeList[p2pId].state].priority;
		int newPrio = Demoddix::stateList[stateName].priority;
		if (newPrio > 0 && newPrio >= oldPrio) {
			Demoddix::nodeList[p2pId].state = stateName;
		}
		return true;
	}
	// handle sent by appending a new packet to the list
	else if (sscanf(buffer, packetSentF, &p2pSender, &time, &p2pReceiver, &pktName) == 4) {
		Demoddix::packetList.push_back(Packet(p2pSender, p2pReceiver, pktName));
		return true;
	}
	// handle receive by removing the first packet satisfying the criteria
	else if (sscanf(buffer, packetReceivedF, &p2pReceiver, &time, &p2pSender, &pktName) == 4) {
		Demoddix::packetList.erase(std::find_if(Demoddix::packetList.begin(), Demoddix::packetList.end(), [=](const Packet& p){ 
			return p.source == p2pSender && p.destination == p2pReceiver && p.message == pktName && !p.lost; 
		}));
		return true;
	}
	// handle lost by appending a new packet to the list (same as sent)
	else if (sscanf(buffer, packetLostF, &p2pSender, &time, &p2pReceiver, &pktName) == 4) {
		Demoddix::packetList.push_back(Packet(p2pSender, p2pReceiver, pktName, true));
		return true;
	}
	// handle other events via the tracer
	else {
		if (Tracer::tracerList[e.id].status() == Tracer::CONNECTED) {
			Tracer::Send(buffer);
		}
		return false;
	}
}

// handle one event backward
bool Demoddix::Back(const Event& e, const char* buffer) 
{	
	unsigned long p2pId, p2pSender, p2pReceiver;
	unsigned int stateName, prevStateName, pktName;
	unsigned long long time;
	// handle state change by changing node color (if priority criteria are met)
	if (sscanf(buffer, nodeChangedStateF, &p2pId, &time, &stateName, &prevStateName) == 4) {
		int oldPrio = Demoddix::stateList[Demoddix::nodeList[p2pId].state].priority;
		int newPrio = Demoddix::stateList[prevStateName].priority;
		if (newPrio > 0 && newPrio >= oldPrio) {
			Demoddix::nodeList[p2pId].state = prevStateName;
		}
		return true;
	}
	// handle sent by removing the first packet satisfying the criteria
	else if (sscanf(buffer, packetSentF, &p2pSender, &time, &p2pReceiver, &pktName) == 4) {
		Demoddix::packetList.erase(std::find_if(Demoddix::packetList.begin(), Demoddix::packetList.end(), [=](const Packet& p){ 
			return p.source == p2pSender && p.destination == p2pReceiver && p.message == pktName && !p.lost; 
		}));
		return true;
	}
	// handle receive by appending a new packet to the list
	else if (sscanf(buffer, packetReceivedF, &p2pReceiver, &time, &p2pSender, &pktName) == 4) {
		Demoddix::packetList.push_back(Packet(p2pSender, p2pReceiver, pktName));
		return true;
	}
	// handle lost by removing the first packet satisfying the criteria (same as sent)
	else if (sscanf(buffer, packetLostF, &p2pSender, &time, &p2pReceiver, &pktName) == 4) {
		Demoddix::packetList.erase(std::find_if(Demoddix::packetList.begin(), Demoddix::packetList.end(), [=](const Packet& p){ 
			return p.source == p2pSender && p.destination == p2pReceiver && p.message == pktName && p.lost; 
		}));
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char **argv) 
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " [configuration-file]" << " [trace-name]"  << std::endl;
		return 1;
	}
	
	Demoddix::iniFileName = argv[1];
	Demoddix::iniFile = fopen(Demoddix::iniFileName, "r");
	if (Demoddix::iniFile == NULL) {
		std::cerr << "Cannot find configuration file " << Demoddix::iniFileName << "!" << std::endl;
		return 1;
	}
	
	// check for trace files and insert them into the list
	for (unsigned long i = 0; i < ULONG_MAX; ++i) {
		char fileName[512];
		sprintf(fileName, "%s_%lu.xml", argv[2], i);
		if (access(fileName, F_OK) == 0) {
			FILE* fp = fopen(fileName, "r");
			if (fp != NULL) {
				Demoddix::fileList.push_back(fp);
				continue;
			}
		}
		break;
	}
	if (Demoddix::fileList.empty()) {
		std::cerr << "No trace files found!" << std::endl;
		fclose(Demoddix::iniFile);
		return 1;
	}
	
	// start
	Demoddix::Open();
	Tracer::Open();

	// init GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	// create window
	RootWindow::Create();
	
	// GLUT loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

	// terminate
	Demoddix::Close();
	Tracer::Close();

	return 0;
}
