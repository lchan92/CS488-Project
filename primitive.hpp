#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

// #include "algebra.hpp"
#include <QMatrix4x4>

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(QMatrix4x4 transformMatrix) const = 0;
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual void walk_gl(QMatrix4x4 transformMatrix) const;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual void walk_gl(QMatrix4x4 transformMatrix) const;
};

#endif
