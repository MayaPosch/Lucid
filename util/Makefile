include ../Makefile.$(TARGET)

OUTPUT = libLucidUtil
VERSION := 0.1

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/lucid/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE)
SHARED_FLAGS := $(STD_SHARED_FLAGS) -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT = ../build/$(TARGET)/lucid/obj
INCLOUT = $(TOP)/build/$(TARGET)/lucid
#SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/lucid/util/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := -L$(TOP)/build/$(TARGET)/lucid/lib -lLucidNet -lLucidFoundation -lLucidJson -lLucidXML $(STD_LIBS) 

SOURCES = AbstractConfiguration.cpp \
Application.cpp \
ConfigurationMapper.cpp \
ConfigurationView.cpp \
FilesystemConfiguration.cpp \
HelpFormatter.cpp \
IniFileConfiguration.cpp \
IntValidator.cpp \
JSONConfiguration.cpp \
LayeredConfiguration.cpp \
LoggingConfigurator.cpp \
LoggingSubsystem.cpp \
MapConfiguration.cpp \
Option.cpp \
OptionCallback.cpp \
OptionException.cpp \
OptionProcessor.cpp \
OptionSet.cpp \
PropertyFileConfiguration.cpp \
RegExpValidator.cpp \
ServerApplication.cpp \
Subsystem.cpp \
SystemConfiguration.cpp \
Timer.cpp \
TimerTask.cpp \
Validator.cpp \
XMLConfiguration.cpp

ifeq ($(TARGET_OS), windows)
	SOURCES += WinRegistryConfiguration.cpp \
	WinRegistryKey.cpp \
	WinService.cpp
endif

OBJECTS := $(addprefix $(OBJROOT)/util/static,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/util/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir $(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/util/static%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/util/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/util/static
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/util/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/util
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a: $(OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(LD_FLAGS) $(SHARED_FLAGS) $(SHARED_OBJECTS) $(SHARED_COBJECTS) $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS)
