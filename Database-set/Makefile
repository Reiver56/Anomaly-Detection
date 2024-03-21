CXX = g++
# Flags di compilazione: aggiungi il percorso degli header di PostgreSQL
CXXFLAGS = -Wall -g -I/usr/include/postgresql
# Flags di linking: specifica dove trovare le librerie e quali librerie linkare
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lpqxx -lpq

# Definisci i percorsi dei file sorgente, oggetto ed eseguibili
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))
TARGET = bin/main

# Definisci le regole per la compilazione
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj/*.o bin/*
