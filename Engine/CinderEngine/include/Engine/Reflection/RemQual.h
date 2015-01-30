#ifndef REMQUAL_H_
#define REMQUAL_H_

// from http://www.randygaul.net/

namespace Reflection
{
	template <typename T>
	struct RemQual
	{
		typedef T Type;
	};

	template <typename T>
	struct RemQual<const T>
	{
		typedef typename RemQual<T>::Type Type;
	};

	template <typename T>
	struct RemQual<T&>
	{
		typedef typename RemQual<T>::Type Type;
	};

	template <typename T>
	struct RemQual<const T&>
	{
		typedef typename RemQual<T>::Type Type;
	};

	template <typename T>
	struct RemQual<T&&>
	{
		typedef typename RemQual<T>::Type Type;
	};

	template <typename T>
	struct RemQual<const T *>
	{
		typedef typename RemQual<T *>::Type Type;
	};
}

#endif