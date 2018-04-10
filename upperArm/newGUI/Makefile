all: mission

CXXFLAGS += -std=c++11 -I. -g

LDFLAGS += -lopencv_imgproc -lopencv_highgui -lopencv_core

mission: mission.cpp
	g++ $(CXXFLAGS) -o mission mission.cpp $(LDFLAGS)
