include ../Makefile.$(TARGET)

OUTPUT = libLucidFoundation
VERSION := 0.1

INCLUDE = $(STD_INCLUDE) -Iinclude -I$(TOP)/build/$(TARGET)/lucid/include
CFLAGS = $(STD_CFLAGS) $(INCLUDE)
SHARED_FLAGS := $(STD_SHARED_FLAGS) -fPIC -shared -Wl,-soname,$(OUTPUT).so.$(VERSION)

OBJROOT := ../build/$(TARGET)/lucid/obj
INCLOUT := $(TOP)/build/$(TARGET)/lucid
#SOURCES := $(wildcard src/*.cpp)
HEADERS := $(addprefix $(INCLOUT)/,$(wildcard include/lucid/*.h)) $(addprefix $(INCLOUT)/,$(wildcard include/lucid/dynamic/*.h))
LD_FLAGS := $(STD_LDFLAGS)
LIBS := $(STD_LIBS) -lz -lpcre

# ifeq ($(TARGET_OS), windows)
# SHARED_FLAGS += -DFoundation_EXPORTS
# endif

# Filter out files (OpenVMS-specific)
#SOURCES := $(filter-out src/OpcomChannel.cpp, $(SOURCES))
#HEADERS := $(filter-out include/Poco/OpcomChannel.h, $(SOURCES))

CSOURCES := $(wildcard src/*.c)
SOURCES := AbstractObserver.cpp \
ActiveDispatcher.cpp \
ArchiveStrategy.cpp \
Ascii.cpp \
ASCIIEncoding.cpp \
AsyncChannel.cpp \
AtomicCounter.cpp \
Base32Decoder.cpp \
Base32Encoder.cpp \
Base64Decoder.cpp \
Base64Encoder.cpp \
BinaryReader.cpp \
BinaryWriter.cpp \
Bugcheck.cpp \
ByteOrder.cpp \
Channel.cpp \
Checksum.cpp \
Clock.cpp \
Condition.cpp \
Configurable.cpp \
ConsoleChannel.cpp \
CountingStream.cpp \
DateTime.cpp \
DateTimeFormat.cpp \
DateTimeFormatter.cpp \
DateTimeParser.cpp \
Debugger.cpp \
DeflatingStream.cpp \
DigestEngine.cpp \
DigestStream.cpp \
DirectoryIterator.cpp \
DirectoryIteratorStrategy.cpp \
DirectoryWatcher.cpp \
Environment.cpp \
Error.cpp \
ErrorHandler.cpp \
Event.cpp \
EventArgs.cpp \
EventChannel.cpp \
Exception.cpp \
FIFOBufferStream.cpp \
File.cpp \
FileChannel.cpp \
FileStream.cpp \
FileStreamFactory.cpp \
Format.cpp \
Formatter.cpp \
FormattingChannel.cpp \
FPEnvironment.cpp \
Glob.cpp \
Hash.cpp \
HashStatistic.cpp \
HexBinaryDecoder.cpp \
HexBinaryEncoder.cpp \
InflatingStream.cpp \
JSONString.cpp \
Latin1Encoding.cpp \
Latin2Encoding.cpp \
Latin9Encoding.cpp \
LineEndingConverter.cpp \
LocalDateTime.cpp \
LogFile.cpp \
Logger.cpp \
LoggingFactory.cpp \
LoggingRegistry.cpp \
LogStream.cpp \
Manifest.cpp \
MD4Engine.cpp \
MD5Engine.cpp \
MemoryPool.cpp \
MemoryStream.cpp \
Message.cpp \
Mutex.cpp \
NamedEvent.cpp \
NamedMutex.cpp \
NestedDiagnosticContext.cpp \
Notification.cpp \
NotificationCenter.cpp \
NotificationQueue.cpp \
NullChannel.cpp \
NullStream.cpp \
NumberFormatter.cpp \
NumberParser.cpp \
NumericString.cpp \
Path.cpp \
PatternFormatter.cpp \
Pipe.cpp \
PipeImpl.cpp \
PipeStream.cpp \
PriorityNotificationQueue.cpp \
Process.cpp \
PurgeStrategy.cpp \
Random.cpp \
RandomStream.cpp \
RefCountedObject.cpp \
RegularExpression.cpp \
RotateStrategy.cpp \
Runnable.cpp \
RWLock.cpp \
Semaphore.cpp \
SHA1Engine.cpp \
SharedLibrary.cpp \
SharedMemory.cpp \
SignalHandler.cpp \
SimpleFileChannel.cpp \
SortedDirectoryIterator.cpp \
SplitterChannel.cpp \
Stopwatch.cpp \
StreamChannel.cpp \
StreamConverter.cpp \
StreamCopier.cpp \
StreamTokenizer.cpp \
String.cpp \
StringTokenizer.cpp \
SynchronizedObject.cpp \
Task.cpp \
TaskManager.cpp \
TaskNotification.cpp \
TeeStream.cpp \
TemporaryFile.cpp \
TextBufferIterator.cpp \
TextConverter.cpp \
TextEncoding.cpp \
TextIterator.cpp \
Thread.cpp \
ThreadLocal.cpp \
ThreadPool.cpp \
ThreadTarget.cpp \
TimedNotificationQueue.cpp \
Timer.cpp \
Timespan.cpp \
Timestamp.cpp \
Timezone.cpp \
Token.cpp \
Unicode.cpp \
UnicodeConverter.cpp \
URI.cpp \
URIStreamFactory.cpp \
URIStreamOpener.cpp \
UTF16Encoding.cpp \
UTF32Encoding.cpp \
UTF8Encoding.cpp \
UTF8String.cpp \
UUID.cpp \
UUIDGenerator.cpp \
Var.cpp \
VarHolder.cpp \
VarIterator.cpp \
Void.cpp \
Windows1250Encoding.cpp \
Windows1251Encoding.cpp \
Windows1252Encoding.cpp

ifneq ($(TARGET_OS), windows)
SOURCES += SyslogChannel.cpp
else
SOURCES += WindowsConsoleChannel.cpp \
EventLogChannel.cpp
endif

COBJECTS := $(addprefix $(OBJROOT)/foundation/static/,$(notdir $(CSOURCES:.c=.o)))
SHARED_COBJECTS := $(addprefix $(OBJROOT)/foundation/shared/,$(notdir $(CSOURCES:.c=.o)))
OBJECTS := $(addprefix $(OBJROOT)/foundation/static/,$(notdir $(SOURCES:.cpp=.o)))
SHARED_OBJECTS := $(addprefix $(OBJROOT)/foundation/shared/,$(notdir $(SOURCES:.cpp=.o)))

#$(info Headers are [${HEADERS}])

all: makedir writeobj $(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a \
		$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION) $(HEADERS)

$(OBJROOT)/foundation/static/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

$(OBJROOT)/foundation/static/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJROOT)/foundation/shared/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
$(OBJROOT)/foundation/shared/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(SHARED_FLAGS)
	
makedir:
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/foundation/static
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/obj/foundation/shared
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/lib
	$(MKDIR) $(TOP)/build/$(TARGET)/lucid/include/lucid/dynamic
	
writeobj:
	$(file >$(TOP)/build/$(TARGET)/lucid/obj/foundation/shared/s_objects.psr, $(SHARED_OBJECTS) $(SHARED_COBJECTS))
	
$(INCLOUT)/%.h: %.h
	$(CP) $< $(INCLOUT)/$<

$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).a: $(OBJECTS) $(COBJECTS)
	-rm -f $@
	$(AR) rcs $@ $^
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
$(TOP)/build/$(TARGET)/lucid/lib/$(OUTPUT).so.$(VERSION): $(SHARED_OBJECTS) $(SHARED_COBJECTS)
	$(CXX) -o $@ $(SHARED_FLAGS) $(LD_FLAGS) @$(TOP)/build/$(TARGET)/lucid/obj/foundation/shared/s_objects.psr $(LIBS)
	cp $@ $@.debug
	$(STRIP) -S --strip-unneeded $@
	
clean:
	$(RM) $(OBJECTS) $(COBJECTS) $(SHARED_OBJECTS) $(SHARED_COBJECTS)
