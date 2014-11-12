/******************************************************************************/
/*!
\file   Cinder_Engine_Data_Types.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_DATA_TYPES
#define _CINDER_ENGINE_DATA_TYPES

namespace Framework
{
  typedef unsigned int	  INDEX;
  typedef int				      INT32;
  typedef short			      INT16;
  typedef unsigned int    UINT32;
  typedef unsigned char	  UINT8;
  typedef float			      REAL32;
  typedef int				      COUNTER;
  typedef char			      CHAR;
  typedef unsigned char	  UCHAR;
#undef					        VOID
  typedef void			      VOID;
#undef					        BOOL
#define BOOL			      char
#define BOOLEAN_true	  true
#define BOOLEAN_false	  false


#define CINDER_LOOP_THROUGH_TABLE( _index_, _table_ ) \
  \
  for (\
  ; \
  _index_ < _table_.size (); \
  _index_++ \
  )

#define CINDER_LOOP_THROUGH_INDEX( _index_, _start_index_value_, _end_index_value ) \
  \
  for (\
  _index_ = _start_index_value_; \
  _index_ <= _end_index_value; \
  _index_++ \
  )
}

#endif