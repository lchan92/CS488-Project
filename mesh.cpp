#include "mesh.hpp"
#include <iostream>



SubMesh::SubMesh() {
	mNumTriangles = 0;
}

SubMesh::~SubMesh() {}

void SubMesh::bind() {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
	mVBO.create();
	mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

	mNBO.create();
	mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

	mUVBO.create();
	mUVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else
	mVBO.create();
	mVBO.setUsagePattern(QGLBuffer::StaticDraw);

	mNBO.create();
	mNBO.setUsagePattern(QGLBuffer::StaticDraw);

	mUVBO.create();
	mUVBO.setUsagePattern(QGLBuffer::StaticDraw);
#endif

	if (!mVBO.bind()) {
		std::cerr << "could not bind mesh vertex buffer" << std::endl;
	}
	mVBO.allocate(&mVertices[0], mVertices.size() * sizeof(float));

	if (!mNBO.bind()) {
		std::cerr << "could not bind mesh vertex buffer" << std::endl;
	}
	mNBO.allocate(&mNormals[0], mNormals.size() * sizeof(float));

	if (!mUVBO.bind()) {
		std::cerr << "could not bind mesh vertex buffer" << std::endl;
	}
	mUVBO.allocate(&mTexCoords[0], mTexCoords.size() * sizeof(float));
}





Mesh::Mesh() {
	mTextures = new Textures();
	mInitializedBB = false;

	mVertex1 = QVector4D(0,0,0,1);
	mVertex2 = QVector4D(0,0,0,1);
}

Mesh::~Mesh() {}

void Mesh::load(const char* filePath, const char* defaultTexture) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | 
														aiProcess_GenSmoothNormals | 
														aiProcess_FlipUVs);

	if (!scene) {
		std::cout << "Error reading from file: " << importer.GetErrorString() << std::endl;
	} else {
		init(scene, defaultTexture);
	}
}

void Mesh::init(const aiScene* scene, const char* defaultTexture) {
	// load materials
	for (int i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string imagePath = path.data;
				imagePath = path.data + imagePath.substr(2, imagePath.length()-2);

				mTextures->loadImage(imagePath.c_str(), "JPG");
			}
		} else {
			//load a default texture
			mTextures->loadImage(defaultTexture, "JPG");
		}
	}

	// load meshes
	for (int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		initSubMesh(i, mesh);
	}

	setTransform();
}



void Mesh::initSubMesh(int index, const aiMesh* mesh) {
	std::vector<vertexData> verticesData;
	std::vector<float> indices;

	// BOUNDING BOX VARIABES
	float minX, minY, minZ;
	float maxX, maxY, maxZ;



	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// load vertices, normals, tex coords
	for (int i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D* vertex = &(mesh->mVertices[i]);
		const aiVector3D* normal = &(mesh->mNormals[i]);
		const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

		vertexData v;
		v.pos = QVector3D(vertex->x, vertex->y, vertex->z);
		v.normal = QVector3D(normal->x, normal->y, normal->z);
		v.texCoord = QVector2D(texCoord->x, texCoord->y);

		verticesData.push_back(v);

		// DETERMINE BOUNDING BOX STUFF
		if (!mInitializedBB) {
			minX = v.pos.x();
			maxX = minX;

			minY = v.pos.y();
			maxY = minY;

			minZ = v.pos.z();
			maxZ = minZ;

			mInitializedBB = true;
		} else {
			if (v.pos.x() < minX) minX = v.pos.x();
			if (v.pos.x() > maxX) maxX = v.pos.x();

			if (v.pos.y() < minY) minY = v.pos.y();
			if (v.pos.y() > maxY) maxY = v.pos.y();

			if (v.pos.z() < minZ) minZ = v.pos.z();
			if (v.pos.z() > maxZ) maxZ = v.pos.z();
		}
	}


	// BOUNDING BOX STUFF
	if (minX < mVertex1.x()) mVertex1.setX(minX);
	if (maxX > mVertex2.x()) mVertex2.setX(maxX);

	if (minY < mVertex1.y()) mVertex1.setY(minY);
	if (maxY > mVertex2.y()) mVertex2.setY(maxY);

	if (minZ < mVertex2.z()) mVertex2.setZ(minZ);
	if (maxZ > mVertex1.z()) mVertex1.setZ(maxZ);


	// load faces
	for (int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}


	// grab vertex at index and push into array of vertices for vbo
	SubMesh* subMesh = new SubMesh();

	for (int i = 0; i < indices.size(); i++) {
		int index = indices[i];

		vertexData v = verticesData[index];

		subMesh->mVertices.push_back(v.pos.x());
		subMesh->mVertices.push_back(v.pos.y());
		subMesh->mVertices.push_back(v.pos.z());

		subMesh->mNormals.push_back(v.normal.x());
		subMesh->mNormals.push_back(v.normal.y());
		subMesh->mNormals.push_back(v.normal.z());

		subMesh->mTexCoords.push_back(v.texCoord.x());
		subMesh->mTexCoords.push_back(v.texCoord.y());

		subMesh->mMaterialIndex = mesh->mMaterialIndex;
		subMesh->mNumTriangles++;
	}

	subMeshes.push_back(subMesh);
}



void Mesh::bind() {
	for (int i = 0; i < subMeshes.size(); i++) {
		subMeshes[i]->bind();
	}
}

void Mesh::setTransform() {
	mTransform.setToIdentity();
	mRotateTransform.setToIdentity();

	float midX = mVertex1.x() + (mVertex2.x() - mVertex1.x())/2;
	float midY = mVertex1.y() + (mVertex2.y() - mVertex1.y())/2;
	float midZ = mVertex2.z() + (mVertex1.z() - mVertex2.z())/2;
	mInitPosition = QVector4D(midX, midY, midZ, 1);

	translate(QVector3D(0,0,7));
	mRotateTransform.rotate(180, QVector3D(0,1,0));
}

QMatrix4x4 Mesh::getTransform() {
	return mTransform;
}

QMatrix4x4 Mesh::getRotationTransform() {
	return mRotateTransform;
}


void Mesh::translate(QVector3D amount) {
	mTransform.translate(amount.x(), amount.y(), amount.z());
}

void Mesh::rotate(char axis, double angle) {
  switch(axis) {
    case 'x': {
      mTransform.rotate(angle,1,0,0);
      break;
    }
    case 'y': {
      mTransform.rotate(angle,0,1,0);
      break;
    }
    case 'z': {
      mTransform.rotate(angle,0,0,1);
      break;
    }
  }
}

void Mesh::resetPosition() {
	setTransform();

	QVector4D pos = mTransform * mRotateTransform * mInitPosition;
	std::cout << "pos reset: " << pos.x() << "," << pos.y() << "," << pos.z() << std::endl;
}