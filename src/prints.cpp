
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
namespace OZ
{
	namespace PRINTS
	{
		template<typename T>
		concept IsContainer = requires(T t) {
			{ t.begin() } -> std::input_or_output_iterator;
			{ t.end() } -> std::sentinel_for<decltype(t.begin())>;
		} && !std::is_same_v<T, std::string>;

		template<typename T> requires (IsContainer<T>)
			static inline void printV(const T& l)
		{
			std::cout << "[";
			if constexpr (IsContainer<typename T::value_type>)
			{
				std::cout << std::endl;
				for (auto it = l.begin(); it != l.end(); ++it)
				{
					printV(*it);
					std::cout << "," << std::endl;
				}
			}
			else if constexpr (!IsContainer<typename T::value_type>)
			{
				for (auto it = l.begin(); it != l.end(); ++it)
				{
					if constexpr (requires { std::get<0>(*it); std::get<1>(*it); })
					{
						auto temp = it;
						temp++;
						if (temp != l.end()) std::cout << std::get<0>(*it) << ":" << std::get<1>(*it) << ",";
						else std::cout << std::get<0>(*it) << ":" << std::get<1>(*it);
					}
					else {
						auto temp = it;
						temp++;
						if (temp != l.end()) std::cout << *it << ",";
						else std::cout << *it;
					}
				}
			}
			std::cout << "]";
		}
		template<typename T>
		static void print(const T& a)
		{
			if constexpr (IsContainer<T>)
			{
				printV(a);
				std::cout << std::endl;
				return;
			}
			else if constexpr (!IsContainer<T>)
			{
				const auto printable = a;
				std::cout << printable << std::endl;
			}
		}
	}
}

