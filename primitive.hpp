#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <QMatrix4x4>
#include <QVector4D>

class Primitive {
public:
	virtual ~Primitive();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const = 0;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction);
  	virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);

    static double EPSILON;
	double mTranslateX, mTranslateY, mTranslateZ; // maximum translate from original position
};

class Block : public Primitive {
public:
	Block(int type, double translateX, double translateY, double translateZ);
	virtual ~Block();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction);
  	virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);

private:
	bool intersectsFront(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);
	bool intersectsBack(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);
	bool intersectsRight(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);
	bool intersectsLeft(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);
	bool intersectsTop(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);
	bool intersectsBottom(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity);

	bool betweenLeftRight(double x, double left, double right);
	bool betweenTopBottom(double y, double top, double bottom);
	bool betweenFrontBack(double z, double front, double back);

	bool aboveAndUnderBox(QVector4D p1, QVector4D p2);
	bool leftAndRightOfBox(QVector4D p1, QVector4D p2);
	bool inFrontAndBehindBox(QVector4D p1, QVector4D p2);

	double mReflectFactor, mTransparency;
	QVector4D mVertex1, mVertex2;
	std::vector<int> mTextureIDs;

	double oldVelocityX, oldVelocityY, oldVelocityZ;
};

class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual void walk_gl(QMatrix4x4 transformMatrix) const;
	virtual void setBoundaries(QMatrix4x4 transformMatrix);

	virtual bool faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction);
  	virtual bool isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor);
};

#endif
