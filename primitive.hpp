#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <QMatrix4x4>
#include <QVector4D>

class Primitive {
public:
	virtual ~Primitive();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const = 0;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);
	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2);
};

class Block : public Primitive {
public:
	virtual ~Block();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);
	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2);
private:
	bool intersectsFace(QVector4D a1, QVector4D a2, QVector4D b1, QVector4D b2);

	QVector4D mVertex1, mVertex2;
};

class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);
	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2);
};

#endif
