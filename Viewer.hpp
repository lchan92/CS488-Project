#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

#include "scene.hpp"
#include "character.hpp"
#include "obstacle_map.hpp"
#include "sounds.hpp"
#include "textures.hpp"
#include "lights.hpp"
#include "cubemap.hpp"
#include "mesh.hpp"


class Viewer : public QGLWidget, protected QOpenGLFunctions {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void draw_mesh(Mesh* mesh);
    void draw_cube(QMatrix4x4 transformMatrix, std::vector<int> textureIDs, float reflectFactor);
    void draw_sphere(QMatrix4x4 transformMatrix);

    void setMapRoot(SceneNode* node);

    static Sounds* mSounds;
    static CubeMap* mCubeMap;
    static Textures* mTextures;
    static Lights* mLights;
    static Character* mPlayer;
    static ObstacleMap* mMap;

public slots:
    void updatePositions();

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);


    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    void drawSkyBox();
    void drawReflection();

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float theta, float x, float y, float z);
    void scaleWorld(float x, float y, float z);

    void cubeSetup();
    void sphereSetup();

    std::vector<GLfloat> mCubeVerts, mCubeNormals, mCubeUVCoords;
    std::vector<GLfloat> mSphereVerts, mSphereNormals;


    bool mLeftFlag, mRightFlag, mForwardFlag, mBackwardFlag;
    int mJumpCount;

    bool mDrawSkyBox, mDrawReflection, mDrawTopFaces;

    // mouse
    float mCameraHeight;
    QVector3D mCameraPosition;
    QMatrix4x4 mCameraTransformation, mCameraRotation;


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCubeBufferObject, mCubeNormalBufferObject, mCubeUVBufferObject;
    QOpenGLBuffer mSphereBufferObject, mSphereNormalBufferObject;
#else 
    QGLBuffer mCubeBufferObject, mCubeNormalBufferObject, mCubeUVBufferObject;
    QGLBuffer mSphereBufferObject, mSphereNormalBufferObject;
#endif


    int mShadingLocation;
    int mDiffuseLocation, mSpecularLocation, mShininessLocation;
    int mMvpMatrixLocation, mMvMatrixLocation, mNormalMatrixLocation, mMMatrixLocation;
    int mDrawSkyBoxLocation, mWorldCameraPosLocation, mDrawReflectionLocation, mReflectFactorLocation;
    int mLightPositionLocation, mLightColourLocation, mLightFalloffLocation;


    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QGLShaderProgram mProgram;
};

#endif