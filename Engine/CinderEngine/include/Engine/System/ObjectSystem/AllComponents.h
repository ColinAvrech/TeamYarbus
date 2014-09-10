/******************************************************************************/
/*!
\file   Component.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The GameComponent class is inherited by all components. It has
an initialize function which will talk with the other system to set
itself up. (ex. Sprite talks to Graphics, Colliders talk to physics
relaying information)
*/
/******************************************************************************/

#pragma once

#include "Component.h"

// Include all components that are added here:

#include "Transform.h"
#include "Sprite.h"