#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <QMatrix4x4>
#include <QVector4D>

class Primitive {
public:
	virtual ~Primitive();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const = 0;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction);

    static double EPSILON;
};

class Block : public Primitive {
public:
	Block();
	virtual ~Block();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction);


private:
	bool intersectsFront(QVector4D p1, QVector4D p2, double* velocity);
	bool intersectsBack(QVector4D p1, QVector4D p2, double* velocity);
	bool intersectsRight(QVector4D p1, QVector4D p2, double* velocity);
	bool intersectsLeft(QVector4D p1, QVector4D p2, double* velocity);
	bool intersectsTop(QVector4D p1, QVector4D p2, double* velocity);
	bool intersectsBottom(QVector4D p1, QVector4D p2, double* velocity);

	bool betweenLeftRight(double x, double left, double right);
	bool betweenTopBottom(double y, double top, double bottom);
	bool betweenFrontBack(double z, double front, double back);

	QVector4D mVertex1, mVertex2;
	std::vector<int> mTextureIDs;
};

class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction);
};

#endif
