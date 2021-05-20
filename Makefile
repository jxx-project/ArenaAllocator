ARENA_ALLOCATOR_OBJECTS=ArenaAllocator.o ArenaAllocatorHooks.o NativeAllocator.o ConsoleLogger.o ArenaMap.o Arena.o ChunkMap.o EnvironmentConfiguration.o Timer.o
#CXXFLAGS=-g -fno-exceptions -fno-rtti -fno-threadsafe-statics
CXXFLAGS=-g

all: libArenaAllocator.so benchmark

clean:
	rm -f libArenaAllocator.so ArenaAllocator $(ARENA_ALLOCATOR_OBJECTS) benchmark.o

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fPIC -o $@ $<

benchmark.o: benchmark.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

libArenaAllocator.so: $(ARENA_ALLOCATOR_OBJECTS)
	$(CXX) -shared -o $@ $^

benchmark: benchmark.o
	$(CXX) -o $@ benchmark.o
