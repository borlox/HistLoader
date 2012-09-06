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
CPPFLAGS +=  -Iinclude -O3 -std=c++0x   
CPPFLAGS_DEBUG += -g -O0 -s
CPPFLAGS_RELEASE +=  -DNDEBUG

LD = g++
LDFLAGS += -Lbin 
LDFLAGS_DEBUG +=
LDFLAGS_RELEASE += 
LIBS +=  -lsim

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

SRC_FILES = $(wildcard $(SRC)/*.cpp)
LIB_FILES = $(wildcard $(LIB)/*.cpp)

SRC_DEPS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.d,$(SRC_FILES))
LIB_DEPS = $(patsubst $(LIB)/%.cpp,$(BUILD)/%.d,$(LIB_FILES))

ALL_DEPS = $(SRC_DEPS) $(LIB_DEPS)

SRC_OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))
LIB_OBJS = $(patsubst $(LIB)/%.cpp,$(BUILD)/%.o,$(LIB_FILES))

ALL_OBJS = $(SRC_OBJS) $(LIB_OBJS)

TARGETS = $(patsubst $(SRC)/%.cpp,$(BIN)/%,$(SRC_FILES))

#VERSIONMAJOR = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $1 }' `
#VERSIONMINOR = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $2 }' ` 
#VERSIONPATCH = `$(CPP) --version | grep gcc | awk '{print $3}' |awk -F.  '{ print $3 }' `


##  Targets

all: simlib  $(TARGETS)

simlib: $(BIN)/libsim.a

help:
	@echo Use 'make' to create all programs
	@echo Use 'make simlib' to create the libsim.a
	@echo Special targets:
	@echo "  list           - List all targets"
	@echo "  mkinfo         - List makefile itnernals"
	@echo "  help           - Show this help message"
	@echo "  check			- Checks for required libs and compilers"
	@echo "  lua            - Create Lua wrapper"

# Dependency file creation
$(BUILD)/%.d: $(SRC)/%.cpp
	@$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$<) $< > $@

$(BUILD)/%.d: $(LIB)/%.cpp
	@$(CPP) $(CPPFLAGS) -MM -MT $(patsubst $(LIB)/%.cpp,$(BUILD)/%.o,$<) $< > $@

# Object creation
$(BUILD)/%.o:
	@echo Compiling $@
	@$(CPP) $(CPPFLAGS) -c $< -o $@
	
# Target
$(BIN)/%: $(BUILD)/%.o $(LIB_OBJS)
	@echo Linking $@
	@$(LD) -o $@ $(LDFLAGS) $(LIBS) $(LIB_OBJS)  $<

$(BIN)/libsim.a: $(LIB_OBJS)
	@echo Creating $@
	@$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

-include $(ALL_DEPS)

clean:
	@$(RM) $(ALL_OBJS) $(ALL_DEPS) $(TARGETS) $(BIN)/libsim.a


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

check:
	@./chkconfig $(CPP) 


.PHONY: clean distclean mkinfo help 

