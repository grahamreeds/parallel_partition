#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

template<typename Iter>
struct swap_block
{
	void operator()(Iter first, Iter mid, Iter last)
	{	// calculate the most efficient?
		std::rotate(first, mid, last);
	}	
};

template<typename Iter, typename Pred>
struct partition_block
{
	void operator()(Iter first, Iter last, Pred p, Iter& result)
	{
		result = std::partition(first, last, p);
	}
};

template<typename Iter, typename T, typename Pred = std::less<T>>
Iter parallel_partition(Iter first, Iter last, Pred p);
{
	auto n = std::thread::hardware_concurrency();
	auto l = std::distance(first, last);
	auto b = l/n;

	std::vector<Iter> r(n);
	std::vector<std::thread> t(n);

	auto begin = first;
	for (auto i=0u; i<n-1; ++i)
	{	// improve by using async+lambda?
		auto end = std::next(begin, b)
		t[i] = std::thread(partition_block<Iter,Pred>(), begin, end, std::ref(r[i]));
		begin = end;
	}
	t[n-1] = std::thread(partition_block<Iter,Pred>()(begin, v.end(), std::ref(r[n-1]));
	std::for_each(first, last, std::mem_fn(&std::thread::join));	

	for (auto i=n-2; i>0; --i)
	{
		auto begin = r[i];
		auto mid = std::next(first, b * (i + 1));
		auto end = last;
		swap_block<Iter>()(begin, mid, last);
	}
	swap_block<Iter>()(r[0], std::next(first, b), last);
	
	auto p = std::find_if_not(first, last, p);

	return p;
}

int main()
{
	std::vector<int> v(100);
	std::iota(v.begin(), v.end(), 0);
	
	std::random_device rd;
	std::mt19337 g(rd());
	std::shuffle(v.begin(), v.end(), g);

	auto l = [](int i) { return i < 50; };
	auto p = parallel_partition(v.begin(), v.end(), l);
	
	std::cout << (std::is_partitioned(v.begin(), v.end(), l) ? "sorted" : "not sorted") << std::endl;
	auto i = 0;
	for (auto d : v)
	{
		std::cout << i++ << " " << d << std::endl;
	}
}