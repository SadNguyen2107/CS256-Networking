# Output File
TARGET 			= server

# Compiler and Flags
CXX 			= x86_64-w64-mingw32-g++
CXXFLAGS 		= -static-libgcc -static-libstdc++ -Wall -DBOOST_LOG_DYN_LINK 
LIBS 			= -lsqlite3

# Check the OS-SYSTEM to give the Right LIBS
#--------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Windows-specific settings or actions
	LIBS += -lwsock32 -lws2_32 -lboost_log-mt -lboost_system-mt -lcrypto
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		CXX = g++
		LIBS += -lboost_log -lboost_system -lcrypto
    endif
endif
#------------------------------------------------------------------

# Directories
OBJ_DIR 			= build/obj
BIN_DIR 			= build/bin

# Source Files Directory and Main File
MAIN 				= server.cpp
SOURCE_PATH			= src
BASE_PATH 			= $(SOURCE_PATH)/Base
DATA_STRUCTURE_PATH = $(BASE_PATH)/DataStructures
INCLUDE_PATH 		= $(SOURCE_PATH)/include
SQL_PATH 			= $(SOURCE_PATH)/sql
UTILS_PATH			= $(SOURCE_PATH)/utils

#Setting To Compile
SOURCES 			= $(MAIN) 
SOURCES				+= $(wildcard $(DATA_STRUCTURE_PATH)/*.cpp) 
SOURCES 			+= $(wildcard $(BASE_PATH)/*.cpp) 
SOURCES 			+= $(wildcard $(INCLUDE_PATH)/*.cpp) 
SOURCES				+= $(wildcard $(SQL_PATH)/*.cpp) 
SOURCES				+= $(wildcard $(UTILS_PATH)/*.cpp) 
OBJS 				= $(patsubst %, $(OBJ_DIR)/%, $(notdir $(SOURCES:.cpp=.o)))

MK_DIR 				= mkdir -p

# Build Target
all: directories $(TARGET)

.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Make Executable FIle
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(RES_FILES) $(LIBS)


# Make Object Files (for main.cpp)
$(OBJ_DIR)/server.o: $(MAIN)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Make Object Files (for Base Path)
$(OBJ_DIR)/%.o: $(BASE_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Make Object Files (for Data Structure Path)
$(OBJ_DIR)/%.o: $(DATA_STRUCTURE_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Make Object Files (for SQL Path)
$(OBJ_DIR)/%.o: $(SQL_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Make Object Files (for Utils Path)
$(OBJ_DIR)/%.o: $(UTILS_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# Make Object Files (for Include Path)
$(OBJ_DIR)/%.o: $(INCLUDE_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

# For Cleaning Up the Folder
.PHONY: clean
clean:
	@echo clean...
	@rm -rf $(OBJ_DIR) $(BIN_DIR)


# For Running Allplication
.PHONY: run
run:
	@./$(BIN_DIR)/$(TARGET)


# Check Memory Leak
.PHONY: valgrind
valgrind:
	@valgrind ./$(BIN_DIR)/$(TARGET)