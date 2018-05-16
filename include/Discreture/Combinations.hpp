#pragma once
#include <algorithm>
#include <numeric>

#include "CombinationsTree.hpp"
#include "CombinationsTreePrunned.hpp"
#include "CompoundContainer.hpp"
#include "Misc.hpp"
#include "NaturalNumber.hpp"
#include "NumberRange.hpp"
#include "Sequences.hpp"
#include "VectorHelpers.hpp"
#include "detail_combinations_bf.hpp" //Horrible. Do NOT read. Please. But I can't find another way. Sorry about that. If you think you can do better, please, tell me about it.

namespace dscr
{

////////////////////////////////////////////////////////////
/// \brief class of all n choose k combinations of size k of the set
/// {0,1,...,n-1}. \param IntType should be an integral type with enough space
/// to store n and k. It can be signed or unsigned. \param n the size of the set
/// \param k the size of the combination (subset). Should be an integer such
/// that n choose k is not bigger than the largest unsigned long int there is.
/// For example, typically 50 choose 25 is already larger than the largest long
/// unsigned int. # Example:
///
///		for (auto& x : combinations(6,3))
///			cout << '[' << x << "] ";
///
/// Prints out:
///
/// 	[ 0 1 2 ] [ 0 1 3 ] [ 0 2 3 ] [ 1 2 3 ] [ 0 1 4 ] [ 0 2 4 ] [ 1 2 4 ] [
/// 0 3 4 ] [ 1 3 4 ] [ 2 3 4 ] [ 0 1 5 ] [ 0 2 5 ] [ 1 2 5 ] [ 0 3 5 ] [ 1 3 5
/// ] [ 2 3 5 ] [ 0 4 5 ] [ 1 4 5 ] [ 2 4 5 ] [ 3 4 5 ]
///
/// # Example 2:
///
///	    string A = "abcde";
///		for (auto x : compound_combinations(A,3))
///			cout << x << endl;
///		Prints out:
///			a b c
///			a b d
///			a c d
///			b c d
///			a b e
///			a c e
///			b c e
///			a d e
///			b d e
///			c d e
///
///
////////////////////////////////////////////////////////////
template <class IntType, class RAContainerInt = std::vector<IntType>>
class basic_combinations
{
public:
    using value_type = RAContainerInt;
    using combination = value_type;
    using difference_type = long long; // NOLINT
    using size_type = difference_type; // yeah, signed.
    class iterator;
    using const_iterator = iterator;
    class reverse_iterator;
    using const_reverse_iterator = reverse_iterator;

    // **************** Begin static functions

    /** @name next_combination
     *@brief next_combination, which has a few different overloads (depending
     *on how much information you have about the current combination in order
     *to make it faster)
     *@param data is the current combination
     *@param hint is an integer value that holds which index was last modified
     *(in a previous call to next_combination).
     *It serves to speed things up. If unsure, use 0.
     *The overload which only takes data automatically does this.
     *@param last is data.size()-1. Tests show precomputing this value
     *is highly beneficial to performance. Contrary to what you might be
     *thinking, this is *NOT* premature optimization, since we've
     *carefully measured the impact.
     *@param n denotes that combinations should be taken from the set
     *{0,1,...,n-1}
     *@retval void is returned if starting with data, and a bool is
     *returned if starting with n, depending on whether or not we
     *are at the last possible combination.
     *Note that n is not needed if you don't need to know if this combination
     *is the "last" one.
     */
    ///@{
    //* Assumes hint = 0 and last=data.size()-1. If you don't need the utmost
    // performance, just use this one. */
    static void next_combination(combination& data)
    {
        size_type hint = 0;
        size_type last = data.size() - 1;
        next_combination(data, hint, last);
    } // next_combination data only

    //* Calculates last as data.size()-1 automatically */
    static void next_combination(combination& data, size_type& hint)
    {
        size_type last = data.size() - 1;
        next_combination(data, hint, last);
    } // next_combination data, hint

