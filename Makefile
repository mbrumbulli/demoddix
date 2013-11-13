
CXX=g++
CXXFLAGS=-std=c++11 -pthread -O2 -g -Wall
LIBS=-lGL -lGLU -lglut

# DO NOT CHANGE THE ORDER OF SOURCE FILES
SRC=demoddix.cpp \
	tracer.cpp \
	window.cpp \
	root_window.cpp \
	main_window.cpp \
	state_window.cpp \
	message_window.cpp \
	progress_window.cpp \
	state_color_window.cpp \
	message_color_window.cpp

OBJ=$(patsubst %.cpp,%.o,$(SRC))

TARGET=demoddix

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBS) -o $(TARGET)
	
%.o: %.cpp Makefile
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET) *.o
