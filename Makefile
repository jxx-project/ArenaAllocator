CUSTOM_ALLOCATORS_OBJECTS = \
        AllocationMap.o \
        AllocatorHooks.o \
        ChunkMap.o \
        ConsoleLogger.o \
        EnvironmentConfiguration.o \
        OperationType.o \
        ParseConfiguration.o \
        PassThroughAllocator.o \
        PoolAllocator.o \
        PoolMap.o \
        PoolStatisticsAllocator.o \
        PoolStatistics.o \
        Pool.o \
        SizeRangeMap.o \
        Timer.o \

CXXFLAGS = -std=c++17 -g -O3
CPPFLAGS = -Isrc -Iinclude

all: libArenaAllocator.so benchmark

clean:
	rm -f libArenaAllocator.so $(CUSTOM_ALLOCATORS_OBJECTS) benchmark benchmark.o

$(CUSTOM_ALLOCATORS_OBJECTS): %.o: src/ArenaAllocator/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fno-exceptions -fno-rtti -fPIC -o $@ $<

benchmark.o: utils/src/benchmark.cpp
	$(CXX) -c $(CPPFLAGS) -g -o $@ $<

libArenaAllocator.so: $(CUSTOM_ALLOCATORS_OBJECTS)
	$(CXX) -shared -Wl,-init=initArenaAllocator -o $@ $^

benchmark: benchmark.o
	$(CXX) -o $@ benchmark.o

debug:
	gdb --args env LD_PRELOAD=./libArenaAllocator.so ./benchmark
