
#include <SmallStringOptimization/SmallStringOptimization.hpp>
#include <SmallStringOptimization/SimpleString.hpp>


#include <benchmark/benchmark.h>
#include <memory>
#include <vector>
#include <fstream>
#include <string>

using namespace NSSO;
namespace std
{
using namespace NSSO;
}

const char* gFilePath = "pan-tadeusz.txt";

std::vector<std::string> readFileLineByLine(const char* aFilePath)
{
    std::vector<std::string> output;
    std::ifstream file;
    file.open(aFilePath);

    if (!file)
    {
        return output;
    }
    
    
    for (std::string line; std::getline(file, line); )
    {
        output.push_back(line);
    }
    return output;
}

const std::vector<std::string>& getLines()
{
    static std::vector<std::string> output = readFileLineByLine(gFilePath);
    return output;
}

std::vector<std::string> getWordsFromLines(const std::vector<std::string>& aLines)
{
    std::vector<std::string> output;
    for (const auto& line : aLines)
    {
        size_t index = 0;
        size_t pos = 0;
        while (std::string::npos != (pos = line.find(' ', index)))
        {
            if (pos > index)
            {
                size_t end = pos;
                const auto c = line[end - 1];
                if (!std::isalnum(c))
                {
                    --end;
                }
                std::string word(line.data() + index, end - index);
                output.push_back(std::move(word));
            }
            index = pos + 1;
        }
        std::string word(line.data() + index, line.length() - index);
        output.push_back(std::move(word));
    }
    return output;
}

const std::vector<std::string>& getWords()
{
    static std::vector<std::string> output = getWordsFromLines(getLines());
    return output;
}
// ----------------------------------------------------


struct CWordToMap
{
    template<typename T>
    static unsigned int run(const std::vector<std::string>& aWords)
    {
        std::map<T, unsigned int> wordsOccurs;
        
        for (const auto& word : aWords)
        {
            const std::string buf = word;
            wordsOccurs[T(buf.c_str())]++;
        }
        return wordsOccurs.size();
    }
};



struct CIncreasedWordToMap
{
    static volatile char volBuf;
    
    template<typename T>
    static unsigned int run(const std::vector<std::string>& aWords)
    {
        std::map<T, unsigned int> wordsOccurs;
        char charBuffer[100];
        for (const auto& word : aWords)
        {
            const std::string buf = word + "ab";
            volBuf = buf[buf.size() / 3];
            memcpy(charBuffer, buf.c_str(), buf.size());
            charBuffer[buf.size()] = '\0';
            charBuffer[buf.size() / 2] = volBuf;
            wordsOccurs[T(charBuffer)]++;
        }
        return wordsOccurs.size();
    }
};
volatile char CIncreasedWordToMap::volBuf;



struct CSumWordToMap
{
    static volatile char volBuf1;
    static volatile char volBuf2;
    
    template<typename T>
    static unsigned int run(const std::vector<std::string>& aWords)
    {
        std::map<T, unsigned int> wordsOccurs;
        
        for (unsigned int i = 0; i < aWords.size(); i += 5)
        {
            volBuf1 = aWords[i][aWords[i].size() / 2];
            volBuf2 = aWords[i + 3][aWords[i + 3].size() / 2];
            T a{aWords[i]};
            a += aWords[i + 1];
            a += aWords[i + 2].c_str();
            a += aWords[i + 3];
            a += aWords[i + 4].c_str();
            a[a.size() / 3] = volBuf1;
            a[a.size() / 4] = volBuf2;
            wordsOccurs[a]++;
        }
        return wordsOccurs.size();
    }
};
volatile char CSumWordToMap::volBuf1;
volatile char CSumWordToMap::volBuf2;

// ---------------
template<typename TType, typename TFunctor>
void runner(benchmark::State& aState)
{
    const auto words = getWords();
    while(aState.KeepRunning())
    {
        // Prevent optimisation.
        benchmark::DoNotOptimize(TFunctor::template run<TType>(words));
        // Force writing cached values into memory.
        benchmark::ClobberMemory();
    }
}

template<typename T>
void Benchmark_String(benchmark::State& aState)
{
    runner<std::string, T>(aState);
}

template<typename T>
void Benchmark_StringOpt10(benchmark::State& aState)
{
    runner<CSmallStringOpt<10u>, T>(aState);
}

template<typename T>
void Benchmark_StringOpt20(benchmark::State& aState)
{
    runner<CSmallStringOpt<20u>, T>(aState);
}


template<typename T>
void Benchmark_SimpleString(benchmark::State& aState)
{
    runner<CSimpleString<>, T>(aState);
}


BENCHMARK_TEMPLATE(Benchmark_String, CWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt10, CWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt20, CWordToMap);
BENCHMARK_TEMPLATE(Benchmark_SimpleString, CWordToMap);


BENCHMARK_TEMPLATE(Benchmark_String, CIncreasedWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt10, CIncreasedWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt20, CIncreasedWordToMap);
BENCHMARK_TEMPLATE(Benchmark_SimpleString, CIncreasedWordToMap);


BENCHMARK_TEMPLATE(Benchmark_String, CSumWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt10, CSumWordToMap);
BENCHMARK_TEMPLATE(Benchmark_StringOpt20, CSumWordToMap);
BENCHMARK_TEMPLATE(Benchmark_SimpleString, CSumWordToMap);

BENCHMARK_MAIN();

