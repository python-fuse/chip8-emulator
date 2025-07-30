# ------------------------------------------------------------------
#  Makefile for CHIP‑8 emulator with SFML + ImGui + ImGui‑SFML
# ------------------------------------------------------------------

CXX        := g++
CXXFLAGS   := -Wall -std=c++17 -Iimgui -I.
LDFLAGS    := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL

# List all your .cpp files (main emulator + ImGui + ImGui‑SFML)
SRCS := \
    main.cpp \
    platform.cpp \
    chip8.cpp \
    imgui/imgui.cpp \
    imgui/imgui_draw.cpp \
    imgui/imgui_tables.cpp \
    imgui/imgui_widgets.cpp \
    imgui/imgui_demo.cpp \
    imgui/imgui-SFML.cpp

# Automatically turn them into .o names:
OBJS := $(SRCS:.cpp=.o)

TARGET := chip8

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile rule (works for both top‑level and subdirs)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
