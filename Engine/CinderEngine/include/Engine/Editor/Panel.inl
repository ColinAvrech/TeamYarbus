namespace Panel
{
	template <typename T>
	void Panel::AddField( const std::string & label,
						  T * value,
						  const std::string & group,
						  bool readOnly )
	{
		// not a anttweakbar known type
		if ( PanelType<T>::mType == TW_TYPE_UNDEF )
			__debugbreak( );

		int r = 0;
		std::ostringstream oss;

		if ( group.empty( ) == false )
			oss << " group='" << group << "' ";

		// force float point value to have different precision
		if ( PanelType<T>::mType == TW_TYPE_FLOAT || PanelType<T>::mType == TW_TYPE_DOUBLE )
			oss << "step=0.01 precision=23";

		if ( readOnly == false )
			r = TwAddVarRW( mPtr, ( std::to_string( mCount ) + label ).c_str( ), PanelType<T>::mType, value, oss.str( ).c_str( ) );
		else
			r = TwAddVarRO( mPtr, ( std::to_string( mCount ) + label ).c_str( ), PanelType<T>::mType, value, oss.str( ).c_str( ) );

		TwDefine( ( std::string( " '" ) + mName + "'/'" + std::to_string( mCount ) + label + "'  label='" + label + "' " ).c_str( ) );
		++mCount;
	}

	template <>
	void Panel::AddField( const std::string & label,
						  Vec2 * value,
						  const std::string & group,
						  bool readOnly )
	{
		float * ptr = reinterpret_cast< float * >( value );

		AddField( label + ".x", ptr, label, readOnly );
		AddField( label + ".y", ptr + 1, label, readOnly );

		std::ostringstream oss;
		oss << "'" << mName << "'/" << label << " group=" << group;

		TwDefine( oss.str( ).c_str( ) );
		TwDefine( ( "'" + mName + "'/" + label + " opened=false" ).c_str( ) );
	}

	template <>
	void Panel::AddField( const std::string & label,
						  Vec4 * value,
						  const std::string & group,
						  bool readOnly )
	{
		float * ptr = reinterpret_cast< float * >( value );

		AddField( label + ".x", ptr, label, readOnly );
		AddField( label + ".y", ptr + 1, label, readOnly );
		AddField( label + ".z", ptr + 2, label, readOnly );
		AddField( label + ".w", ptr + 3, label, readOnly );

		std::ostringstream oss;
		oss << "'" << mName << "'/" << label << " group=" << group;

		TwDefine( oss.str( ).c_str( ) );
		TwDefine( ( "'" + mName + "'/" + label + " opened=false" ).c_str( ) );
	}

	template <typename IteratorType>
	void Panel::AddContainer( const std::string & label,
							  IteratorType first,
							  IteratorType last,
							  const std::string & group,
							  bool readOnly )
	{
		for ( unsigned i = 0; first != last; ++first, ++i )
			AddField( label + "_" + std::to_string( i ), &*first, group, readOnly );

		std::ostringstream oss;
		oss << "'" << mName << "'/" << label << " group=" << group;

		TwDefine( oss.str( ).c_str( ) );
		TwDefine( ( "'" + mName + "'/" + label + " opened=false" ).c_str( ) );
	}

	template <typename T>
	void Panel::AddFieldCB( const std::string & label,
							void * obj,
							GetCBType getCB,
							SetCBType setCB,
							const std::string & group )
	{
		std::ostringstream oss;
		if ( group.empty( ) == false )
			oss << " group='" << group << "' ";

		ErrorCheck( TwAddVarCB( mPtr, ( std::to_string( mCount ) + label ).c_str( ), PanelType<T>::mType, set, get, obj, oss.str( ).c_str( ) ) );
		
		TwDefine( ( std::string( " '" ) + mName + "'/'" + std::to_string( mCount ) + label + "'  label='" + label + "' " ).c_str( ) );
		++mCount;
	}

}