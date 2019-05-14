CC = g++
SQL = -L/usr/local/lib/c++/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/lib -lmysqlcppconn -I/usr/local/lib/c++/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include
GTKMM = `pkg-config gtkmm-3.0 --cflags --libs`
CFLAGS = --std=c++14 $(SQL) $(GTKMM)
LDFLAGS = $(SQL) $(GTKMM)
SRC = main.cpp
PROG = main
OBJS = $(SRC:.cpp=.o)
.SUFFIXES: .cpp .o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: my_sql.hpp player.hpp graphView.hpp control.hpp game.hpp abstractObservator.hpp card.hpp observable.hpp popupUser.hpp popupSize.hpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *~ core $(PROG)
