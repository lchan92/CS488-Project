#ifndef MESH_HPP
#define MESH_HPP

#include "assimp/usr/local/include/assimp/Importer.hpp"
#include <assimp/usr/local/include/assimp/scene.h>
#include <assimp/usr/local/include/assimp/postprocess.h>
#include <QtOpenGL>
#include "textures.hpp"

struct vertexData {
	QVector3D pos, normal;
	QVector2D texCoord;
};


class SubMesh {
public:
	SubMesh();
	virtual ~SubMesh();

	void bind();


	int mNumTriangles, mMaterialIndex;
	std::vector<float> mVertices, mNormals, mTexCoords;

	// OpenGL stuff
	#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    	QOpenGLBuffer mVBO, mNBO, mUVBO;
	#else 
    	QGLBuffer mVBO, mNBO, mUVBO;
	#endif
};


class Mesh {
public:
	Mesh();
	virtual ~Mesh();

	void load(const char* filePath, const char* defaultTexture);
	void init(const aiScene* scene, const char* defaultTexture);

	void bind();

	void setTransform();
	QMatrix4x4 getTransform();
	QMatrix4x4 getRotationTransform();

	void translate(QVector3D amount);
	void rotate(char axis, double angle);

	void resetPosition();


    std::vector<SubMesh*> subMeshes;
    Textures* mTextures;

    // BOUNDING BOX VARIABLES
    bool mInitializedBB;
    QVector4D mVertex1, mVertex2;
    QVector4D mInitPosition;
private:
	void initSubMesh(int index, const aiMesh* mesh);

	QMatrix4x4 mRotateTransform, mTransform;
};

#endif