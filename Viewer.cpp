#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
#include <GL/glu.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent) 
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    // , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    // , mCircleBufferObject(QGLBuffer::VertexBuffer)
#endif
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);

    //FLAGS
    mForwardFlag = false;
    mBackwardFlag = false;
    mLeftFlag = false;
    mRightFlag = false;

    cubeSetup();
    sphereSetup();

    mPlayer = new Character();
    mMap = new ObstacleMap();

    // positions for all objects are updated every second
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePositions()));
    timer->start(20);
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
        return;
    }


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCubeBufferObject.create();
    mCubeBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else 
    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of 
     * QOpenGLBuffer.
     */
    uint vao;
     
    typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
    typedef void (APIENTRY *_glBindVertexArray) (GLuint);
     
    _glGenVertexArrays glGenVertexArrays;
    _glBindVertexArray glBindVertexArray;
     
    glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");
     
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);    

    mCubeBufferObject.create();
    mCubeBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCubeBufferObject.bind()) {
        std::cerr << "could not bind cube vertex buffer to the context." << std::endl;
        return;
    }
    mCubeBufferObject.allocate(&mCubeVerts[0], 36 * 3 * sizeof(float));

    if (!mSphereBufferObject.bind()) {
        std::cerr << "could not bind sphere vertex buffer to the context." << std::endl;
        return;
    }
    mSphereBufferObject.allocate(&mSphereVerts[0], 1600 * 6 * sizeof(float));



    mProgram.bind();
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);


    // variables to pass to shader
    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    mMvpNormalMatrixLocation = mProgram.uniformLocation("mvpNormalMatrix");
    mEyeMatrixLocation = mProgram.uniformLocation("eyeMatrix");

    mColorLocation = mProgram.uniformLocation("frag_color");

    mDiffuseLocation = mProgram.uniformLocation("Kd");
    mSpecularLocation = mProgram.uniformLocation("Ks");
    mShininessLocation = mProgram.uniformLocation("Shininess");
}




void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mPlayer->draw();
    mMap->draw();
}




void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;
    QVector3D modelPosition = mPlayer->getPosition();

    QMatrix4x4 cameraTransformation = mCameraTransformation;
    cameraTransformation.translate(0,10,20);

    QVector3D cameraPosition = (cameraTransformation * QVector4D(0,0,0,1)).toVector3D();
    QVector3D cameraUpDirection = QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, modelPosition, cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float theta, float x, float y, float z) {
    mTransformMatrix.rotate(theta, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red()/255.0, col.green()/255.0, col.blue()/255.0);
}




void Viewer::mousePressEvent(QMouseEvent *event) {
    std::cout << "mouse pressed" << std::endl;
}

void Viewer::mouseReleaseEvent(QMouseEvent *event) {
}

void Viewer::mouseMoveEvent(QMouseEvent *event) {
    QPoint center = mapToGlobal(rect().center());
    QPoint mouseMove = (event->globalPos() - center);
    if (mouseMove.isNull())
        return;

    float deltaX = mouseMove.x();
    mPlayer->rotateY(-deltaX/10);
    mCameraTransformation.rotate(-deltaX/10, 0, 1, 0);

    cursor().setPos(center);
}

void Viewer::keyPressEvent(QKeyEvent *event) {
    // std::cout << "key: " << event->key() << " val: " << qPrintable(event->text()) << std::endl;

    switch(event->key()) {
        case 87: {
            //w
            mForwardFlag = true;
            break;
        }
        case 65: {
            //a
            mLeftFlag = true;
            break;
        }
        case 83: {
            //s
            mBackwardFlag = true;
            break;
        }
        case 68: {
            //d
            mRightFlag = true;
            break;
        }
        case 32: {
            //space
            mPlayer->jump();
            break;
        }
        default:
            break;
    }
}

void Viewer::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case 87: {
            mForwardFlag = false;
            break;
        }
        case 65: {
            mLeftFlag = false;
            break;
        }
        case 83: {
            mBackwardFlag = false;
            break;
        }
        case 68: {
            mRightFlag = false;
            break;
        }
        default:
            break;
    }
}








// NODE SETUP

void Viewer::setModelRoot(SceneNode* node) {
    mPlayer->setRoot(node);
}

void Viewer::setMapRoot(SceneNode* node) {
    mMap->setRoot(node);
    mPlayer->setMapRoot(node);
}



// MOTION/PHYSICS
void Viewer::updatePositions() {
    mPlayer->applyGravity();

    if (mForwardFlag) {
        if (mPlayer->walkForward()) {
            mCameraTransformation.translate(0,0,-1);
        }
    } else if (mBackwardFlag) {
        if (mPlayer->walkBackward()) {
            mCameraTransformation.translate(0,0,1);
        }
    }

    if (mLeftFlag) {
        if (mPlayer->strafeLeft()) {
            mCameraTransformation.translate(-1,0,0);
        }
    } else if (mRightFlag) {
        if (mPlayer->strafeRight()) {
            mCameraTransformation.translate(1,0,0);
        }
    }

    update();
}


// DRAWING STUFF
void Viewer::cubeSetup() {
    float cubeData[] = {
        //  X     Y     Z
        0.0f, 0.0f, 0.0f, // front face
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, // bottom face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, // left face
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, // right face
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, // top face
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, // back face
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
    };

    for (int i = 0; i < 36*3; i++) {
        mCubeVerts.push_back(cubeData[i]);
    }
}

void Viewer::sphereSetup() {
    for(int i = 0; i < 40; i++)
    {
        double lat0 = M_PI * 2 * (-0.5 + (double) (i - 1) / 40);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * 2 * (-0.5 + (double) i / 40);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for(int j = 0; j < 40; j++)
        {
            double lng = 2 * M_PI * (double) (j - 1) / 40;
            double x = cos(lng);
            double y = sin(lng);

            mSphereVerts.push_back(x * zr0); //X
            mSphereVerts.push_back(y * zr0); //Y
            mSphereVerts.push_back(z0);      //Z

            mSphereVerts.push_back(x * zr1); //X
            mSphereVerts.push_back(y * zr1); //Y
            mSphereVerts.push_back(z1);      //Z
        }
    }
}



void Viewer::draw_cube(QMatrix4x4 transformMatrix) {
    mCubeBufferObject.bind();

    mProgram.bind();
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Viewer::draw_sphere(QMatrix4x4 transformMatrix) {
    mSphereBufferObject.bind();

    mProgram.bind();
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3200);
}


void Viewer::setMaterial(const Colour& kd, const Colour& ks, double shininess, QMatrix4x4 transformMatrix) {
    mProgram.setUniformValue(mDiffuseLocation, QVector3D(kd.R(), kd.G(), kd.B()));
    mProgram.setUniformValue(mSpecularLocation, QVector3D(ks.R(), ks.G(), ks.B()));
    mProgram.setUniformValue(mShininessLocation, (float) shininess);

    QMatrix4x4 mvpMatrix = getCameraMatrix() * transformMatrix;
    mProgram.setUniformValue(mMvpMatrixLocation, mvpMatrix);
    mProgram.setUniformValue(mMvpNormalMatrixLocation, mvpMatrix.normalMatrix());
    mProgram.setUniformValue(mEyeMatrixLocation, getCameraMatrix());
}
