#include "Precompiled.h"
#include "MetaContainer.h"

#include <array>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

namespace Reflection
{
	void testFunction( )
	{
		// array
		// std::array<int, 10> testArr;
		// MetaContainer* containerArr = Reflection::CreateContainer( testArr );
		// 
		// vector
		std::vector< unsigned int > testVector;
		testVector.push_back( 1 );
		testVector.push_back( 2 );
		testVector.push_back( 3 );
		MetaContainer * containerVec = Reflection::CreateContainer( testVector );
		// 
		// // list
		// std::list< unsigned int > testList;
		// MetaContainer * containerList = Reflection::CreateContainer( testList );
		// 
		// // forward list
		// std::forward_list< unsigned int > testFList;
		// MetaContainer * containerFList = Reflection::CreateContainer( testFList );
		// 
		// // deque
		// std::deque< unsigned int > testDeque;
		// MetaContainer * containerDeque = Reflection::CreateContainer( testDeque );
		// 
		// // set
		// std::set <std::string> testSet;
		// MetaContainer * containerSet = Reflection::CreateContainer( testSet );
		// 
		// // map
		// std::map< std::string, int > testMap;
		// MetaContainer * containerMap = Reflection::CreateContainer( testMap );
		// 
		// unordered set
		// std::unordered_set< int > testUSet;
		// MetaContainer * containerUSet = Reflection::CreateContainer( testUSet );
		// 
		// // unordered map
		// std::unordered_map< int, int > testUMap;
		// MetaContainer * containerUMap = Reflection::CreateContainer( testUMap );
		
		auto begin = containerVec->Begin( &testVector );
		auto end = containerVec->End( &testVector );
		
		while ( *begin != *end )
		{
			std::cout << * reinterpret_cast<unsigned int * >(**begin) << std::endl;
			++*begin;
		}
	}



}