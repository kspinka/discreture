# Discreture

This is a modern C++ 11 (and 14) library designed to facilitate combinatorial research by providing fast and easy iterators to a few combinatorial objects, such as combinations, permutations, partitions, and others. The idea is to have them resemble the STL containers as much as possible, without actually storing the whole set of objects in memory.

Discreture is designed to follow the STL containers as closely as possible, by providing the standard ways of iterating. In addition, many of the algorithms described in the standard <algorithm> library work as-is in these containers, as if the containers were marked as const.

# Quick preview:

```c++
    #include <iostream>
    #include <discreture.hpp>
    int main()
    {
		using namespace std;
		using namespace dscr;
		
        combinations X(5,3);
        for (auto& x : X)
            cout << x << endl;
        
        return 0;
    }
```
The above code would produce the following output:

    [ 0 1 2 ]
    [ 0 1 3 ]
    [ 0 2 3 ]
    [ 1 2 3 ]
    [ 0 1 4 ]
    [ 0 2 4 ]
    [ 1 2 4 ]
    [ 0 3 4 ]
    [ 1 3 4 ]
    [ 2 3 4 ]

Of course, you need to link with the discreture library and compile with the -std=c++14 or -std=c++11 flag:
g++ -std=c++14 -O2 -ldiscreture main.cpp

Some tests show discreture is usually faster when compiled with clang++ instead of g++. Full benchmarks at the end of the readme.

# Installation

To install on linux, do the standard cmake/make dance:
```sh
git clone https://github.com/mraggi/discreture
cd discreture
mkdir build
cd build
cmake ..
make
sudo make install
```

This will install everything under `/usr/local/` by default. If you wish to install to some other directory, replace the cmake .. above by `cmake .. -DCMAKE_INSTALL_PREFIX=/usr/`.

You can run the tests by running the executable: `./testdiscreture`

# How to start using the library
To use the library, after compiling, just add `#include <discreture.hpp>` to your project and link to `libdiscreture.so`. With the GCC compiler, this can be done by compiling like this: `g++ -ldiscreture -std=c++14 myfile.cpp`. If you wish to include only part of the library, one could do `#include <Discreture/Combinations.hpp>` for example.

# Combinatorial Objects

Within this library, one can construct a few combinatorial objects, such as:
  - Combinations
  - Permutations
  - Subsets
  - Multisets
  - Partitions
  - Dyck Paths
  - Motzkin Paths
  - Range
  - Set Partitions

All follow the same design principle: The templated class is calles basic_SOMETHING<class T>, and the most reasonable type for T is instantiated as SOMETHING. For example, `combinations` is a typedef of `basic_combinations<int>`, and `partitions` is a typedef of `basic_partitions<int>`.

# Basic usage

Although the full reference is in the doxygen documentation, here is a quick preview. Remember to always `#include <discreture.hpp>` and either add `using namespace dscr;` or add `dscr::` to everything.):

```c++
combinations X(30,10); //all subsets of size 10 of {0,1,2,...,29}
for (auto& x : X) 
{ 
	// x is of type const vector<int>&, so anything that works with 
	// const vector references also works on x, such as calling x[3], etc.
}
```

You can iterate in reverse too, in the same way you would reverse-iterate an STL container.
```c++
combinations X(30,10);
for (auto it = X.rbegin(); it != X.rend(); ++it) 
{ 
	auto& x = *it;
	// x is of type const vector<int>&, so anything that works with vectors works on x
}
```

Combinations, subsets and permutations are a random-access container (although they are slower as such than forward or reverse iteration), so something like this works too:
```c++
combinations X(30,10);
for (size_t i = 0; i < X.size(); ++i)
{
	auto x = X[i];
}
```

This is much slower if one plans to actually iterate over all of them, but iterator arithmetic is implemented, so one could even do the following:
```c++
#include <algorithm>
// ...
combinations X(30,10);
std::partition_point(X.begin(), X.end(), predicate);
```
where `predicate` is a unary predicate that takes a `const vector<int>&` as an argument and returns true or false, in a way that for all the first combinations it returns true and the last ones return false. This would do binary search.

## Combinations find_if and find_all
Combinations is the most mature part of the library, so the following functions have been implemented:

