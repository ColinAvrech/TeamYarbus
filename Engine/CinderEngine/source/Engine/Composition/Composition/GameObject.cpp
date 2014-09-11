/******************************************************************************/
/*!
\file   GOC.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The Game Object Composition (GOC) makes up all objects in the game engine.
it holds an array of pointers to its components which will be initialized before
the level runs.
*/
/******************************************************************************/

  /*
  struct ComponentSorter
  {
    /*!When overloading the functaion call we can type the struct/class name
    then "()" to call the function. so to use ComponentSorter we call Component
    Sorter(left,right)
    bool operator()(GameComponent* left, GameComponent* right)const
    {
      return left->TypeId < right->TypeId;
    }
  };*/

  /* Binary Search in a sorted array of Components. If the component cannot be found NULL is returned.
  GameComponent* BinaryComponentSearch(ComponentArray& components, ComponentTypeId name)
  {
    unsigned begin = 0;
    unsigned end = components.size();

    while (begin < end)
    {
      unsigned mid = (begin + end) / 2;
      if (components[mid]->TypeId < name)
        begin = mid + 1;
      else
        end = mid;
    }

    //! still in index (not size + 1) and valid. Return component
    if ((begin < components.size()) && (components[begin]->TypeId == name))
      return components[begin];
    else
      return NULL;
  }*/
  