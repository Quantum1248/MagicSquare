ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= main
endif

all: bin/$(EXECUTABLE)

clean:
	$(RM) bin/$(EXECUTABLE)

run: all
	./bin/$(EXECUTABLE)

bin/$(EXECUTABLE): src/*.cpp
	g++ src/*.cpp -std=c++17 -Wall -Wextra -g -pthread -Iinclude -Llib -o "bin/$(EXECUTABLE)" -O0 -lgmpxx -lgmp