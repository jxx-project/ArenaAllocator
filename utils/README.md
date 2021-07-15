## allocatorLoadTest

### Configuration
```
export ARENA_ALLOCATOR_CONFIGURATION='{pools:{[1,8]:512,[9,16]:512,[17,32]:512,[33,64]:512,[65,128]:256,[129,256]:128,[257,512]:64,[513,1024]:32,[1025,2048]:16,[2049,4096]:8,[4097,8192]:8,[8193,16384]:8,[16385,32768]:8,[32769,65536]:8,[65537,131072]:8},class:SegregatedFreeLists,logLevel:TRACE,logger:TimeTrace}'
```
### Execution
```
LD_PRELOAD=/usr/local/lib/libArenaAllocator.so utils/allocatorLoadTest
```

## timeTraceDistribution

### Execution
```
LD_PRELOAD=/usr/local/lib/libArenaAllocator.so utils/allocatorLoadTest 2>&1 | utils/timeTraceDistribution
```
