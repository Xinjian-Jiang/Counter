CPPFLAGS = -std=c++17 -Wall -Wextra -Werror
INCLUDE_PATH = -I../../external/parlaylib/include/ -I../../external

all: clean mis

mis: MIS.cpp
	g++ $(CPPFLAGS) $(INCLUDE_PATH) MIS.cpp -o MIS -pthread
	
clean:
	rm -f MIS