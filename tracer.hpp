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

#ifndef TRACER_HPP
#define TRACER_HPP

#include <string>
#include <vector>
#include <thread>
#include <atomic>

class Tracer {
public:
	enum {IDLE, OPENED, CONNECTED, CLOSED};	// tracer status
	Tracer(): port_(0), sock_(-1), status_(IDLE) {}
	Tracer(const Tracer& other): port_(other.port_.load()), sock_(other.sock_.load()), status_(other.status_.load()) {}
	Tracer& operator=(const Tracer& other) 
	{
		port_.store(other.port_.load());
		sock_.store(other.sock_.load());
		status_.store(other.status_.load());
		return *this;
	}
	
	unsigned short port() { return port_.load();}
	void port(unsigned short value) { port_.store(value); }
	int sock() { return sock_.load(); }
	void sock(int value) { sock_.store(value); }
	int status() { return status_.load(); }
	void status(int value) { status_.store(value); }
	
private:
	std::atomic_ushort port_;	// port where tracer is listening
	std::atomic_int sock_;		// socket for sending to tracer
	std::atomic_int status_;	// status of the tracer
	

public:
	static void Open();
	static void Close();
	static void Launch(unsigned long id);
	static void Poll();
	static void Send(const char* buffer);
	
public:
	static std::vector<Tracer> tracerList;
	static std::vector<std::thread> threadList;
	
	static std::thread pollThread;
	static std::string tracerCommand;
	static std::string pollCommand;
	static unsigned int pollInterval;
	static std::atomic_bool doPoll;
};

#endif
