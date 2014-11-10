include c.mk

EXECUTABLE = fibonacci
rwildcard=$(foreach d, $(wildcard $1*), $(call rwildcard, $d/, $2) $(filter $(subst *, %, $2), $d))
SOURCES = $(call rwildcard, src/, *.cpp)
HEADERS = $(call rwildcard, src/, *.h)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

TEST_EXECUTABLE := $(EXECUTABLE)_tests

# flags
CXXFLAGS := $(CXX_ALL_WARNINGS_FLAG) src/

CXXFLAGS_RELEASE := $(CXX_OPTIMIZE_FLAG) $(CXX_RELEASE_FLAG)
CXXFLAGS_DEBUG := $(CXX_DEBUG_FLAG)

LDFLAGS_TEST := $(CXX_GTEST_LIB)

BUILD ?= release
ifeq ($(BUILD), release)
	CXXFLAGS += $(CXXFLAGS_RELEASE)
endif
ifeq ($(BUILD), debug)
	CXXFLAGS += $(CXXFLAGS_DEBUG)
endif

.PHONY: all
all: build

.PHONY: build
build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(CXX_OUTPUTFILE_FLAG) $@

OBJECTS_UNITTEST = $(patsubst %.o, %_gTest.o, $(OBJECTS))
%_gTest.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXX_UNITTEST_FLAG) $(CXX_NOLINK_FLAG) $^ $(CXX_OUTPUTFILE_FLAG) $@

$(TEST_EXECUTABLE): $(OBJECTS_UNITTEST)
	$(LD) $(LDFLAGS) $(LDFLAGS_TEST) $^ $(CXX_OUTPUTFILE_FLAG) $@

.PHONY: tests
tests: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(TEST_EXECUTABLE) $(OBJECTS) $(OBJECTS_UNITTEST)
