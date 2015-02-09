/******************************************************************************/
/*!
\file   Terrain2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Terrain2D.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Thermodynamics.h"
#include "random.hpp"
#include "PhysicsSystem.h"
#include "Collider2D.h"
#include "RigidBody2D.h"
#include "Thermodynamics.h"
#include "TDLib.h"
#include "Pipeline.h"

META_DEFINE( Framework::Terrain2D, Terrain2D )
{

}

namespace Framework
{
  using namespace Physics;
  static std::vector <float> lineVertices;
  static VAO* vao1;
  static VBO* vbo1;
  DefineComponentName (Terrain2D);

  ZilchDefineType (Terrain2D, CinderZilch)
  {
    type->HandleManager = ZilchManagerId (Zilch::PointerManager);
    ZilchBindConstructor (Terrain2D);
  }

  // Constructor
  Terrain2D::Terrain2D ()
  {}
  
  // Destructor
  Terrain2D::~Terrain2D ()
  {
    gameObject->Terrain2D = nullptr;
    delete vao1, vbo1;
    vao = nullptr;
    vbo = nullptr;
    tc = nullptr;
    vao1 = nullptr;
    vbo1 = nullptr;

    for (auto* i : edges)
    {
      delete i;
      i = nullptr;
    }

    edges.clear ();
  }


  void Terrain2D::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value;
    data->FindElement(data, "MapSize")->GetValue (&MapSize);

    data->FindElement (data, "BaseHeight")->GetValue (&BaseHeight);

    data->FindElement (data, "PeakHeight")->GetValue (&PeakHeight);

    data->FindElement (data, "Passes")->GetValue (&Passes);

    data->FindElement (data, "Waves")->GetValue (&Waves);

    //waterdepth defaults to 0 if not specified
    value = data->FindElement(data, "Water");
    if (value != nullptr)
      value->GetValue(&WaterDepth);

    //addcollider defaults to false if not specified
    value = data->FindElement(data, "AddCollider");
    if (value != nullptr)
      value->GetValue(&AddCollider);

    //color1 is mandatory
    data->FindElement(data, "Color1")->GetValue(&color1);
    //color2 defaults to color1 if not given by level file
    value = data->FindElement(data, "Color2");
    if (value != nullptr)
      value->GetValue(&color2);
    else
      color2 = color1;

    value = data->FindElement(data, "MapPreset");
    if (value)
      value->GetValue(&MapFile);
    else
      MapFile = "";

    value = data->FindElement(data, "Slope");
    if (value)
      value->GetValue(&Slope);
    else
      Slope = glm::vec2(1.f, 0.f);
  }

  void Terrain2D::Initialize ()
  {
    gameObject->Terrain2D = this;
    Generate_Height_Points ();
    Generate_Vertices ();
    Generate_Buffers ();

    if (AddCollider)
    {
      Generate_Edges ();
    }
  }

  void Terrain2D::Draw ()
  { 
    //glEnable (GL_BLEND);
    shader->Use ();
    vao->bindVAO ();
    shader->uniMat4 ("mvp", glm::value_ptr (gameObject->Transform->GetModelViewProjectionMatrix ()));
    shader->uni4f ("color1", color1.r, color1.g, color1.b, color1.a);
    shader->uni4f ("color2", color2.r, color2.g, color2.b, color2.a);

    glDrawArrays (GL_QUAD_STRIP, 0, vertices.size () / 3);

    vao->unbindVAO ();
    shader->Disable ();
    //OPENGL->ResetBlendMode ();
  }

  const float* Terrain2D::GetTerrain()
  {
    return tc->GetRockMap();
  }

  const float* Terrain2D::GetWater()
  {
    return tc->GetWaterMap();
  }

  int Terrain2D::GetWidth()
  { 
    return MapSize;
  }

  int Terrain2D::GetPeakHeight()
  {
    return tc->GetPeakHeight();
  }

  void Terrain2D::Generate_Height_Points ()
  {
    std::vector <float> heights;
    tc = new Procedural::TerrainCreator (MapSize, BaseHeight, Passes, Waves, PeakHeight, WaterDepth, MapFile, Slope);
    Procedural::TerrainCreator& t = *tc;
    float* Map = t.GetRockMap ();
    //float* W_Map = t.GetWaterMap();
    //float peak = t.GetPeakHeight () / t.Get_Width();

    {
      float offsetX = -1.0f;
      float offsetY = -1.0f;
      float nX = 2.f / (t.Get_Width () - 1);
      float nY = 2.f / (t.GetPeakHeight () - 1);
      float previousHeight = -1.f;

      for (int i = 0; i < t.Get_Width(); ++i)
      {
        //if (previousHeight != offsetY || i == t.Get_Width() - 1)
        //{
          height_points.push_back({ offsetX, offsetY });
          //previousHeight = offsetY;
        //}

        offsetY = (Map[i] * nY) / 2.0f;
        if (offsetY < 0)
          offsetY = 0.0f;

        offsetX += nX;
      }
    }

    //Set y scale to match terrain height
    float x_scale = this->gameObject->Transform->GetScale().x;
    float y_scale = t.GetPeakHeight();
    this->gameObject->Transform->Scale(x_scale, y_scale, 1.f);

    //if (AddCollider)
    //{
    //  //Allocate thermo grid
    //  int w_size = 2 * (int)this->gameObject->Transform->GetScale().x;
    //  THERMODYNAMICS->SetMapSize(w_size);

    //  int num_height_points = t.Get_Width();
    //  int size_factor = w_size / num_height_points;
    //  for (int x = 0; x < THERMODYNAMICS->MapSize.x; ++x)
    //  {
    //    ErrorIf(size_factor == 0, "Terrain size must be greater than terrain's heightpoints.");
    //    float cur_peak = Map[x / size_factor];
    //    THERMODYNAMICS->Terrain.Set (x, 0, STONE);
    //    THERMODYNAMICS->y_offset[x] = (int)cur_peak;
    //  } //for x
    //} //if main terrain
  }


  bool CalculateIntersectionPoint
    (
      double Ax, double Ay,
      double Bx, double By,
      double Cx, double Cy,
      double Dx, double Dy,
      double *X, double *Y
    )
  {

    double  distAB, theCos, theSin, newX, ABpos;

    //  Fail if either line segment is zero-length.
    if (Ax == Bx && Ay == By || Cx == Dx && Cy == Dy)
      return false;

    //  Fail if the segments share an end-point.
    if (Ax == Cx && Ay == Cy || Bx == Cx && By == Cy
      || Ax == Dx && Ay == Dy || Bx == Dx && By == Dy) {
      return true;
    }

    //  (1) Translate the system so that point A is on the origin.
    Bx -= Ax; By -= Ay;
    Cx -= Ax; Cy -= Ay;
    Dx -= Ax; Dy -= Ay;

    //  Discover the length of segment A-B.
    distAB = sqrt (Bx*Bx + By*By);

    //  (2) Rotate the system so that point B is on the positive X axis.
    theCos = Bx / distAB;
    theSin = By / distAB;
    newX = Cx*theCos + Cy*theSin;
    Cy = Cy*theCos - Cx*theSin; Cx = newX;
    newX = Dx*theCos + Dy*theSin;
    Dy = Dy*theCos - Dx*theSin; Dx = newX;

    //  Fail if segment C-D doesn't cross line A-B.
    if (Cy < 0. && Dy < 0. || Cy >= 0. && Dy >= 0.)
      return false;

    //  (3) Discover the position of the intersection point along line A-B.
    ABpos = Dx + (Cx - Dx)*Dy / (Dy - Cy);

    //  Fail if segment C-D crosses line A-B outside of segment A-B.
    if (ABpos<0. || ABpos>distAB)
      return false;

    //  (4) Apply the discovered position to line A-B in the original coordinate system.
    *X = Ax + ABpos*theCos;
    *Y = Ay + ABpos*theSin;

    //  Success.
    return true;
  }


  void Terrain2D::Generate_Edges ()
  {
    Vector2* p = new Vector2 [4];
    float y = 0.0f;
    // Edges for Line Colliders
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      PolygonCollider2D* poly = new PolygonCollider2D ();
      edges.push_back (poly);
      glm::dvec2 center;
      glm::vec2 p0 = (glm::mat2)gameObject->Transform->GetModelMatrix () * glm::vec2 (height_points [i].x, y);
      glm::vec2 p1 = (glm::mat2)gameObject->Transform->GetModelMatrix () * glm::vec2 (height_points [i + 1].x, y);
      glm::vec2 p2 = (glm::mat2)gameObject->Transform->GetModelMatrix () * glm::vec2 (height_points [i + 1].x, height_points [i + 1].y);
      glm::vec2 p3 = (glm::mat2)gameObject->Transform->GetModelMatrix () * glm::vec2 (height_points [i].x, height_points [i].y);

      p [0] = Vector2 (p2.x, p2.y);
      p [1] = Vector2 (p3.x, p3.y);
      p [2] = Vector2 (p0.x, p0.y);
      p [3] = Vector2 (p1.x, p1.y);

      ////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////
      //// EDGES FOR THERMODYNAMICS
      ////////////////////////////////////////////////////////////////////////////
      //Physics::ThermodynamicsSystem::TerrainPoints.push_back (p2);
      //Physics::ThermodynamicsSystem::TerrainPoints.push_back (p3);
      //Physics::ThermodynamicsSystem::TerrainPoints.push_back (p0);
      //Physics::ThermodynamicsSystem::TerrainPoints.push_back (p1);
      ////////////////////////////////////////////////////////////////////////////
      p [2].y = p [3].y = y;

      CalculateIntersectionPoint
        (
        p0.x, p0.y,
        p2.x, p2.y,
        p1.x, p1.y,
        p3.x, p3.y,
        &center.x, &center.y
        );

      poly->Set (p, 4);
      RigidBody2D* b = PHYSICS->Add (poly, gameObject->Transform->GetPosition().x + (float)(center.x),
        (gameObject->Transform->GetPosition ().y + (float)(center.y)));
      b->SetOrient (0);
      b->SetStatic ();
      b->restitution = 0.5f;

      b->staticFriction = 1.0f;
      b->dynamicFriction = 1.0f;
    }
    delete [] p;
    p = nullptr;
  }

  void Terrain2D::Generate_Vertices ()
  {
    float y = -BaseHeight;
    // Vertices
    for (unsigned i = 0; i < height_points.size () - 1; ++i)
    {
      // Triangle 1

      vertices.push_back (height_points [i].x);
      vertices.push_back (height_points [i].y);
      vertices.push_back (0);

      vertices.push_back (height_points [i].x);
      vertices.push_back (y);
      vertices.push_back (0);
    }
  }

  void Terrain2D::Generate_Buffers ()
  {
    vao = new VAO ();
    shader = Resources::RS->Get_Shader ("Terrain");
    shader->Use ();

    vbo = new VBO (vertices.size () * sizeof (float), vertices.data ());
    GLuint posAttrib = shader->attribLocation ("position");
    shader->enableVertexAttribArray (posAttrib);
    shader->vertexAttribPtr (posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), 0);

    vao->unbindVAO ();
  }

}