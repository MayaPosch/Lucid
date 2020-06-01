include ../Makefile.$(TARGET)

OUTPUT := libLucidNet
VERSION := 0.1

INCLUDE := $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/lucid/include
CFLAGS := $(STD_CFLAGS) $(INCLUDE)
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE)
SHARED_FLAGS := $(STD_SHARED_FLAGS) -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := ../build/$(TARGET)/lucid/obj
INCLOUT := $(TOP)/build/$(TARGET)/lucid
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/lucid/net/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := -L$(TOP)/build/$(TARGET)/lucid/lib -lLucidFoundation $(STD_LIBS) 

OBJECTS := $(addprefix $(OBJROOT)/net/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/net/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir writeobj $(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)
$(OBJROOT)/net/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	
$(OBJROOT)/net/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/net/static
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/net/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/net

writeobj:
	$(file >$(TOP)/build/$(TARGET)/lucid/obj/net/shared/s_objects.psr, $(SHARED_OBJECTS))
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a: $(OBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS)
	$(CXX) -o $@ $(LD_FLAGS) $(SHARED_FLAGS) @$(TOP)/build/$(TARGET)/lucid/obj/net/shared/s_objects.psr $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(SHARED_OBJECTS)
