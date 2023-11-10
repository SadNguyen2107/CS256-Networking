# Determine the operating system
ifeq ($(OS),Windows_NT)
    # Windows-specific compiler and flags
    CC = x86_64-w64-mingw32-g++
    CFLAGS = -static-libgcc -static-libstdc++
    LOGFLAGS = -DBOOST_LOG_DYN_LINK
    LDFLAGS = -lsqlite3 -lole32 -lshell32 -lcomdlg32 -luuid -lws2_32 -lboost_system-mt -lboost_log-mt -lcrypto
else
    # Linux-specific compiler and flags
    CC = g++
    CFLAGS =
    LDFLAGS = -lsqlite3 -lboost_system -lboost_log -lcrypto # Add Linux-specific linker flags here
endif

# Common compiler flags and source files
CFLAGS += -Wall
SOURCES = client.cpp

# Output executable name
OUTPUT = client

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(LOGFLAGS) -o $(OUTPUT) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(OUTPUT)
