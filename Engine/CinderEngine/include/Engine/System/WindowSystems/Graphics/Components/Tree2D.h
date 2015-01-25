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

#include <vector>
#include "IGraphicsObject.h"

namespace Framework
{
  class VertexArrayObject;
  class VertexBufferObject;
  class FractalGenerator;

  enum Tree_Type
  {
    TREE_0,
    TREE_1,
    TREE_2,
    TREE_3,
    TREE_4,
	  TREE_LONG,
    TREE_SHORT_GRASS,
    TREE_TALL_GRASS,
    TREE_5,
    OPEN,  //Keep this last
    INVALID
  };


  class Tree2D : public IGraphicsObject
  {
  public:
  Tree2D ();
  virtual ~Tree2D ();
  
  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void Draw ();
  virtual bool Draw_Last() { return false; }

  void Set(glm::vec4& _color, Tree_Type _type);
  const static std::string Name;
  private:

    // Mesh Data
    VAO* vao;
    VBO* vbo;
    unsigned vertex_count;
    std::vector <float> treeMesh;
    Tree_Type type;
    // Resources
    glm::vec4 color;
    float base_radius = 0.05f;
    float decay_rate = 0.5f;
    int segments = 10;
    float length;

    //object bounds
    float bound_l;
    float bound_r;
    float bound_t;
    float bound_b;

    void Make_Tree0(float x1, float y1, float length1, float angle1, int depth, float rad, unsigned parent = 0);
    void Make_Tree1(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount);
    void Make_Tree2(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent = 0);
    void Make_Tree3(float x1, float y1, float length1, float angle1, int depth, float rad, unsigned parent = 0);
    void Make_Tree4(float x1, float y1, float length, float angle, int depth, float rad, unsigned parent = 0);
	void Make_TreeLong(float x1, float y1, float x2, float y2, float angle, int depth, int branchCount);
    void Make_Grass(float x1, float y1, float length1, int depth);

    //Helper
    //Pine stuff
    void Make_Pine_Branch(float x1, float y1, float length1, float angle1, int depth, int curve, float rad, unsigned parent = 0);
    //Grass stuff
    //void Make_Grass_Stalk(float x1, float y1, float length1, float angle1, int depth, int curve, unsigned parent = 0);
    void Make_Grass_Blade(float x1, float y1, float length, float angle, int depth, float width, unsigned parent = 0);
    //void Make_Stalk_Head(float x1, float y1, float length, float angle);
    //call for adding a line
    unsigned Add_Branch(float x1, float y1, float x2, float y2, float rad, unsigned parent = 0);
    void Generate_Buffers ();
    void CalculateBounds();
    bool InViewport();
  };
}

#endif