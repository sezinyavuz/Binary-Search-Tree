CXXFLAGS += -g -DNDEBUG

.phony: all wsp release

all: release

release: webstore.cpp
	g++ -std=c++11 webstore.cpp -o webstore $(CXXFLAGS)

clean:
	rm -f ./webstore