    //* Use this one for best speed */
    static void
    next_combination(combination& data, size_type& hint, IntType last)
    {
        if (hint > 0)
        {
            ++data[--hint];
            return;
        }

        assert(last + 1 == data.size());

        if (last > 0)
        {
            if (data[0] + 1 != data[1])
            {
                ++data[0];
                return;
            }
            data[0] = 0;
            IntType i = 1;

            // 			IntType i = 0;

            for (; i < last && (data[i] + 1 == data[i + 1]); ++i)
            {
                data[i] = i;
            }

            ++data[hint = i];
            return;
        }

        if (last == 0)
            ++data[0];
    } // next_combination data, hint, last

    //* This overload returns false if data is the last combination, true
    // otherwise. */
    static bool next_combination(IntType n, combination& data)
    {
        if (data.empty())
            return false;
        next_combination(data);
        return data.back() != n;
    } // next_combination n, data

    //* This overload returns false if data is the last combination, true
    // otherwise. */
    static bool next_combination(IntType n, combination& data, size_type& hint)
    {
        if (data.empty())
            return false;
        next_combination(data, hint);
        return data.back() != n;
    } // next_combination n, data hint

    //* This overload returns false if data is the last combination, true
    // otherwise. */
    static bool next_combination(IntType n,
                                 combination& data,
                                 size_type& hint,
                                 IntType last)
    {
        assert(last == data.size() - 1);
        if (last < 0) // this means data is empty
            return false;
        next_combination(data, hint, last);
        return data.back() != n;
    } // next_combination n data hint last
    ///@}

    static void prev_combination(combination& data)
    {
        prev_combination(data, data.size() - 1);
    }

    static void prev_combination(combination& data, IntType last)
    {
        if (last > 0)
        {
            if (data[0] != 0)
            {
                --data[0];
                return;
            }

            IntType i = 1;

            // Advance i until the first that can decrease: data[i] != i
            for (; i < last && (data[i] == i); ++i) {}

            --data[i];
            --i;

            for (; i >= 0; --i)
            {
                data[i] = data[i + 1] - 1;
            }

            return;
        }

        if (last == 0)
            --data[0];
    }

    static inline void construct_combination(combination& data, size_type m)
    {
        IntType k = data.size();

        size_type upper =
          68; // this is the biggest for which binomial is still well defined.
              // Hopefully it's enough for most use cases.

        for (IntType r = k; r > 1; --r)
        {
            IntType t;

            big_number_range NR(r, upper);

            t = NR.partition_point(
                  [m, r](auto x) { return binomial<size_type>(x, r) <= m; }) -
              1;
            data[r - 1] = t;
            upper = t;
            m -= binomial<size_type>(t, r);
        }
        if (k > 0)
            data[0] = m;
    }

    ///////////////////////////////////////
    /// \brief Combination comparison "less than" operator. Assumes lhs and rhs
    /// have the same size. \return true if lhs would appear before rhs in the
    /// normal iteration order, false otherwise
    ///////////////////////////////////////
    static bool compare(const combination& lhs, const combination& rhs)
    {
        assert(lhs.size() == rhs.size());
        return std::lexicographical_compare(lhs.rbegin(), lhs.rend(), rhs.rbegin(), rhs.rend());
    }

    /////////////////////////////////////////////////////////////////////////////
    /// \brief Returns the ID of the iterator whose value is comb. That is, the
    /// index of combination comb in the lexicographic order.
    ///
    /// Inverse of operator[]. If combination x is the m-th combination, then
    /// get_index(x) is m. If one has a combinations::iterator, then the member
    /// function ID() should return the same value. \return the index of
    /// combination comb, as if basic_combinations was a proper data structure
    /// \note This constructs the proper index from scratch. If an iterator is
    /// already known, calling ID on the iterator is much more efficient.
    /////////////////////////////////////////////////////////////////////////////
    static size_type get_index(const combination& comb)
    {
        const size_type k = comb.size();

        size_type result = 0;

        for (difference_type i = 0; i < k; ++i)
            result += binomial<size_type>(comb[i], i + 1);

        return result;
    }

    // **************** End static functions

public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// \param n is an integer >= 0
    /// \param k is an integer with 0 <= k <= n
    ///
    ////////////////////////////////////////////////////////////
    basic_combinations(IntType n, IntType k)
        : m_n(n), m_k(k), m_size(binomial<size_type>(n, k))
    {}

