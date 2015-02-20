#ifndef METACONTAINER_H_
#define METACONTAINER_H_

#include "MetaCommon.h"

#include <string>
#include <functional>
#include <allocators>

namespace Reflection
{

	//////////////////////////////////////////////////
	// Meta Container Iterator
	//////////////////////////////////////////////////

	class MetaContainerIterator
	{

	public:
		MetaContainerIterator( ) = default;
		virtual ~MetaContainerIterator( ) { };

		virtual MetaContainerIterator & operator++ ( ) = 0;

		virtual bool operator!= ( const MetaContainerIterator & rhs ) const = 0;
		virtual bool operator== ( const MetaContainerIterator & rhs ) const = 0;

		virtual void * operator* ( ) = 0;
		// virtual const void * operator* ( ) const = 0;

	};

	template <typename IteratorType>
	class MetaConatinerIteratorImplementation : public MetaContainerIterator
	{

	public:
		MetaConatinerIteratorImplementation( IteratorType it ) : mIt( it ) { }
		~MetaConatinerIteratorImplementation( ) = default;

		static unsigned Sizeof( ) { return sizeof( MetaConatinerIteratorImplementation<IteratorType> ); }

		MetaContainerIterator & operator++ ( ) { ++mIt; return *this; }

		bool operator!= ( const MetaContainerIterator & rhs ) const
		{
			return mIt != static_cast< const MetaConatinerIteratorImplementation<IteratorType> * >( &rhs )->mIt;
		};
		bool operator== ( const MetaContainerIterator & rhs ) const { return !operator!=( rhs ); }

		void * operator* ( ) { return const_cast< void * >(reinterpret_cast< const void* >( &( *mIt ) ) ); }
		// const void * operator* ( ) const { return reinterpret_cast< const void* >( &( *mIt ) ); }

	private:
		IteratorType mIt;

	};

	//////////////////////////////////////////////////
	// Meta Container
	//////////////////////////////////////////////////

	class MetaContainer
	{

	public:
		MetaContainer( ) = default;
		virtual ~MetaContainer( ) { }

		HashedNameType mKeyType = 0;
		HashedNameType mValueType = 0;

		virtual MetaContainerIterator * Begin( void * container ) = 0;
		virtual MetaContainerIterator * End( void * container ) = 0;

		virtual MetaContainerIterator * Begin( const void * conatiner ) const = 0;
		virtual MetaContainerIterator * End( const void * container ) const = 0;

	};

	template <typename ContainerType>
	class MetaContainerImplementation : public MetaContainer
	{

	public:
		MetaContainerImplementation( ) = default;
		~MetaContainerImplementation( ) = default;

		MetaContainerIterator * Begin( void * container )
		{
			return new MetaConatinerIteratorImplementation<ContainerType::iterator>( 
				static_cast<ContainerType *>(container)->begin( ) );
		}
		
		MetaContainerIterator * End( void * container )
		{
			return new MetaConatinerIteratorImplementation<ContainerType::iterator>(
				static_cast<ContainerType *>( container )->end( ) );
		}

		MetaContainerIterator * Begin( const void * container ) const
		{
			return new MetaConatinerIteratorImplementation<ContainerType::const_iterator>(
				static_cast<const ContainerType *>( container )->cbegin( ) );
		}

		MetaContainerIterator * End( const void * container ) const
		{
			return new MetaConatinerIteratorImplementation<ContainerType::const_iterator>(
				static_cast<const ContainerType *>( container )->cend( ) );
		}

	};

	//////////////////////////////////////////////////
	// Creation Function
	//////////////////////////////////////////////////

	// stack and queue not supported because iterators are not supported for them

	// not valid
	template <typename T>
	MetaContainer * CreateContainer( T & )
	{
		return nullptr;
	}

	// array
	template< template<class T, size_t N> class ContainerType, typename MemberType, size_t Size >
	MetaContainer * CreateContainer( ContainerType<MemberType, Size> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<MemberType, Size> >( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	// vector, list, forward list, deque
	template< template<class T, class = std::allocator<T>> class ContainerType, typename MemberType >
	MetaContainer * CreateContainer( ContainerType<MemberType> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<MemberType> >( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	// set
	template< template<class T, class = std::less<T>, class = std::allocator<T> > class ContainerType, typename MemberType >
	MetaContainer * CreateContainer( ContainerType<MemberType> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<MemberType> >( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	// map
	template< template<class K, class T, class = std::less<K>, class = std::allocator< std::pair<const K, T>> > class ContainerType, typename KeyType, typename MemberType >
	MetaContainer * CreateContainer( ContainerType<KeyType, MemberType> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<KeyType, MemberType> >( );
		cont->mKeyType = Reflection::MetaTypeToHash<KeyType>( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	// unordered set
	template< template<class T, class = std::hash<T>, class = std::equal_to<T>, class = std::allocator<T> > class ContainerType, typename MemberType >
	MetaContainer * CreateContainer( ContainerType<MemberType> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<MemberType> >( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	// unordered map
	template< template<class K, class T, class = std::hash<K>, class = std::equal_to<K>, class = std::allocator< std::pair<const K, T>> > class ContainerType, typename KeyType, typename MemberType >
	MetaContainer * CreateContainer( ContainerType<KeyType, MemberType> & container )
	{
		MetaContainer * cont = new MetaContainerImplementation< ContainerType<KeyType, MemberType> >( );
		cont->mKeyType = Reflection::MetaTypeToHash<KeyType>( );
		cont->mValueType = Reflection::MetaTypeToHash<MemberType>( );
		return cont;
	}

	void testFunction( );

}

#include "MetaContainer.inl"

#endif