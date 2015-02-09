#pragma once
namespace Framework
{
  class Material
  {
    public:
      Material();
      ~Material();

      void Serialize(Serializer::DataNode* data);

      float density;  // density
      float staticFriction;
      float dynamicFriction;
      float restitution;
  };
}
