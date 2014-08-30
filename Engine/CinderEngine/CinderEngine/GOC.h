/*!

/auther Micah
/file GOC.h

/brief Game Object Compositions make up everything in the Cinder Engine.
They are the core of every object and hold a Vector of pointers to all of
their components. A component is a part of an object (ie. Physics, graphic,
AI, controlers). Game Object Composition have three different states which
they should be in during their lifespan.

-Constructed: The Composition has been created and there are components
which are being added to it.  Data on the components is being added via
serialization or hard code.

-Intialization: After all data have been added, the composition is
ready to be used. If the game is running the systems will then
access each aplicable component when running their update function.

-R.I.P. : Composition is about to die and will be deleted by the
factory in the next loop.

*/

#pragma once

#include <stdint.h>           // uint32_t
#include <vector>             // std::vector<GameComponent*>
#include "ComponentTypeIds.h" // ComponentTypeId
#include "Component.h"
#include "Vec2.h"

namespace Framework
{
  //!Game Object Id Type. I unique id is given to every Game Object Composition
  typedef uint32_t GOCId;

  /*!Game Components are stored in a sorted Vector so they can be easily found.
  for obvious reasons we use pointers to components so that we can sort with ease*/
  typedef std::vector<GameComponent*> ComponentArray;

  //! Also known as GOC
  class GameObjectComposition
  {
  public:
    /*!Allows the Factory to create GameObjectCompositions
    This also prevents other classes from doing naughty things.*/
    friend class GameObjectFactory;

    /*!Every GOC has a position*/
    Vec2 GOCPosition;

    /*!TODO add a way to talk to components through
    The event system*/

    /*!Finds a Game Component on this Game Object Composition
    -Returns NULL if the component was not found*/
    GameComponent* GetComponent(ComponentTypeId TypeId);

    /*!Type safe way of accessing Components. Takes the component type to relate
    to its specific enum.*/
    template<typename componentname>
    componentname* GetComponentType(ComponentTypeId typeId);

    /*!Initialize all components on the game object composition.*/
    void Initialize();

    /*!Destroy a Game Object Composition (object) by adding it to the
    factory's list to be deleted*/
    void Destroy();

    /*!Add a component to the composition*/
    void AddComponent(ComponentTypeId typeId, GameComponent *component);

    /*!Get the game Object Composition (and also the object's) id*/
    GOCId GetId(){ return ObjectId; }

  private:

    /*!The Game Object Compositions's Vector of Components.
    this are sorted so we can use a binary search to find the
    correct component.*/
    ComponentArray Components;

    /*!A unique id gor each Game Object Composition (Object) for
    reference*/
    GOCId ObjectId;

    /*!Game object Compositions can only be created by the factory.
    We make the constructor and Destructor private to prevent the
    creation of a GOC by any other means.*/

    //!FACTORY USE ONLY: Use CreateEmptyComposition on factory is you want a composition
    GameObjectComposition();
    //!FACTORY USE ONLY: use Destroy, and the factory will take care of the rest.
    ~GameObjectComposition();



  };

  //!A acrunub for Game Object Composition
  typedef GameObjectComposition GOC;

  /*!A Safe way of accessing Components. To interface with a
  combponent create a pointer to the component the use Object->has(component)
  ex:
  Transform* transform = Object->has(Transform);
  BoxColider* boxcolider = Object->has(BoxColider);*/
  template<typename componentname>
  componentname* GameObjectComposition::GetComponentType(ComponentTypeId typeId)
  {
    return static_cast<componentname*>(GetComponent(typeId));
  }

  /*!This define allows us to have CT_componentname for our enumerated components.
 
  the ## takes the identifier on the left and right and create 1 new identifyer, can
  be used on operations as well.
  More Details on this preprocessor function here: https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html#Concatenation */
#define has(componentname) GetComponentType<componentname>( CT_##componentname)

}