/*!

/auther Micah
/file ComponentTypeIds.h

/brief An enumeration of all components in the engine. This list may be subject to
to self-generation in the future


NOTE: it is possible to do this with strings, and It should be considered after
the event system has been created since that will have a majority of the same
code required to do this.

*/

#pragma once

namespace Framework
{

  /*!An enumeration of identifers for each component to identify then and sort
  then within the ComponentArray*/
  enum ComponentTypeId
  {
    //!Invalid component id
    CT_None = 0,
    //! ADD MORE! Graphics:
    CT_Camera, 
    CT_Sprite,
    //! a little more Physic:
    CT_Body,
    CT_BoxColider,
    //! some candy? GameLogic:
    CT_Controler,
    CT_BurnTime,
    //!Max Components
    CT_MaxComponents,
  };
}