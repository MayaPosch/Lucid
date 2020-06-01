ifndef TARGET
$(error TARGET parameter not provided.)
endif

export TOP := $(CURDIR)
export TARGET

# Determine whether host OS is Linux or Windows
# Linux returns 'Linux', Windows doesn't have uname, OS X returns 'Darwin'.
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
export HOST = linux
else
export HOST = win32
export FILE_EXT = .exe
endif

# Set proper commandline commands for the detected OS
ifeq ($(HOST), linux)
export MKDIR 	= mkdir -p
export RM		= rm -rf
export CP		= cp -RL
#export LN		= ln -sf
else
# Assuming MSYS shell
export MKDIR 	= mkdir -p
export RM		= rm -rf
export CP		= cp -RL
#export LN		= mklink /d .
endif

include Makefile.$(TARGET)

export TARGET_OS
export TARGET_ARCH
export TOOLCHAIN_NAME

all: foundation net xml json util

foundation:
	$(MAKE) -C ./foundation

net:
	$(MAKE) -C ./net
	
xml:
	$(MAKE) -C ./xml
	
util:
	$(MAKE) -C ./util

json:
	$(MAKE) -C ./json

clean: clean-json clean-foundation clean-net clean-util

clean-foundation:
	$(MAKE) -C ./foundation clean
	
clean-net:
	$(MAKE) -C ./net clean
	
clean-util:
	$(MAKE) -C ./util clean
	
clean-json:
	$(MAKE) -C ./json clean
	
clean-xml:
	$(MAKE) -C ./xml clean

.PHONY: all clean foundation net util json xml clean-net clean-foundation clean-json clean-util clean-xml
