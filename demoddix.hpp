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

#ifndef DEMODDIX_HPP
#define DEMODDIX_HPP

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "tracer.hpp"
#include "window.hpp"
#include "root_window.hpp"
#include "state_window.hpp"
#include "message_window.hpp"
#include "main_window.hpp"
#include "progress_window.hpp"
#include "state_color_window.hpp"
#include "message_color_window.hpp"

class State {
public:
	State(std::string&& name_, unsigned int color_, unsigned int priority_): 
		name(name_), 
		color(color_), 
		priority(priority_), 
		position(0) {}
	
	std::string name;			// name of the state
	unsigned int color;			// index of the color found in Window
	unsigned int priority;		// state priority [0, 99]
	int position; 				// used by StateWindow for positioning
};

class Process {
public:
	Process(std::string&& name_): 
		name(name_) {}
	
	std::string name; 	// name of the process
};

class Semaphore {
public:
	Semaphore(std::string&& name_): 
		name(name_) {}
	
	std::string name;	// name of the semaphore
};

class Message {
public:
	Message(std::string&& name_, unsigned int color_): 
		name(name_), 
		color(color_), 
		position(0),
		active(true) {}
	
	std::string name;	// name of the message
	unsigned int color;	// index of the color found in Window
	int position; 		// used by MessageWindow for positioning
	bool active;		// true for showing message
};

class Node {
public:
	Node(double x_, double y_, unsigned int state_): 
		x(x_), 
		y(y_), 
		state(state_) {}

	double x;			// x coordinate of the node
	double y;			// y coordinate of the node
	unsigned int state;	// index of the current State of the node
};

class Packet {
public:
	Packet(unsigned long source_, unsigned long destination_, unsigned int message_, bool lost_ = false): 
		source(source_), 
		destination(destination_), 
		message(message_), 
		lost(lost_) {}
	
	unsigned long source;		// index of the source Node
	unsigned long destination;	// index of the destination Node
	unsigned int message;		// index of the Message 
	bool lost;					// true if packet is lost
};

class Event {
public:
	Event(unsigned long long time_, unsigned long id_, fpos_t pos_): 
		time(time_), 
		id(id_), 
		pos(pos_) {}
	
	unsigned long long time;	// time of Event
	unsigned long id;			// index of the Node the Event belongs to 
	fpos_t pos;					// location of the Event in the trace file
};

class Demoddix {
public:
	static void Open();
	static void Close();
	static void Forward();
	static void Rewind();
	static void Next();
	static void Previous();
	static void Reset();
	static bool Front(const Event& e, const char* buffer);
	static bool Back(const Event& e, const char* buffer);
	
	static const unsigned int bufferSize;
	static const char* iniFileName;
	
	static FILE* iniFile;
	static std::vector<FILE*> fileList;
	
	static std::vector<State> stateList;
	static std::vector<Process> processList;
	static std::vector<Semaphore> semaphoreList;
	static std::vector<Message> messageList;
	static std::vector<Node> nodeList;
	static std::vector<Packet> packetList;
	
	static std::vector<Event> eventList;
	static unsigned long currentEvent;
	
	static unsigned long long currentTime;
	static unsigned long long beginTime;
	static unsigned long long endTime;
	static unsigned long long stepTime;
};

#endif
