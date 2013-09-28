#pragma once

#include<tuple>
namespace tuple_helper{

	namespace deteil{
		template<size_t i>
		struct _tuple_for_each_impl
		{
			template < class Tuple, class Visitor>
			static inline void loop(Visitor &visitor, Tuple & tuple)
			{
				_tuple_for_each_impl<i-1>::loop(visitor, tuple);
				visitor(std::get<i>(tuple));
			}
		};
		template<>
		struct _tuple_for_each_impl<0>
		{
			template < class Tuple, class Visitor>
			static inline void loop(Visitor &visitor, Tuple & tuple)
			{
				visitor(std::get<0>(tuple));
			}
		};



	}

	template<class Tuple,class Visitor>
	Visitor& tuple_foreach(Tuple&tuple, Visitor& visitor)
	{
		static const size_t N = std::tuple_size<Tuple>::value;
		deteil::_tuple_for_each_impl<N - 1>::loop(visitor,tuple);
		return visitor;
	}

	namespace deteil{
		class _Tuple_hash_Help_Visitor
		{
		public:
			_Tuple_hash_Help_Visitor()
				: hash(0)
			{}
			template<class T>
			void operator()(const T&x)
			{
				hash^=std::hash<T>()(x);
			}
			std::size_t get()const
			{
				return hash;
			}
		private:
			size_t hash;
		};
	}


	





}

namespace std{

	template<class ...U>
	class hash<std::tuple<U...>>
	{
	public:
		typedef std::tuple<U...> Tuple;
		std::size_t operator()(const Tuple&tuple)
		{
			using namespace tuple_helper;
			return tuple_foreach(tuple, deteil::_Tuple_hash_Help_Visitor()).get();
		}

	};





}