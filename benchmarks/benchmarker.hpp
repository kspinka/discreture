#pragma once

#include "Probability.hpp"
#include "TimeHelpers.hpp"
#include "do_not_optimize.hpp"
#include "external/rang.hpp"
#include <iomanip>
#include <iostream>
#include <string>

template <class Func>
double Benchmark(Func f, int numtimes = 1)
{
    // 	f(); //warm up
    dscr::Chronometer C;
    for (int i = 0; i < numtimes; ++i)
    {
        f();
    }
    double avg = C.Reset()/numtimes;

    return avg;
}

template <class Container>
double FWIterationBenchmark(const Container& A)
{
    return Benchmark([&A]() {
        for (auto& a : A)
        {
            DoNotOptimize(a);
        }
    });
}

template <class Container>
double FWIterationBenchmarkCopy(const Container& A)
{
    return Benchmark([&A]() {
        for (auto a : A)
        {
            DoNotOptimize(a);
        }
    });
}

template <class Container>
double ReverseIterationBenchmark(const Container& A)
{
    return Benchmark([&A]() {
        for (auto it = A.rbegin(); it != A.rend(); ++it)
        {
            DoNotOptimize(*it);
        }
    });
}

template <class Container>
double ForEachBenchmark(const Container& A)
{
    return Benchmark(
      [&A]() { A.for_each([](const auto& a) { DoNotOptimize(a); }); });
}

template <class Container>
double ConstructionBenchmark(const Container& A, int numtimes)
{
    return Benchmark([&A, numtimes]() {
        for (int i = 0; i < numtimes; ++i)
        {
            auto t = dscr::random::random_int<std::int64_t>(0, A.size());
            DoNotOptimize(A[t]);
        }
    });
}
