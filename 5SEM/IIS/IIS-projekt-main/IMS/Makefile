CC = g++
CXXFLAGS = -I${HOME}/.local/include -fPIC
LDFLAGS = -L${HOME}/.local/lib -lsimlib -static
SOURCES = winery.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ims_sim
RUN_PARAMS = -dt 1 -ds 100 -gs 2000 -ps 60 -bs 200 -sl 365

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(RUN_PARAMS)
