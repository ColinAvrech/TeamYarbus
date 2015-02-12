/******************************************************************************/
/*!
\file   Tree2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TREE_3D_H
#define _TREE_3D_H

#include <vector>
#include "IGraphicsObject.h"

#include "MetaCreator.h"

namespace Framework
{
  class VertexArrayObject;
  class VertexBufferObject;

  enum Tree3D_Type
  {
    TREE_0_3D,
    TREE_1_3D,
    OPEN_3D,
    INVALID_3D
  };


  class Tree3D : public IGraphicsObject
  {
  public:
	  META_DECLARE( Tree3D );

  public:
  Tree3D ();
  virtual ~Tree3D ();
  
  virtual void Serialize (Serializer::DataNode* data);
  virtual void Initialize ();
  virtual void Draw ();
  virtual bool Draw_Last() { return false; }

  //std::vector <glm::vec2>& Get_Edges ()
  //{
  //  return edges;
  //}

  void Set(glm::vec4& _color, Tree3D_Type _type);
  const static std::string Name;
  private:

    // Mesh Data
    VAO* vao;
    VBO* vbo;
    EBO* ebo;
    GLuint elementbuffer;

    unsigned vertex_count;
    unsigned poly_count;
    std::vector <float> treeMesh;
    std::vector<unsigned> indices;
    std::vector <glm::vec2> edges;
    Tree3D_Type type;
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

    void Make_Tree0(glm::vec3 &pos, float length1, glm::vec3 &angle1, int depth, float rad, int previous, unsigned parent = 0);
    void Make_Tree1(glm::vec3 &pos, float length1, glm::vec3 &angle1, int depth, float rad, unsigned parent = 0);
    void Make_Tree_Fan(glm::vec3 &pos, float length1, glm::vec2 &angle1, int depth, float rad, unsigned parent = 0);

    //Helper
    //Pine stuff
    void Make_Pine_Branch(float x1, float y1, float length1, float angle1, int depth, int curve, float rad, unsigned parent = 0);
    //Grass stuff
    //void Make_Grass_Stalk(float x1, float y1, float length1, float angle1, int depth, int curve, unsigned parent = 0);
    void Make_Grass_Blade(float x1, float y1, float length, float angle, int depth, float width, unsigned parent = 0);
    //void Make_Stalk_Head(float x1, float y1, float length, float angle);
    //call for adding a line
    unsigned Add_Branch(glm::vec3 &pos1, glm::vec3 &pos2, float rad, unsigned parent = 0, bool branch_tip = false);
    void Generate_Buffers ();
    void CalculateBounds();
    bool InViewport();
  };
}



#endif