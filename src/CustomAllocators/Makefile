ARENA_ALLOCATOR_OBJECTS=AllocatorHooks.o ArenaAllocator.o NativeAllocator.o ConsoleLogger.o ArenaMap.o Arena.o ChunkMap.o StaticConfiguration.o Timer.o
#CXXFLAGS=-g -fno-exceptions -fno-rtti -fno-threadsafe-statics
CXXFLAGS=-g

all: libArenaAllocator.so benchmark

clean:
	rm -f libArenaAllocator.so ArenaAllocator $(ARENA_ALLOCATOR_OBJECTS) benchmark benchmark.o

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fPIC -o $@ $<

benchmark.o: benchmark.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

libArenaAllocator.so: $(ARENA_ALLOCATOR_OBJECTS)
	$(CXX) -shared -o $@ $^

benchmark: benchmark.o
	$(CXX) -o $@ benchmark.o

debug:
	gdb --args env LD_PRELOAD=./libArenaAllocator.so ./benchmark