    ////////////////////////////////////////////////////////////
    /// \brief The total number of combinations
    ///
    /// \return binomial(n,r)
    ///
    ////////////////////////////////////////////////////////////
    size_type size() const { return m_size; }

    IntType get_n() const { return m_n; }
    IntType get_k() const { return m_k; }

    iterator begin() const { return iterator(m_n, m_k); }

    const iterator end() const
    {
        return iterator(size());
        // 		return iterator::make_invalid_with_id(size());
    }

    ////////////////////////////////////////////////////////////
    /// \brief Access to the m-th combination (slow for iteration)
    ///
    /// This is equivalent to calling *(begin()+m)
    /// \param m should be an integer between 0 and size(). Undefined behavior
    /// otherwise. \return The m-th combination, as defined in the order of
    /// iteration (lexicographic)
    ////////////////////////////////////////////////////////////
    combination operator[](size_type m) const
    {
        assert(m >= 0 && m < size());
        combination comb(m_k);
        construct_combination(comb, m);
        return comb;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get an iterator whose current value is comb
    ///
    /// \param comb the wanted combination
    /// \return An iterator currently pointing at comb.
    ////////////////////////////////////////////////////////////
    iterator get_iterator(const combination& comb) { return iterator(comb); }

    reverse_iterator rbegin() const { return reverse_iterator(m_n, m_k); }

    const reverse_iterator rend() const { return reverse_iterator(size()); }

    //************** Begin iterator definitions
    class iterator
        : public boost::iterator_facade<iterator,
                                        const combination&,
                                        boost::random_access_traversal_tag>
    {
    public:
        iterator() = default;

        iterator(IntType n, IntType k)
            : m_ID(0), m_last(k - 1), m_hint(k), m_data(k)
        {
            UNUSED(n);
            std::iota(m_data.begin(), m_data.end(), 0);
        }

        explicit iterator(const combination& data)
            : m_ID(get_index(data))
            , m_last(data.size() - 1)
            , m_hint(0)
            , m_data(data)
        {}

        inline bool is_at_end(IntType n) const
        {

            return m_data.empty() || m_data.back() == n;
        }

        void reset(IntType n, IntType k)
        {
            UNUSED(n);
            m_ID = 0;
            m_hint = k;
            m_last = k - 1;
            m_data.resize(k);
            std::iota(m_data.begin(), m_data.end(), 0);
        }

        size_type ID() const { return m_ID; }
        // boost::iterator_facade provides all the public interface you need,
        // like ++, etc.

        //////////////////////////////////////
        /// @brief Constructs an invalid iterator whose only purpose is to have
        /// an id (for comparison purposes)
        //////////////////////////////////////
        static iterator make_invalid_with_id(size_type id)
        {
            return iterator(id);
        }

    private:
        explicit iterator(size_type id) : m_ID(id) {}

        void increment()
        {
            next_combination(m_data, m_hint, m_last);
            ++m_ID;
        }

        bool equal(const iterator& other) const { return m_ID == other.m_ID; }

        const combination& dereference() const { return m_data; }

        ////////////////////////////////////////
        ///
        /// \brief Random access capabilities to the iterators
        /// \param n -> This assumes 0 <= n+ID <= size(n,k)
        ///
        ////////////////////////////////////////
        void advance(difference_type n)
        {
            assert(0 <= n + m_ID);

            // If n is small, it's actually more efficient to just advance to it
            // one by one. 40 was found empirically
            if (std::abs(n) < 40)
            {
                while (n > 0)
                {
                    increment();
                    --n;
                }

                while (n < 0)
                {
                    decrement();
                    ++n;
                }
            }

            // If n is large, then it's better to just construct it from
            // scratch.
            m_ID += n;
            construct_combination(m_data, m_ID);
            m_hint = 0;
        }

        difference_type distance_to(const iterator& other) const
        {
            return other.m_ID - m_ID;
        }

        void decrement()
        {
            if (m_ID == 0)
                return;

            --m_ID;
            m_hint = 0;

            prev_combination(m_data, m_last);
        }

        friend class boost::iterator_core_access;
        friend class basic_combinations;

        size_type m_ID{0};
        IntType m_last{-1}; // should always be m_data.size()-1!!!
        size_type m_hint{0};
        combination m_data{};

    }; // end class iterator

    class reverse_iterator
        : public boost::iterator_facade<reverse_iterator,
                                        const combination&,
                                        boost::random_access_traversal_tag>
    {
    public:
        reverse_iterator() = default;

        reverse_iterator(IntType n, IntType k)
            : m_n(n), m_ID(0), m_last(k - 1), m_data(k)
        {
            std::iota(m_data.begin(), m_data.end(), n - k);
        }

        inline size_type ID() const { return m_ID; }

        inline bool is_at_end() const { return m_data.back() == m_data.size(); }

        void reset(IntType n, IntType k)
        {
            m_n = n;
            m_ID = 0;
            m_last = k - 1;
            m_data.resize(k);
            std::iota(m_data.begin(), m_data.end(), n - k);
        }

        //////////////////////////////////////
        /// @brief Constructs an invalid iterator whose only purpose is to have
        /// an id (for comparison purposes)
        //////////////////////////////////////
        static reverse_iterator make_invalid_with_id(size_type id)
        {
            return reverse_iterator(id);
        }

    private:
        explicit reverse_iterator(size_type id)
            : m_ID(id) {} // ending initializer: for id only. Do not use unless
                          // you know what you are doing.

        void increment()
        {
            ++m_ID;

            prev_combination(m_data, m_last);
        }

        void decrement()
        {
            assert(m_ID != 0);

            --m_ID;

            next_combination(m_data);
        }

        difference_type distance_to(const reverse_iterator& other) const
        {
            return other.m_ID - m_ID;
        }

        const combination& dereference() const { return m_data; }

        ////////////////////////////////////////
        ///
        /// \brief Random access capabilities to the iterators
        /// \param n -> This assumes 0 <= n+ID <= size(n,k)
        ///
        ////////////////////////////////////////
        void advance(difference_type m)
        {
            assert(0 <= m + m_ID);

            if (std::abs(m) < 20)
            {
                while (m > 0)
                {
                    increment();
                    --m;
                }

                while (m < 0)
                {
                    decrement();
                    ++m;
                }

                return;
            }

            m_ID += m;
            auto num = binomial<size_type>(m_n, m_data.size()) - m_ID - 1;
            // If n is large, then it's better to just construct it from
            // scratch.
            construct_combination(m_data, num);
        }

        bool equal(const reverse_iterator& it) const { return it.m_ID == m_ID; }

    private:
        IntType m_n{0};
        size_type m_ID{0};
        IntType m_last{-1}; // this should ALWAYS be m_data.size()-1!!!!
        combination m_data{};

        friend class boost::iterator_core_access;
        friend class basic_combinations;
    }; // end class iterator

    ///////////////////////////////////////////////
    /// \brief This is an efficient way to construct a combination of size k
    /// which fully satisfies a predicate
    ///
    /// This function is conceptually equivalent to std::find_if(begin(), end(),
    /// Pred), but much faster if the predicate can be evaluated on a partial
    /// combination (so as to prune the search tree)
    ///
    /// # Example:
    ///
    /// 	combinations X(40,6);
    /// 	auto it = X.find_if([](const std::vector<int>& comb) -> bool
    /// 	{
    /// 		for (int i = 0; i < comb.size()-1; ++i)
    /// 		{
    /// 			if (2*comb[i] + 1 > comb[i+1])
    /// 				return false;
    /// 		}
    /// 		return true;
    ///		});
    /// 	cout << *it << endl;
    ///
    /// Prints out:
    /// 	[ 0 1 3 7 15 31 ]
    ///
    ///
    /// \param Pred should be what we call a *partial predicate*: It takes a
    /// combination as a parameter and returns either true or false.
    ///
    ///
    /// \return An interator to a combination which fully satisfies the
    /// predicate.
    ///
    /////////////////////////////////////////////
    template <class PartialPredicate>
    iterator find_if(PartialPredicate pred)
    {
        combination A;
        A.reserve(m_k);

        while (DFSUtil(A, pred))
        {
            if (A.size() == static_cast<size_t>(m_k))
                return get_iterator(A);
        }

        return end();
    }

    ///////////////////////////////////////////////
    /// \brief This is an efficient way to construct all combination of size k
    /// which fully satisfy a predicate
    ///
    /// This function is similar to find_if, but it returns an object for which
    /// you can iterate over all combinations which satisfy Predicate pred,
    ///
    /// # Example:
    ///
    /// 	combinations X(30,4);
    /// 	auto vall = X.find_all([](const std::vector<int>& comb) -> bool
    ///		{
    ///			if (comb.size() < 2) return true;
    ///			int k = comb.size();
    ///			if (comb[k-2] == 0) return false;
    ///			return (comb[k-1]%comb[k-2] == 0);
    ///		});
    /// 	for (auto& v : vall)
    /// 		cout << v << endl;
    ///
    ///
    /// Prints out:
    /// 	[ 1 2 4 8 ]
    /// 	[ 1 2 4 12 ]
    /// 	[ 1 2 4 16 ]
    /// 	[ 1 2 4 20 ]
    /// 	[ 1 2 4 24 ]
    /// 	[ 1 2 4 28 ]
    /// 	[ 1 2 6 12 ]
    /// 	[ 1 2 6 18 ]
    /// 	[ 1 2 6 24 ]
    /// 	[ 1 2 8 16 ]
    /// 	[ 1 2 8 24 ]
    /// 	[ 1 2 10 20 ]
    /// 	[ 1 2 12 24 ]
    /// 	[ 1 2 14 28 ]
    /// 	[ 1 3 6 12 ]
    /// 	[ 1 3 6 18 ]
    /// 	[ 1 3 6 24 ]
    /// 	[ 1 3 9 18 ]
    /// 	[ 1 3 9 27 ]
    /// 	[ 1 3 12 24 ]
    /// 	[ 1 4 8 16 ]
    /// 	[ 1 4 8 24 ]
    /// 	[ 1 4 12 24 ]
    /// 	[ 1 5 10 20 ]
    /// 	[ 1 6 12 24 ]
    /// 	[ 1 7 14 28 ]
    /// 	[ 2 4 8 16 ]
    /// 	[ 2 4 8 24 ]
    /// 	[ 2 4 12 24 ]
    /// 	[ 2 6 12 24 ]
    /// 	[ 3 6 12 24 ]
    /// which are all combinations for which every element is a divisor of the
    /// next element.
    ///
    /// \param pred should be a *partial predicate*: It takes a *partial*
    /// combination as a parameter and returns either true or false. At the end
    /// only combinations for which every subcombination evaluated to true are
    /// returned (the combination tree is prunned when Pred returns false)
    ///
    ///
    /// \return An forward-iterable object which whose elements are all
    /// combinations which satisfy predicate pred.
    ///
    ///
    /////////////////////////////////////////////
    template <class PartialPredicate>
    auto find_all(PartialPredicate pred)
    {
        return basic_combinations_tree_prunned<IntType,
                                               PartialPredicate,
                                               combination>(m_n, m_k, pred);
    }

    ////////////////////////////////////////////////////////////
    /// \brief Applies function f to each element of *this. This is faster than
    /// doing manual iteration up to size 19. After that it falls back on manual
    /// iteration.
    ///			Equivalent (but faster) to:
    ///			for (auto& x : (*this)) f(x);
    ///
    /// \param f is the function to apply. It should take a const combination&
    /// as parameter. You can use lambdas, etc.
    ///////////////////////////////////////////////////////////
    template <class Func>
    void for_each(Func f) const
    {
        // I'm really sorry about this. I don't know how to improve it. If you
        // do, by all means, tell me about it.
        switch (m_k)
        {
        case 0:
            detail::combination_helper0<combination>(f, m_n);
            break;

        case 1:
            detail::combination_helper1<combination>(f, m_n);
            break;

        case 2:
            detail::combination_helper2<combination>(f, m_n);
            break;

        case 3:
            detail::combination_helper3<combination>(f, m_n);
            break;

        case 4:
            detail::combination_helper4<combination>(f, m_n);
            break;

        case 5:
            detail::combination_helper5<combination>(f, m_n);
            break;

        case 6:
            detail::combination_helper6<combination>(f, m_n);
            break;

        case 7:
            detail::combination_helper7<combination>(f, m_n);
            break;

        case 8:
            detail::combination_helper8<combination>(f, m_n);
            break;

        case 9:
            detail::combination_helper9<combination>(f, m_n);
            break;

        case 10:
            detail::combination_helper10<combination>(f, m_n);
            break;

        case 11:
            detail::combination_helper11<combination>(f, m_n);
            break;

        case 12:
            detail::combination_helper12<combination>(f, m_n);
            break;

        case 13:
            detail::combination_helper13<combination>(f, m_n);
            break;

        case 14:
            detail::combination_helper14<combination>(f, m_n);
            break;

        case 15:
            detail::combination_helper15<combination>(f, m_n);
            break;

        case 16:
            detail::combination_helper16<combination>(f, m_n);
            break;

        case 17:
            detail::combination_helper17<combination>(f, m_n);
            break;

        case 18:
            detail::combination_helper18<combination>(f, m_n);
            break;

        case 19:
            detail::combination_helper19<combination>(f, m_n);
            break;

        default:
            for (auto& comb : (*this))
            {
                f(comb);
            }

            break;
        }
    }

private:
    IntType m_n;
    IntType m_k;
    size_type m_size;

    template <class P>
    bool augment(combination& comb, P pred, IntType start = 0)
    {
        if (comb.empty())
        {
            if (start < m_n - m_k + 1)
            {
                comb.push_back(start);
                return true;
            }
            return false;
        }

        auto last = comb.back();
        auto guysleft = m_k - comb.size();

        start = std::max(static_cast<IntType>(last + 1), start);

        for (size_t i = start; i < m_n - guysleft + 1; ++i)
        {
            comb.push_back(i);

            if (pred(comb))
                return true;

            comb.pop_back();
        }

        return false;
    }

    template <class P>
    bool DFSUtil(combination& comb, P pred)
    {
        // 			auto currsize = comb.size();

        if (comb.size() < static_cast<size_t>(m_k))
        {
            if (augment(comb, pred))
                return true;
        }

        auto last = comb.back();

        // If it can't be augmented, be it because size is already k or else, we
        // have to start backtracking
        while (!comb.empty())
        {
            last = comb.back();
            comb.pop_back();

            if (augment(comb, pred, last + 1))
                return true;
        }

        return false;
    }

}; // end class basic_combinations

using combinations = basic_combinations<int>;
using combinations_fast =
  basic_combinations<int, boost::container::static_vector<int, 32>>;

template <class Container, class IntType>
auto compound_combinations(const Container& X, IntType k)
{
    using comb = basic_combinations<IntType>;
    return compound_container<Container, comb>(X, comb(X.size(), k));
}

/**
 *\brief This function recreates next_permutation's
 *		  behaviour but for combinations. [out_first, out_last) should be
 *		  a range of **iterators** in [in_first, in_last).
 **/
template <class ForwardIt, class BiDirectionalItOut>
bool next_combination(ForwardIt in_first,
                      ForwardIt in_last,
                      BiDirectionalItOut out_first,
                      BiDirectionalItOut out_last,
                      BiDirectionalItOut& hint)
{
    if (out_first == out_last)
        return false;

    if (hint != out_first)
    {
        --hint;
        ++(*hint);
        return true;
    }

    auto out_second = out_first;
    ++out_second;
    for (; out_second != out_last; ++out_second, ++in_first, ++hint)
    {
        auto next = *hint;
        ++next;
        if (*next != **out_second)
        {
            ++(*hint);
            return true;
        }
        *hint = in_first;
    }
    ++(*hint);

    return (*hint != in_last);
}

template <class ForwardIt, class BiDirectionalItOut>
bool next_combination(ForwardIt in_first,
                      ForwardIt in_last,
                      BiDirectionalItOut out_first,
                      BiDirectionalItOut out_last)
{
    auto hint = out_first;
    return next_combination(in_first, in_last, out_first, out_last, hint);
}

} // namespace dscr
