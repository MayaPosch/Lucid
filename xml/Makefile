include ../Makefile.$(TARGET)

OUTPUT := libLucidXML
VERSION := 0.1

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/lucid/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE) -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H -DEXPAT_POCO
CXXFLAGS := $(STD_CXXFLAGS) $(INCLUDE) -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H -DEXPAT_POCO
SHARED_FLAGS := $(STD_SHARED_FLAGS) -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT = ../build/$(TARGET)/lucid/obj
INCLOUT = $(TOP)/build/$(TARGET)/lucid

CSOURCES := $(wildcard src/*.c)
SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/lucid/XML/*.h)) \
			$(addprefix $(INCLOUT)/,$(wildcard include/lucid/DOM/*.h)) \
			$(addprefix $(INCLOUT)/,$(wildcard include/lucid/SAX/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := -L$(TOP)/build/$(TARGET)/lucid/lib -lLucidFoundation $(STD_LIBS) 

# ifeq ($(TARGET_OS), windows)
# SHARED_FLAGS += -DXML_EXPORTS
# endif

COBJECTS := $(addprefix $(OBJROOT)/xml/static/,$(notdir $(CSOURCES:.c=.o)))
SHARED_COBJECTS := $(addprefix $(OBJROOT)/xml/shared/,$(notdir $(CSOURCES:.c=.o)))
OBJECTS := $(addprefix $(OBJROOT)/xml/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/xml/shared/,$(notdir $(SOURCES:.cpp=.o)))

all: makedir $(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/xml/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJROOT)/xml/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/xml/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(SHARED_FLAGS)
	
$(OBJROOT)/xml/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/xml/static
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/xml/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/XML
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/SAX
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/DOM
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a: $(OBJECTS) $(COBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(LD_FLAGS) $(SHARED_FLAGS) $(SHARED_OBJECTS) $(SHARED_COBJECTS) $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(COBJECTS) $(SHARED_OBJECTS) $(SHARED_COBJECTS)
