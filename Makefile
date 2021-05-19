ARENA_ALLOCATOR_OBJECTS=ArenaAllocator.o ArenaAllocatorHooks.o NativeAllocator.o Logger.o ArenaMap.o Arena.o ChunkMap.o
CXXFLAGS=-O

all: libArenaAllocator.so benchmark

clean:
	rm -f libArenaAllocator.so ArenaAllocator $(ARENA_ALLOCATOR_OBJECTS) benchmark.o

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fPIC -o $@ $<

benchmark.o: benchmark.cpp
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

libArenaAllocator.so: $(ARENA_ALLOCATOR_OBJECTS)
	$(CC) -shared -o $@ $^

benchmark: benchmark.o libArenaAllocator.so
	$(CXX) -o $@ benchmark.o -L. -lArenaAllocator
