#############################################
#  *nix Makefile for SimpleMemoryAllocator  #
#  Author: Matus Sabol                      #
#  Date: 27. 6. 2018                        #
#############################################

# TODO: dynamic library building?

.PHONY: all clean prebuild static
AR = ar ru
CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2
TARGETS = static

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(subst $(SOURCE_DIR)/,$(BUILD_DIR)/,$(SOURCES:.cpp=.o)) 

all: prebuild $(TARGETS)

# any pre-build actions required
prebuild:
	mkdir -p $(BUILD_DIR)

# build static library
static: libSimpleMemoryAllocator.a 

# build objects
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

libSimpleMemoryAllocator.a: $(OBJECTS)
	$(AR) $(BUILD_DIR)/$@ $(OBJECTS)
	rm -f $(OBJECTS)

clean:
	rm -rf $(BUILD_DIRECTORY) $(OBJECTS) $(BUILD_DIR)/libSimpleMemoryAllocator.*
