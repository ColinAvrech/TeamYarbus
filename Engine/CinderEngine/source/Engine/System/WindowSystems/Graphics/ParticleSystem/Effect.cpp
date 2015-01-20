/******************************************************************************/
/*!
\file   Effect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
	ZilchDefineType(IEffect, CinderZilch) //, builder, type
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	}
	ZilchDefineType(EffectFactory, CinderZilch) //, builder, type
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	}
  std::shared_ptr<IEffect> EffectFactory::create (const char *name)
  {
    string effect{ name };

    if (effect == "tunnel")
      return std::make_shared<TunnelEffect> ();
    else if (effect == "attractors")
      return std::make_shared<AttractorEffect> ();
    else if (effect == "fountain")
      return std::make_shared<PlayerEffect> ();
    else if (effect == "explosion")
      return std::make_shared<ExplosionEffect> ();
    else
      return nullptr;
  }

  std::shared_ptr<IEffect> EffectFactory::create(String name)
  {
	  string effect{ name.c_str() };

	  if (effect == "tunnel")
		  return std::make_shared<TunnelEffect>();
	  else if (effect == "attractors")
		  return std::make_shared<AttractorEffect>();
	  else if (effect == "fountain")
		  return std::make_shared<PlayerEffect>();
	  else if (effect == "explosion")
		  return std::make_shared<ExplosionEffect>();
	  else
		  return nullptr;
  }

}