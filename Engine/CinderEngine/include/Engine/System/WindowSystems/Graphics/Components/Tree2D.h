/******************************************************************************/
/*!
\file   Tree2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TREE_2D_H
#define _TREE_2D_H

#include "GraphicsCommon.h"
#include "IGraphicsObject.h"

namespace Framework
{
  enum Tree_Type
  {
    TREE_0,
    TREE_1,
    TREE_2,
    TREE_3,
    TREE_4,
    GRASS,
  };


  class Tree2D : public IGraphicsObject
  {
  public:
  Tree2D ();
  virtual ~Tree2D ();
  
  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void Draw ();


  const static std::string Name;
  private:

    // Mesh Data
    VAO* vao;
    VBO* vbo;
    std::vector <float> treeMesh;
    Tree_Type type;

    // Resources
    glm::vec4 color;

    void Make_Tree0(float x1, float y1, float length1, float angle1, int depth);
    void Make_Tree1(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount);
    void Make_Tree2(float x1, float y1, float length, float angle, int depth);
    void Make_Tree3(float x1, float y1, float length1, float angle1, int depth);
    void Make_Tree4(float x1, float y1, float length, float angle, int depth);
    void Make_Grass(float x1, float y1, float length1);

    //Helper
    //Pine stuff
    void Make_Pine_Branch(float x1, float y1, float length1, float angle1, int depth, int curve);
    //Grass stuff
    void Make_Grass_Stalk(float x1, float y1, float length1, float angle1, int depth, int curve);
    void Make_Grass_Blade(float x1, float y1, float length, float angle);
    void Make_Stalk_Head(float x1, float y1, float length, float angle);
    //call for adding a line
    void Add_Branch(float x1, float y1, float x2, float y2);
  };  
}

#endif