```c++
	combinations X(30,4);
	
	// T will be an iterable object whose elements are the combinations that satisfy the predicate specified by the lambda function.
	// In this case, the lambda checks that the next to last element divides the last element.
	// The elements of T will therefore be the combinations for which every element is a divisor of the next element.
	
	auto T = X.find_all([](const vector<int>& comb) -> bool
	{
		if (comb.size() < 2) return true;
		int k = comb.size();
		if (comb[k-2] == 0) return false;
		return (comb[k-1]%comb[k-2] == 0);
	});
	for (auto& t : T)
		cout << t << endl;
```
Prints out:	
	[ 1 2 4 8 ]
	[ 1 2 4 12 ]
	[ 1 2 4 16 ]
	[ 1 2 4 20 ]
	[ 1 2 4 24 ]
	[ 1 2 4 28 ]
	[ 1 2 6 12 ]
	[ 1 2 6 18 ]
	[ 1 2 6 24 ]
	[ 1 2 8 16 ]
	[ 1 2 8 24 ]
	[ 1 2 10 20 ]
	[ 1 2 12 24 ]
	[ 1 2 14 28 ]
	[ 1 3 6 12 ]
	[ 1 3 6 18 ]
	[ 1 3 6 24 ]
	[ 1 3 9 18 ]
	[ 1 3 9 27 ]
	[ 1 3 12 24 ]
	[ 1 4 8 16 ]
	[ 1 4 8 24 ]
	[ 1 4 12 24 ]
	[ 1 5 10 20 ]
	[ 1 6 12 24 ]
	[ 1 7 14 28 ]
	[ 2 4 8 16 ]
	[ 2 4 8 24 ]
	[ 2 4 12 24 ]
	[ 2 6 12 24 ]
	[ 3 6 12 24 ]
which are all combinations for which every element is a divisor of the next element. This is done lazily.

# Benchmarks.

## Vs GSL. 

The GNU Scientific Library is a well known and mature library. For more information, [check their website](https://www.gnu.org/software/gsl/). It implements combinations. Iterating over all subsets of size n/2 over a set of size n was took the following time:

![discreture::combinations vs GSL combinations](https://github.com/mraggi/discreture/blob/master/combvsgsl2.png "discreture::combinations vs GSL combinations")

The GSL code used was the following:

```c++
gsl_combination * c;
size_t i = 0;

for (int n = 8; n < 34; n += 1)
{
	c = gsl_combination_calloc (n, n/2);
	do
	{
		if (gsl_combination_get(c,3) == 1)
			++i;
	}
	while (gsl_combination_next (c) == GSL_SUCCESS);
	gsl_combination_free (c);	
}
```

Compare this to the same code using discreture:
```c++
size_t i = 0;
for (int n = 8; n < 34; n += 1)
{
	combinations X(n,n/2);
	for (auto& x : X)
	{
		if (x[3] == 1)
			++i;
	}
}
```

## Discreture vs Sagemath

This comparison isn't even fair (c++ vs python). On the same system, iterating over all (24 choose 12) combinations, sage takes 12.2 seconds. Discreture takes approximately 0.01 seconds. No point in graphing that.

## CLANG vs GCC
On a i7-5820K CPU @ 3.30GHz, on Linux, compiling with -Ofast yields the following results:

| Task | Time taken CLANG 3.7.0 | Time taken GCC 5.2.0 |
| ------------- |:-------------:| :-------------:|
| Time taken to see all (32 choose 16) = 601080390 combinations 					|	 **2.29281s**		|   3.36332s   |
| Time taken to see all (32 choose 16) = 601080390 combinations in reverse order 	|	 **1.67853s**		|   3.98176s   |
| Time taken to see all 12! = 479001600 permutations								|	   1.70865s  		| **1.33693s** |
| Time taken to see all 2^29 = 536870912 subsets 									|	   2.54663s  		| **2.14877s** |
| Time taken to see all 2^29 = 536870912 subsets (fast mode) 						|	   2.10764s  		| **1.84649s** |
| Time taken to see all 56634173 partitions of size 90 								|	 **1.41834s**		|   1.48321s   |
| Time taken to see all 559872000 multisets 										|	 **1.84566s**		|   1.90435s   |
| Time taken to see all 477638700 dyck paths of size 18 							|	 **2.16288s**		|   2.74891s   |
| Time taken to see all 50852019 motzkin paths of size 20 							|	 **1.30359s**		|   1.46971s   |
| Time taken to see all 27644437 set partitions of size 13 							|	   0.960195s  		| **0.79946s** |
| Time taken to see all 42355950 set partitions a set of 15 elements with 4 parts 	|	   1.20166s  		| **1.01687s** |
| **Total Time**																	|	 **19.7s**			|	22.1s	   |

# Acknowledgements
I would like to thank Manuel Alejandro Romo de Vivar (manolo) for his work on dyck paths, motzkin paths, and his contribution to partition numbers.

# Contributing
Please help us testing, debugging, benchmarking, packaging for the various distros, etc. Also, if you use discreture for your own purposes, let us know!
