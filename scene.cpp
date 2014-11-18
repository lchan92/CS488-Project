#include "scene.hpp"
#include <iostream>

int SceneNode::freeColourID = 0;

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
  m_id = freeColourID;
  freeColourID++;
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(QMatrix4x4 transformMatrix) const
{
    std::cout << "scene node" << std::endl;
    std::list<SceneNode*>::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->walk_gl(transformMatrix * m_translation * m_rotation * m_trans);
    }
}

void SceneNode::rotate(char axis, double angle)
{
  switch(axis) {
    case 'x': {
      m_trans.rotate(angle,1,0,0);
      break;
    }
    case 'y': {
      m_trans.rotate(angle,0,1,0);
      break;
    }
    case 'z': {
      m_trans.rotate(angle,0,0,1);
      break;
    }
  }
}

void SceneNode::scale(const QVector3D& amount)
{
  m_trans.scale(amount.x(), amount.y(), amount.z());
}

void SceneNode::translate(const QVector3D& amount)
{
  m_trans.translate(amount.x(), amount.y(), amount.z());
}


void SceneNode::viewerRotate(float angle, float x, float y, float z) {
  QMatrix4x4 temp;
  temp.rotate(angle,x,y,z);
  m_rotation = temp * m_rotation;
}

void SceneNode::viewerTranslate(QVector3D vec) {
  m_translation.translate(vec.x(), vec.y(), vec.z());
}









JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(QMatrix4x4 transformMatrix) const
{
    std::cout << "joint node" << std::endl;
    std::list<SceneNode*>::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->walk_gl(transformMatrix * m_translation * m_rotation * m_trans * m_jointRotation);
    }
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;

  m_angleX = m_joint_x.init;
  m_jointRotation.rotate(m_angleX, 1, 0, 0);
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;

  m_angleY = m_joint_y.init;
  m_jointRotation.rotate(m_angleY, 0, 1, 0);
}












GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(QMatrix4x4 transformMatrix) const
{
  std::cout << "geo node" << std::endl;
  // m_material->apply_gl(transformMatrix * m_trans);
  // m_primitive->walk_gl(transformMatrix * m_trans);
}