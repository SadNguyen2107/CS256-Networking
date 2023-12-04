# Output File
OUTPUT 			= Homework_Submitter

# Compiler and Flags
GCC 			= x86_64-w64-mingw32-g++
GGC_FLAGS 		= -static-libgcc -static-libstdc++ -Wall
LOGFLAGS 		= -DBOOST_LOG_DYN_LINK
LIBS 			= -lmingw32 -lSDL2main -lSDL2 -lSDL2_Image -lSDL2_Mixer -lSDL2_ttf

# Check the OS-SYSTEM to give the Right LIBS
#--------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Windows-specific settings or actions
	LIBS += -lole32 -lshell32 -lcomdlg32 -luuid -lws2_32 -lboost_system-mt -lboost_log-mt -lcrypto
else ifeq ($(OS), Linux)
	# Linux-specific settings or actions
	LIBS += -lboost_log
else ifeq ($(OS),Darwin)
    # macOS-specific settings or actions
else
    $(error Unsupported operating system: $(OS))
endif
#------------------------------------------------------------------

# Directories
SRC_DIR 		= src/include
OBJ_DIR 		= build/obj
BIN_DIR 		= build/bin

# Source Files
SOURCES 		= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard *.cpp)
OBJECTS 		= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE 		= $(BIN_DIR)/$(OUTPUT)
RES_FILES 		= winres/TF-icon.res

# Build Target
all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)


$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(GGC_FLAGS) $(LOGFLAGS) -o $@ $^ $(RES_FILES) $(LIBS)
	@./$(EXECUTABLE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(GGC_FLAGS) $(LOGFLAGS) -c -o $@ $<


# For Cleaning Up the Folder
clean:
	@echo clean...
	@rm -rf $(OBJ_DIR) $(BIN_DIR)


# For Running Allplication
run:
	@./$(EXECUTABLE)


# Check Memory Leak
check:
	@drmemory -logdir logs $(EXECUTABLE).exe

.PHONY: all clean directories