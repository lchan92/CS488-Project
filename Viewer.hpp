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


class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void draw_cube(QMatrix4x4 transformMatrix);
    void draw_sphere(QMatrix4x4 transformMatrix);

    void setMaterial(const Colour& kd, const Colour& ks, double shininess, QMatrix4x4 transformMatrix);

    void setModelRoot(SceneNode* node);
    void setMapRoot(SceneNode* node);

    static Sounds* mSounds;
    static Textures* mTextures;

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

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float theta, float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void set_colour(const QColor& col);

    void cubeSetup();
    void sphereSetup();

    std::vector<GLfloat> mCubeVerts, mCubeNormals, mCubeUVCoords;
    std::vector<GLfloat> mSphereVerts, mSphereNormals;

    Character *mPlayer;
    ObstacleMap *mMap;

    bool mLeftFlag, mRightFlag, mForwardFlag, mBackwardFlag;
    int mJumpCount;

    // mouse
    QMatrix4x4 mCameraTransformation;


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCubeBufferObject, mCubeNormalBufferObject, mCubeUVBufferObject;
    QOpenGLBuffer mSphereBufferObject, mSphereNormalBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mCubeBufferObject, mCubeNormalBufferObject, mCubeUVBufferObject;
    QGLBuffer mSphereBufferObject, mSphereNormalBufferObject;
#endif


    int mShadingLocation;
    int mDiffuseLocation, mSpecularLocation, mShininessLocation;
    // int mMvpMatrixLocation, mMvpNormalMatrixLocation, mEyeMatrixLocation;
    int mMvpMatrixLocation, mMvMatrixLocation, mNormalMatrixLocation;
    int mColorLocation;


    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mTransformMatrix;
    QGLShaderProgram mProgram;
};

#endif