# g++
CXX := g++
LD := $(CXX)

# flags
ifeq ($(CXX), g++)
	CXX_ALL_WARNINGS_FLAG         := -Wall
#	CXX_INFORMATIONAL_WARNINGS_FLAG := -wi
#	CXX_WARNDEPRECATE_FLAG          := -dw
	CXX_IMPORTPATH_FLAG             := -I
	CXX_NOLINK_FLAG                 := -c
	CXX_OUTPUTFILE_FLAG             := -o
#	CXX_SYMBOLICDEBUGINFO_FLAG      := -g
#	CXX_INLINE_FLAG                 := -inline
	CXX_OPTIMIZE_FLAG               := -O3
#	CXX_NOBOUNDSCHECK_FLAG          := -noboundscheck
	CXX_UNITTEST_FLAG               := -DTEST_RUN
	CXX_RELEASE_FLAG                := -DRELEASE
	CXX_DEBUG_FLAG                  := -DDEBUG
#	CXX_VERSION_FLAG                := -version=
#	CXX_NOOBJECT_FLAG               := -o-
#	CXX_STATIC_LIBRARY_FLAG         := -lib
#	CXX_SHARED_LIBRARY_FLAG         := -shared
#	CXX_LINKER_FLAG                 := -L
#	CXX_DOCFILE_FLAG                := -Df
#	CXX_FPIC_FLAG                   := -fPIC
	CXX_GTEST_LIB			:= -pthread -lgtest 
endif

# implicit rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXX_NOLINK_FLAG) $^ $(CXX_OUTPUTFILE_FLAG)$@

%.html: %.cpp
	$(CXX) $(CXXFLAGS) $(CXX_NOLINK_FLAG) $(CXX_NOOBJECT_FLAG) $^ $(CXX_DOCFILE_FLAG)$@
