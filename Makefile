# CXX = g++
# CXXFLAGS = -Wall -std=c++11 -pthread

# LIBS = -lboost_system -lboost_thread

# all: server client

# server: server/server.o
# 	${CXX} -o server/server server/server.o ${LIBS}

# server/server.o: server/server.cpp
# 	${CXX} ${CXXFLAGS} -c server/server.cpp -o server/server.o

# client: client/client.o client/ascii_art.o client/login.o client/logout.o
# 	${CXX} -o client/client client/client.o client/ascii_art.o client/login.o client/logout.o ${LIBS}

# client/client.o: client/client.cpp
# 	${CXX} ${CXXFLAGS} -c client/client.cpp -o client/client.o

# client/ascii_art.o: client/ascii_art.cpp
# 	${CXX} ${CXXFLAGS} -c client/ascii_art.cpp -o client/ascii_art.o

# client/login.o: client/login.cpp
# 	${CXX} ${CXXFLAGS} -c client/login.cpp -o client/login.o

# client/logout.o: client/logout.cpp
# 	${CXX} ${CXXFLAGS} -c client/logout.cpp -o client/logout.o

# clean:
# 	rm -f server/*.o client/*.o server/server client/client


CXX = g++
CXXFLAGS = -Wall -std=c++11 -pthread

LIBS = -lboost_system -lboost_thread

all: server client

server: server/server.o
	${CXX} -o server/server server/server.o ${LIBS}

server/server.o: server/server.cpp
	${CXX} ${CXXFLAGS} -c server/server.cpp -o server/server.o

client: client/client.o client/ascii_art.o client/login.o client/logout.o client/caro.o
	${CXX} -o client/client client/client.o client/ascii_art.o client/login.o client/logout.o client/caro.o ${LIBS}

client/client.o: client/client.cpp
	${CXX} ${CXXFLAGS} -c client/client.cpp -o client/client.o

client/ascii_art.o: client/ascii_art.cpp
	${CXX} ${CXXFLAGS} -c client/ascii_art.cpp -o client/ascii_art.o

client/login.o: client/login.cpp
	${CXX} ${CXXFLAGS} -c client/login.cpp -o client/login.o

client/logout.o: client/logout.cpp
	${CXX} ${CXXFLAGS} -c client/logout.cpp -o client/logout.o

client/caro.o: client/caro.cpp
	${CXX} ${CXXFLAGS} -c client/caro.cpp -o client/caro.o

clean:
	rm -f server/*.o client/*.o server/server client/client
