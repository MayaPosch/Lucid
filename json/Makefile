include ../Makefile.$(TARGET)

OUTPUT = libLucidJson
VERSION := 0.1

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/lucid/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE)
SHARED_FLAGS := -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT = $(TOP)/build/$(TARGET)/lucid/obj
INCLOUT = $(TOP)/build/$(TARGET)/lucid

CSOURCES := $(wildcard src/*.c)
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/lucid/json/*.h))
LIBS := -L$(TOP)/build/$(TARGET)/lucid/lib -lLucidFoundation $(STD_LIBS) 

COBJECTS := $(addprefix $(OBJROOT)/json/static/,$(notdir $(CSOURCES:.c=.o)))
SHARED_COBJECTS := $(addprefix $(OBJROOT)/json/shared/,$(notdir $(CSOURCES:.c=.o)))
OBJECTS := $(addprefix $(OBJROOT)/json/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/json/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir $(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/json/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

$(OBJROOT)/json/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/json/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
$(OBJROOT)/json/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/json/static
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/json/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/json
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a: $(OBJECTS) $(COBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	
$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(CFLAGS) $(SHARED_FLAGS) $(SHARED_OBJECTS) $(SHARED_COBJECTS) $(LIBS)
	
clean:
	$(RM) $(OBJECTS)
