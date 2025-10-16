# Nombre del ejecutable
TARGET = main

# Archivos fuente (.cpp)
SRC = $(wildcard *.cpp)

# Compilador
CXX = g++

# Flags de compilacion obtenidos automaticamente con pkg-config
CXXFLAGS = `pkg-config --cflags opencv4`
LDFLAGS  = `pkg-config --libs opencv4`

# Regla principal
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Limpieza de archivos compilados
clean:
	rm -f $(TARGET)
