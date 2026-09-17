# Define o compilador explicitamente como GCC
CC = gcc

# Nome do executavel final
TARGET = build/CppCardGame

# Arquivos fonte
SRCS = src/main.c

# Detecta se o SO eh Windows ou Linux
ifeq ($(OS),Windows_NT)
    # --- Configurações para Windows ---
    # Ajuste o caminho da pasta SDL3 se necessario (ex: C:/SDL3 ou C:/mingw64)
    SDL_DIR = C:/mingw64
    INCLUDES = -I"$(SDL_DIR)/include"
    LIBS = -L"$(SDL_DIR)/lib" -lSDL3
    EXE = $(TARGET).exe
    RM = del /Q 2>NUL || clean
else
    # --- Configurações para Linux ---
    # Usa o pkg-config para obter as flags de compilacao e bibliotecas nativas
    INCLUDES = $(shell pkg-config --cflags sdl3 2>/dev/null)
    LIBS = $(shell pkg-config --libs sdl3 2>/dev/null || echo "-lSDL3")
    EXE = $(TARGET)
    RM = rm -f
endif

# Flags de otimização e avisos do compilador
CFLAGS = -Wall -Wextra -O2 $(INCLUDES)

# Regra padrao (executada ao rodar apenas 'make')
all: $(EXE)

# Compilacao do executavel
$(EXE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) $(LIBS)

# Limpeza dos binarios gerados
clean:
	$(RM) $(EXE)

.PHONY: all clean