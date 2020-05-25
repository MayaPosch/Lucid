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

all: json foundation net

json:
	$(MAKE) -C ./json

foundation:
	$(MAKE) -C ./foundation

net:
	$(MAKE) -C ./net

clean: clean-json clean-foundation clean-net

clean-foundation:
	$(MAKE) -C ./foundation clean
	
clean-net:
	$(MAKE) -C ./net clean
	
clean-json:
	$(MAKE) -C ./json clean

.PHONY: all clean foundation net json clean-net clean-foundation clean-json