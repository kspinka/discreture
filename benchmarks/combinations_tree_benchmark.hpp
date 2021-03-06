#pragma once

#include "CombinationTree.hpp"
#include "Combinations.hpp"
#include "do_not_optimize.hpp"

using boost::container::static_vector; // NOLINT

inline void BM_CombinationTreeNAP(int n, int k)
{
    using cts = dscr::CombinationTree<int, static_vector<int, 32>>;
    cts::combination comb(k);
    std::iota(comb.begin(), comb.end(), 0);
    int count = 0;
    auto size = dscr::binomial<std::int64_t>(n, k);
    for (; count < size; ++count)
    {
        DoNotOptimize(comb);
        cts::next_combination(comb, n);
    }
}

inline std::int64_t BM_CombinationTreeFindAll(int n, int k)
{
    using cts = dscr::CombinationTree<int, static_vector<int, 32>>;
    auto W = dscr::combination_tree_stack(n, k);
    auto T = W.find_all([](const cts::combination& A) {
        if (A.size() < 2)
            return true;

        std::int64_t k = A.size();
        return A[k - 1] > A[k - 2] + 2;
    });

    std::int64_t size = 0;
    for (auto& t : T)
    {
        DoNotOptimize(t);
        ++size;
    }
    return size;
}

#include "external/euler314_combination_iterator.hpp"

inline void BM_CombinationTreeEuler314(int n, int k)
{
    auto end = combination_iterator<std::int64_t>();

    for (auto it = combination_iterator<std::int64_t>(n, k); it != end; ++it)
    {
        DoNotOptimize(*it);
    }
}

#ifdef TEST_GSL_COMBINATIONS
#include <gsl/gsl_combination.h>
inline void BM_CombinationTreeGSL(int n, int k)
{
    gsl_combination* c;

    c = gsl_combination_calloc(n, k);

    do
    {
        DoNotOptimize(*c);
    } while (gsl_combination_next(c) == GSL_SUCCESS);

    gsl_combination_free(c);
}
#endif
