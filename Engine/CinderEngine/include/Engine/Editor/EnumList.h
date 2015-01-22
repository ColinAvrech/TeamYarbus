#ifndef PANELENUMLIST_H_
#define PANELENUMLIST_H_

#include <vector>
#include <string>

namespace Panel
{

	class EnumList
	{
	public:

		struct EnumEntry
		{
			EnumEntry( const std::string & name, unsigned int value );
			std::string mName;
			std::string mValue;
		};

		void PushBack( const EnumEntry & s );
		std::string GetList( );
		void Clear( );
		bool IsEmpty( ) const;

	private:
		std::vector<EnumEntry> mEnumList;
	};

}

#endif