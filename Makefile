# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Iinclude

LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Project structure
TARGET = chess

SRCDIR = src
OBJDIR = obj
INCDIR = include

SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp) $(wildcard $(SRCDIR)/*/*/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SOURCES))

# Default target: build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

# Platform-aware directory creation
ifeq ($(OS),Windows_NT)
define MKDIR
	if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
endef
else
define MKDIR
	mkdir -p $(dir $@)
endef
endif

# Compile each .cpp file into a .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	@cmd /C "del /Q /F $(OBJDIR)\*.o 2>nul & rmdir /S /Q $(OBJDIR) 2>nul & del /Q /F $(TARGET).exe 2>nul"
else
	@rm -rf $(OBJDIR) $(TARGET) $(TARGET).exe
endif


