CUSTOM_ALLOCATORS_OBJECTS = \
        AllocationMap.o \
        AllocatorHooks.o \
        ChunkMap.o \
        ConsoleLogger.o \
        EnvironmentConfiguration.o \
        ParseConfiguration.o \
        PassThroughAllocator.o \
        PoolAllocator.o \
        PoolMap.o \
        PoolStatisticsAllocator.o \
        PoolStatistics.o \
        Pool.o \
        SizeRangeMap.o \
        StaticConfiguration.o \
        Timer.o \

CXXFLAGS = -std=c++17 -g
CPPFLAGS = -Isrc

all: libArenaAllocator.so benchmark

clean:
	rm -f libArenaAllocator.so $(CUSTOM_ALLOCATORS_OBJECTS) benchmark benchmark.o

$(CUSTOM_ALLOCATORS_OBJECTS): %.o: src/ArenaAllocator/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fno-exceptions -fno-rtti -fPIC -o $@ $<

benchmark.o: src/ArenaAllocator/benchmark.cpp
	$(CXX) -c $(CPPFLAGS) -g -o $@ $<

libArenaAllocator.so: $(CUSTOM_ALLOCATORS_OBJECTS)
	$(CXX) -shared -o $@ $^

benchmark: benchmark.o
	$(CXX) -o $@ benchmark.o

debug:
	gdb --args env LD_PRELOAD=./libArenaAllocator.so ./benchmark
