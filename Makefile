CUSTOM_ALLOCATORS_OBJECTS=AllocatorHooks.o ArenaAllocator.o NativeAllocator.o ConsoleLogger.o ArenaMap.o Arena.o ChunkMap.o StaticConfiguration.o Timer.o
CXXFLAGS=-O3
CPPFLAGS=-Isrc
all: libCustomAllocators.so benchmark

clean:
	rm -f libCustomAllocators.so $(CUSTOM_ALLOCATORS_OBJECTS) benchmark benchmark.o

$(CUSTOM_ALLOCATORS_OBJECTS): %.o: src/CustomAllocators/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -fno-exceptions -fno-rtti -fPIC -o $@ $<

benchmark.o: src/CustomAllocators/benchmark.cpp
	$(CXX) -c $(CPPFLAGS) -g -o $@ $<

libCustomAllocators.so: $(CUSTOM_ALLOCATORS_OBJECTS)
	$(CXX) -shared -o $@ $^

benchmark: benchmark.o
	$(CXX) -o $@ benchmark.o

debug:
	gdb --args env LD_PRELOAD=./libCustomAllocators.so ./benchmark
