CC = g++
# Flags di compilazione: aggiungi il percorso degli header di redis
CXXFLAGS = -Wall -g -I/usr/local/include/hiredis -O2
# Flags di linking: specifica dove trovare le librerie e quali librerie linkare
LDFLAGS = -L/usr/local/lib -lhiredis -O2

# Definisci i percorsi dei file sorgente, oggetto ed eseguibili
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))
TARGET = bin/main

# Definisci le regole per la compilazione
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) 
	@mkdir -p bin
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(CXXFLAGS)

obj/%.o: src/%.cpp 
	@mkdir -p obj
	$(CC) $(CXXFLAGS) -c $< -o $@ 




run: $(TARGET) 
	cd bin && ./main


clean:
	rm -rf obj/*.o bin/*
