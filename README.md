# Small String Optimization
The simplest implementation string class that can hold a char array is done by dynamic memory allocation - std::string dynamically allocates memory for literals. The cost of dynamic memory allocation can be huge. Moreover, in most cases there are small strings used. Therefore, there can be many dynamic allocations for several bytes. To increase performance many strings implementations use a small string optimization approach. It means that the string implementation has a compilation time size array, that is used for small literals, but longer literals are stored in a dynamic allocated memory. The drawback of a small string optimization is that it increases data for empty or very small strings. 
std::string usually follows small string optimization approach.

This repository contains implementations of:
- CSimpleString - string implementation without small string optimization
- CSmallStringOpt - string implementation with small string optimization. The size of the array of for small string optimization is set by a template argument.


## Benchmarks
At the very beginning, before any benchmark start, there read a file that contains poem 'Pan Tadeusz'. Each word is stored separately in std::vector. There 3 benchamarks used. Each benmarks is used against CSimpleString, CSmallStringOpt with 10 characters string optimization, , CSmallStringOpt with 20 characters string optimization and std::string.
1) word counter
2) concatation of 3 words to one string and histogram of this string is calculated
3) concatation of 5 words to one string and histogram of this string is calculated

```
Running ./SmallStringOptimizationBenchmarks
Run on (12 X 2200 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 9216 KiB (x1)
Load Average: 0.93, 1.10, 1.20
--------------------------------------------------------------------------------------
Benchmark                                            Time             CPU   Iterations
--------------------------------------------------------------------------------------
Benchmark_String<CWordToMap>                  19502166 ns     19497912 ns           34
Benchmark_StringOpt10<CWordToMap>             20908626 ns     20906000 ns           32
Benchmark_StringOpt20<CWordToMap>             18948634 ns     18948250 ns           36
Benchmark_SimpleString<CWordToMap>            22868819 ns     22868276 ns           29
Benchmark_String<CIncreasedWordToMap>         19591234 ns     19591057 ns           35
Benchmark_StringOpt10<CIncreasedWordToMap>    23314253 ns     23310517 ns           29
Benchmark_StringOpt20<CIncreasedWordToMap>    19459188 ns     19457514 ns           35
Benchmark_SimpleString<CIncreasedWordToMap>   24184942 ns     24180821 ns           28
Benchmark_String<CSumWordToMap>                8906361 ns      8905587 ns           75
Benchmark_StringOpt10<CSumWordToMap>          14029428 ns     14027041 ns           49
Benchmark_StringOpt20<CSumWordToMap>          11456929 ns     11454322 ns           59
Benchmark_SimpleString<CSumWordToMap>         15372860 ns     15367867 ns           45
```

