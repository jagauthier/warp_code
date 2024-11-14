# Compiler and flags
CXX = aarch64-linux-gnu-g++
CXX = g++

CXXFLAGS = -g -Wall -Wextra -std=c++17

# Target executable
TARGET = main
INCLUDES = -Iincludes
LIBS=-lm
# Source files
SRCS = main.cc thread.cc example.cc example_queue.cc

# Build directory
BUILD_DIR = objs
HEADERS = includes/*.h


# Object files
OBJS = $(SRCS:%.cc=$(BUILD_DIR)/%.o)

# Phony target to create the build directory

# Compile and link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)
	#scp $(TARGET) root@192.168.80.188:~/

$(BUILD_DIR)/%.o: %.cc $(HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	

# Clean up
clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o 

.PHONY: build

build:
	mkdir -p $(BUILD_DIR)