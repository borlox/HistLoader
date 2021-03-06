######
##  Default configuration
##
## Modify this configuration by creating a local.config file and resetting the
## variables there. See local.config.sample for an example and a description
## of all used variables.

MODE = RELEASE

AR = ar
ARFLAGS = rcs

CPP = g++
CPPFLAGS +=  -Iinclude -O3 -std=c++0x -fPIC `root-config --cflags` #-Wall -Wextra -pedantic
CPPFLAGS_DEBUG += -g -O0 -s
CPPFLAGS_RELEASE +=  -DNDEBUG

LD = g++
LDFLAGS += -Lbin `root-config --ldflags`
LDFLAGS_DEBUG +=
LDFLAGS_RELEASE += 
LIBS += -lHistLoader `root-config --libs` -lboost_unit_test_framework

BIN = bin
SRC = src
INC = include
LIB = lib
BUILD = build

-include ./local.config

# Set build mode specific variables
CPPFLAGS += $(CPPFLAGS_$(MODE))
LDFLAGS += $(LDFLAGS_$(MODE))

######
##  File lists

LIB_HDR = $(wildcard $(INC)/*.h)

SRC_FILES = $(wildcard $(SRC)/*.cpp)
LIB_FILES = $(wildcard $(LIB)/*.cpp)
#ALL_LIB_FILES = $(LIB_FILES) $(BUILD)/dict.cpp

SRC_DEPS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.d,$(SRC_FILES))
LIB_DEPS = $(patsubst $(LIB)/%.cpp,$(BUILD)/%.d,$(LIB_FILES))

ALL_DEPS = $(SRC_DEPS) $(LIB_DEPS)

SRC_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))
LIB_OBJS = $(BUILD)/dict.o
LIB_OBJS += $(patsubst $(LIB)/%.cpp,$(BUILD)/%.o,$(LIB_FILES)) 

ALL_OBJS = $(SRC_OBJS) $(LIB_OBJS)

SLIB = $(BIN)/libHistLoader.a
DYNLIB = $(BIN)/libHistLoader.so

TARGETS = $(patsubst $(SRC)/%.cpp,$(BIN)/%,$(SRC_FILES))

#VERSIONMAJOR = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $1 }' `
#VERSIONMINOR = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $2 }' ` 
#VERSIONPATCH = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $3 }' `

##  Targets

all: libs $(TARGETS)

check: bin/unit_test
	@bin/unit_test --build_info --log_level=test_suite

install: lib
	@cp $(DYNLIB) $$ROOTSYS/lib

uninstall:
	@rm -f $$ROOTSYS/lib/$(notdir $(DYNLIB))

libs: $(DYNLIB) $(SLIB)

help:
	@echo Use 'make' to create all programs
	@echo Use 'make lib' to create the libHistLoader.a
	@echo Special targets:
	@echo "  check          - Run all tests"
	@echo "  list           - List all targets"
	@echo "  mkinfo         - List makefile itnernals"
	@echo "  help           - Show this help message"

# Dependency file creation
$(BUILD)/%.d: $(SRC)/%.cpp
	@$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(BUILD)/%.d: $(LIB)/%.cpp
	@$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(LIB)/%.cpp,$(BUILD)/%.o,$<) $< > $@

# Object creation

$(BUILD)/dict.o: $(BUILD)/dict.cpp 

$(BUILD)/%.o:
	@echo Compiling $@
	@$(CPP) $(CPPFLAGS) -c $< -o $@

$(BUILD)/dict.cpp: $(LIB_SRC) $(LIB_HDR) etc/LinkDef.h
	@echo Creating ROOT dictionary
	@rootcint -f $@ -c -I$(INC) $(patsubst $(INC)/%.h,%.h,$(LIB_HDR)) etc/LinkDef.h
	
# Target
$(BIN)/%: $(BUILD)/%.o $(SLIB)
	@echo Linking $@
	@$(LD) -o $@ $(LDFLAGS) $< $(SLIB) $(LIBS)

$(SLIB): $(LIB_OBJS)
	@echo Creating $@
	@rm -f $@
	@$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

$(DYNLIB): $(LIB_OBJS)
	@echo Creating $@
	@$(LD) -shared -o $@ $(LIB_OBJS)

-include $(ALL_DEPS)

clean:
	@$(RM) $(ALL_OBJS) $(ALL_DEPS) $(TARGETS) $(BIN)/libHistLoader.a $(BIN)/libHistLoader.so

list:
	@echo Target List
	@echo ----------
	@echo $(TARGETS)

mkinfo:
	@echo Make Variables
	@echo --------------
	@echo MODE = $(MODE)
	@echo
	@echo CPP = $(CPP)
	@echo CPPFLAGS = $(CPPFLAGS)
	@echo LD = $(LD)
	@echo LDFLAGS = $(LDFLAGS)
	@echo
	@echo Program Files
	@echo -------------
	@echo Source files:
	@echo "    $(SRC_FILES)"
	@echo Dependency files:
	@echo "    $(SRC_DEPS)"
	@echo Object files:
	@echo "    $(SRC_OBJS)"
	@echo 
	@echo Library Files
	@echo -------------
	@echo Source files:
	@echo "    $(LIB_FILES)"
	@echo Dependency files:
	@echo "    $(LIB_DEPS)"
	@echo Object files:
	@echo "    $(LIB_OBJS)"
	@echo 

.PHONY: clean distclean mkinfo help 

