#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include "primitive.hpp"
#include "material.hpp"

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(QMatrix4x4 transformMatrix = QMatrix4x4());
  virtual void setBoundaries(QMatrix4x4 transformMatrix = QMatrix4x4()) const;
  virtual void moveObjects(QMatrix4x4 transformMatrix = QMatrix4x4());

  virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, int direction);
  virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);

  const QMatrix4x4& get_transform() const { return m_trans; }
  const QMatrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const QMatrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.inverted();
  }

  void set_transform(const QMatrix4x4& m, const QMatrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const QVector3D& amount);
  void translate(const QVector3D& amount);
  
  void viewerRotate(float angle, float x, float y, float z);
  void viewerTranslate(QVector3D vec);

  void moveObject();

  // Transformations
  QMatrix4x4 m_translation, m_rotation; // TRANSLATION AND ROTATION MATRICES FOR THE PUPPET
  QMatrix4x4 m_trans; // INITIAL TRANSFORMATION MATRICES FOR POSITIONING THE PARTS OF THE PUPPET RELATIVE TO EACH OTHER
  QMatrix4x4 m_invtrans; // UNUSED

  // colour IDs
  static int freeColourID;

protected:
  int m_id;
  std::string m_name;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;

  SceneNode* m_parent;
  
  double mMaxTranslate[3], mCurrentTranslate[3], mDirection[3], mVelocity[3];
};





class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(QMatrix4x4 transformMatrix = QMatrix4x4());
  virtual void setBoundaries(QMatrix4x4 transformMatrix = QMatrix4x4()) const;
  virtual void moveObjects(QMatrix4x4 transformMatrix = QMatrix4x4());

  virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, int direction);
  virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };

protected:
  JointRange m_joint_x, m_joint_y;
  QMatrix4x4 m_jointRotation; // ROTATION FOR JUST THE JOINTS

  int m_angleX, m_angleY;
};






class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(QMatrix4x4 transformMatrix = QMatrix4x4());
  virtual void setBoundaries(QMatrix4x4 transformMatrix = QMatrix4x4()) const;
  virtual void moveObjects(QMatrix4x4 transformMatrix = QMatrix4x4());

  virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, int direction);
  virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);  

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
  }

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
