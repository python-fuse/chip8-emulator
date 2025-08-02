# ------------------------------------------------------------------
#  Makefile for CHIP‑8 emulator with SFML + ImGui + ImGui‑SFML
# ------------------------------------------------------------------

CXX        := g++
CXXFLAGS   := -Wall -std=c++17 -Iimgui -I. -Itinyfile
LDFLAGS    := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL

SRCS := \
    main.cpp \
    platform.cpp \
    chip8.cpp \
    imgui/imgui.cpp \
    imgui/imgui_draw.cpp \
    imgui/imgui_tables.cpp \
    imgui/imgui_widgets.cpp \
    imgui/imgui_demo.cpp \
    imgui/imgui-SFML.cpp  \
    tinyfile/tinyfiledialogs.c

# Convert all .cpp and .c files in SRCS to .o
OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

TARGET := chip8

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Build rule for .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build rule for .c
%.o: %.c
	$(CC) -std=c99 -I. -Itinyfile -c $< -o $@
# or if you want to keep using g++ for C code too:
# %.o: %.c
# 	$(CXX) $(CXXFLAGS) -x c -c $< -o $@

clean:
	rm -f $(OBJS)
