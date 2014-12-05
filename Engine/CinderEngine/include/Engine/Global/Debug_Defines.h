/******************************************************************************/
/*!
\file   Debug_Defines.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <assert.h>

namespace Framework
{
#define DEBUG_check_this_expression( _condition_ )\
  assert (_condition_)
#define DEBUG_fail_if_implemented()\
  assert (1 == 0)
}