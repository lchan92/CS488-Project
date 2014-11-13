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
    , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
#else 
    , mCircleBufferObject(QGLBuffer::VertexBuffer)
#endif
{
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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

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

    float circleData[120];

    double radius = width() < height() ? 
        (float)width() * 0.25 : (float)height() * 0.25;
        
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0.0;
    }


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

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

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCircleBufferObject.bind()) {
        std::cerr << "could not bind circle vertex buffer to the context." << std::endl;
        return;
    }
    mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

    mProgram.bind();
    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mShadingLocation = mProgram.uniformLocation("shading");

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
}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 20.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
  mProgram.setUniformValue(mColorLocation, col.red()/255.0, col.green()/255.0, col.blue()/255.0);
}

void Viewer::setMaterial(const Colour& kd, const Colour& ks, double shininess, QMatrix4x4 transformMatrix) {
    // mProgram.setUniformValue(mShadingLocation, !mPicking);
    mProgram.setUniformValue(mShadingLocation, true);

    mProgram.setUniformValue(mDiffuseLocation, QVector3D(kd.R(), kd.G(), kd.B()));
    mProgram.setUniformValue(mSpecularLocation, QVector3D(ks.R(), ks.G(), ks.B()));
    mProgram.setUniformValue(mShininessLocation, (float) shininess);

    QMatrix4x4 mvpMatrix = getCameraMatrix() * transformMatrix;
    mProgram.setUniformValue(mMvpMatrixLocation, mvpMatrix);
    mProgram.setUniformValue(mMvpNormalMatrixLocation, mvpMatrix.normalMatrix());
    mProgram.setUniformValue(mEyeMatrixLocation, getCameraMatrix());
}