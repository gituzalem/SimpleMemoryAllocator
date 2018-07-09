#############################################
#  *nix Makefile for SimpleMemoryAllocator  #
#  Author: Matus Sabol                      #
#  Date: 27. 6. 2018                        #
#############################################

# TODO: dynamic library building?

.PHONY: all clean prebuild examples
AR = ar ru
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
TARGETS = static

SOURCE_DIR = src
BUILD_DIR = build
EXAMPLES_DIR = examples

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(subst $(SOURCE_DIR)/,$(BUILD_DIR)/,$(SOURCES:.cpp=.o)) 

all: prebuild $(TARGETS)

  ##################################
  # any pre-build actions required #
##################################################################################
prebuild:
	mkdir -p $(BUILD_DIR)

  ########################
  # build static library #
##################################################################################
static: $(BUILD_DIR)/libSimpleMemoryAllocator.a 

  #################
  # build objects #
##################################################################################
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/libSimpleMemoryAllocator.a: $(OBJECTS)
	$(AR) $@ $(OBJECTS)
	rm -f $(OBJECTS)

  ##################
  # build examples #
##################################################################################
examples: static exampleLinearAllocator

exampleLinearAllocator: $(EXAMPLES_DIR)/LinearAllocatorExample.cpp
	$(CXX) $(CXXFLAGS) $< -I./include -L./build -lSimpleMemoryAllocator -o $(BUILD_DIR)/$@.exe


  #########
  # clean #
##################################################################################
clean:
	rm -rf $(BUILD_DIRECTORY) $(OBJECTS) $(BUILD_DIR)/libSimpleMemoryAllocator.*
