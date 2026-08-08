CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -MMD -MP -O2

SRC_DIR = src
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = my_redis_server

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RM = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
    RMTARGET = if exist $(TARGET) del /q $(TARGET)
else
    MKDIR = mkdir -p $(BUILD_DIR)
    RM = rm -rf $(BUILD_DIR)
    RMTARGET = rm -f $(TARGET)
endif

all: $(TARGET)

$(BUILD_DIR):
	$(MKDIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	$(RM)
	$(RMTARGET)

rebuild: clean all

run: all
	./$(TARGET)