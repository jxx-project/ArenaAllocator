ARENA_ALLOCATOR_OBJECTS=ArenaAllocator.o ArenaAllocatorHooks.o NativeAllocator.o Logger.o ArenaMap.o Arena.o ChunkMap.o
CXXFLAGS=-O

all: libArenaAllocator.so ArenaAllocator

clean:
	rm -f libArenaAllocator.so ArenaAllocator $(ARENA_ALLOCATOR_OBJECTS) main.o

.cpp.o:
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fPIC -o $@ $<

main.o: main.cpp
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

libArenaAllocator.so: $(ARENA_ALLOCATOR_OBJECTS)
	$(CXX) -shared -o $@ $^

ArenaAllocator: main.o libArenaAllocator.so
	$(CXX) -o $@ main.o -L. -lArenaAllocator
