#############################################
#  *nix Makefile for SimpleMemoryAllocator  #
#  Author: Matus Sabol                      #
#  Date: 27. 6. 2018                        #
#############################################

.PHONY: all clean prebuild postbuild examples
AR = ar ru
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
TARGETS = static_int dynamic_int

SOURCE_DIR = src
BUILD_DIR = build
EXAMPLES_DIR = examples

STATIC_BUILD_PATH = $(BUILD_DIR)/libSimpleMemoryAllocator.a
DYNAMIC_BUILD_PATH = $(BUILD_DIR)/libSimpleMemoryAllocator.so

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(subst $(SOURCE_DIR)/,$(BUILD_DIR)/obj/,$(SOURCES:.cpp=.o))
OBJECTS_PIC = $(subst $(SOURCE_DIR)/,$(BUILD_DIR)/pic/,$(SOURCES:.cpp=.o))

all: prebuild $(TARGETS) postbuild

  ##################################
  # any pre-build actions required #
##################################################################################
prebuild:
	@echo "========== PREBUILD ACTIONS =========="
	mkdir -p $(BUILD_DIR)/obj/
	mkdir -p $(BUILD_DIR)/pic/
	@echo ""


  ###################################
  # any post-build actions required #
##################################################################################
postbuild:
	@echo ""
	@echo "========== POSTBUILD ACTIONS =========="
#	rm -f $(OBJECTS)


  ########################
  # build static library #
##################################################################################
static: prebuild static_int postbuild

static_int: $(STATIC_BUILD_PATH)

$(STATIC_BUILD_PATH): $(OBJECTS)
	@echo ""
	@echo "========== BUILDING STATIC LIBRARY =========="
	$(AR) $@ $(OBJECTS)


  #########################
  # build dynamic library #
##################################################################################
dynamic: prebuild dynamic_int postbuild

dynamic_int: $(DYNAMIC_BUILD_PATH)

$(DYNAMIC_BUILD_PATH): $(OBJECTS_PIC)
	@echo ""
	@echo "========== BUILDING DYNAMIC LIBRARY =========="
	$(CXX) $(OBJECTS_PIC) -shared -o $@


  #################
  # build objects #
##################################################################################
$(BUILD_DIR)/obj/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/pic/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) -c -fPIC $(CXXFLAGS) $< -o $@



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
	@echo ""
	@echo "========== CLEANING =========="
	rm -rf $(BUILD_DIRECTORY) $(OBJECTS) $(OBJECTS_PIC) $(STATIC_BUILD_PATH) $(DYNAMIC_BUILD_PATH)


