#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>
#include <GL/glu.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif


Sounds* Viewer::mSounds = NULL;
Textures* Viewer::mTextures = NULL;
Lights* Viewer::mLights = NULL;
CubeMap* Viewer::mCubeMap = NULL;
Character* Viewer::mPlayer = NULL;
Trophy* Viewer::mTrophy = NULL;
ObstacleMap* Viewer::mMap = NULL;


Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent) 
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);


    //FLAGS
    mForwardFlag = false;
    mBackwardFlag = false;
    mLeftFlag = false;
    mRightFlag = false;
    mDrawStencil = false;
    mDrawReflection = false;

    mCameraHeight = 10.0f;

    cubeSetup();
    sphereSetup();


    mPlayer = new Character();
    mTrophy = new Trophy();
    mMap = new ObstacleMap();
    mSounds = new Sounds();
    mCubeMap = new CubeMap();
    mTextures = new Textures();
    mLights = new Lights();

    // positions for all objects are updated every second
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePositions()));
    timer->start(20);


}

Viewer::~Viewer() {}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::initializeGL() {
    initializeOpenGLFunctions();

    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glEnable(GL_DEPTH_TEST);


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
    mCubeBufferObject.create();
    mCubeBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    mCubeNormalBufferObject.create();
    mCubeNormalBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    mCubeUVBufferObject.create();
    mCubeUVBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);

    mSphereNormalBufferObject.create();
    mSphereNormalBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
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

    mCubeNormalBufferObject.create();
    mCubeNormalBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

    mCubeUVBufferObject.create();
    mCubeUVBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

    mSphereBufferObject.create();
    mSphereBufferObject.setUsagePattern(QGLBuffer::StaticDraw);

    mSphereNormalBufferObject.create();
    mSphereNormalBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCubeBufferObject.bind()) {
        std::cerr << "could not bind cube vertex buffer to the context." << std::endl;
        return;
    }
    mCubeBufferObject.allocate(&mCubeVerts[0], 36 * 3 * sizeof(float));

    if (!mCubeNormalBufferObject.bind()) {
        std::cerr << "could not bind cube normal buffer to the context." << std::endl;
        return;
    }
    mCubeNormalBufferObject.allocate(&mCubeNormals[0], 36 * 3 * sizeof(float));

    if (!mCubeUVBufferObject.bind()) {
        std::cerr << "could not bind cube uv buffer to the context." << std::endl;
        return;
    }
    mCubeUVBufferObject.allocate(&mCubeUVCoords[0], 36 * 2 * sizeof(float));



    if (!mSphereNormalBufferObject.bind()) {
        std::cerr << "could not bind sphere vertex buffer to the context." << std::endl;
        return;
    }
    mSphereNormalBufferObject.allocate(&mSphereNormals[0], 1600 * 6 * sizeof(float));

    if (!mSphereBufferObject.bind()) {
        std::cerr << "could not bind sphere vertex buffer to the context." << std::endl;
        return;
    }
    mSphereBufferObject.allocate(&mSphereVerts[0], 1600 * 6 * sizeof(float));

    // load textures
    mCubeMap->load();
    mTextures->load();

    // load and bind meshes
    mPlayer->bind();
    mTrophy->bind();




    // variables to pass to shader
    mMMatrixLocation = mProgram.uniformLocation("M");
    mMvMatrixLocation = mProgram.uniformLocation("MV");
    mNormalMatrixLocation = mProgram.uniformLocation("N");
    mMvpMatrixLocation = mProgram.uniformLocation("MVP");

    mDiffuseLocation = mProgram.uniformLocation("Kd");
    mSpecularLocation = mProgram.uniformLocation("Ks");
    mShininessLocation = mProgram.uniformLocation("Shininess");

    // CUBEMAP
    mDrawSkyBoxLocation = mProgram.uniformLocation("drawSkyBox");
    mWorldCameraPosLocation = mProgram.uniformLocation("worldCameraPos");

    // LIGHTS
    mLightPositionLocation = mProgram.uniformLocation("lightPositions");
    mLightColourLocation = mProgram.uniformLocation("lightColours");
    mLightFalloffLocation = mProgram.uniformLocation("lightFalloffs");

    // REFLECTIONS
    mDrawReflectionLocation = mProgram.uniformLocation("drawReflection");
    mReflectFactorLocation = mProgram.uniformLocation("reflectFactor");

    // TRANSPARENCY
    mTransparencyLocation = mProgram.uniformLocation("transparency");
}








void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    mDrawSkyBox = true;
    drawSkyBox();
    mDrawSkyBox = false;

    mPlayer->draw();
    mTrophy->draw();

     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mMap->draw();
    glDisable(GL_BLEND);

    // REFLECTIONS
    drawReflection();
}

void Viewer::drawSkyBox() {
    QMatrix4x4 transformMatrix;
    transformMatrix.scale(500,500,500);
    draw_sphere(transformMatrix);
}

void Viewer::drawReflection() {
    glDisable(GL_DEPTH_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

    mDrawStencil = true;
    mMap->draw();
    mDrawStencil = false;

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glStencilFunc(GL_EQUAL, 1, 0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mDrawReflection = true;

    mDrawCharacterReflection = true;
    mPlayer->draw();
    mDrawCharacterReflection = false;
    
    mDrawTrophyReflection = true;
    mTrophy->draw();
    mDrawTrophyReflection = false;

    mDrawReflection = false;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glDisable(GL_STENCIL_TEST);
}









void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
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





void Viewer::mousePressEvent(QMouseEvent *event) {}

void Viewer::mouseReleaseEvent(QMouseEvent *event) {}

void Viewer::mouseMoveEvent(QMouseEvent *event) {
    QPoint center = mapToGlobal(rect().center());
    QPoint mouseMove = (event->globalPos() - center);
    if (mouseMove.isNull())
        return;

    float deltaX = mouseMove.x();
    float deltaY = mouseMove.y();
    // mPlayer->rotateY(-deltaX/10);


    mCameraRotation.rotate(-deltaX/10, 0, 1, 0);

    mCameraHeight += deltaY/10;
    if (mCameraHeight > 15) mCameraHeight = 15;
    else if (mCameraHeight < -10) mCameraHeight = -10;

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
void Viewer::setMapRoot(SceneNode* node) {
    mMap->setRoot(node);
    mPlayer->setMapRoot(node);
    mTrophy->setMapRoot(node);
}



// MOTION/PHYSICS
void Viewer::updatePositions() {
    mMap->moveObjects();

    QVector3D velocity;


    // CHECK IF EACH BOXES SIDES MOVE INTO CHARACTER WHILE NOT PRESSING BUTTONS
    mPlayer->checkFrontCollisions(&velocity);
    if (mPlayer->isAlive())
        mCameraTransformation.translate(velocity);
    mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);

    velocity = QVector3D(0,0,0);
    mPlayer->checkBackCollisions(&velocity);
    if (mPlayer->isAlive())
        mCameraTransformation.translate(velocity);
    mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);

    velocity = QVector3D(0,0,0);
    mPlayer->checkLeftCollisions(&velocity);
    if (mPlayer->isAlive())
        mCameraTransformation.translate(velocity);
    mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);

    velocity = QVector3D(0,0,0);
    mPlayer->checkRightCollisions(&velocity);
    if (mPlayer->isAlive())
        mCameraTransformation.translate(velocity);
    mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);



    // GRAVITY
    velocity = QVector3D(0,0,0);
    bool onSurface = mPlayer->applyGravity(&velocity);
    
    if (mPlayer->isAlive())
        mCameraTransformation.translate(velocity);
    mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);


    // MOVEMENT
    if (mForwardFlag) {
        velocity = QVector3D(0,0,-0.5);
        mPlayer->walkForward(&velocity);

        if (mPlayer->isAlive()) 
            mCameraTransformation.translate(velocity);

        mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);
        
        if (onSurface) {
            Viewer::mSounds->playFootsteps();
        }
    } else if (mBackwardFlag) {
        velocity = QVector3D(0,0,0.5);
        mPlayer->walkBackward(&velocity);

        if (mPlayer->isAlive()) 
            mCameraTransformation.translate(velocity);

        mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);;
        
        if (onSurface) {
            Viewer::mSounds->playFootsteps();
        }
    }

    if (mLeftFlag) {
        velocity = QVector3D(-0.2,0,0);
        mPlayer->strafeLeft(&velocity);

        if (mPlayer->isAlive())
            mCameraTransformation.translate(velocity);
        
        mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);

        if (onSurface) {
            Viewer::mSounds->playFootsteps();
        }
    } else if (mRightFlag) {
        velocity = QVector3D(0.2,0,0);
        mPlayer->strafeRight(&velocity);
        
        if (mPlayer->isAlive()) 
            mCameraTransformation.translate(velocity);
        
        mLights->mPositions[0] += QVector4D(velocity.x(),velocity.y(),velocity.z(),0);

        if (onSurface) {
            Viewer::mSounds->playFootsteps();
        }
    }

    if (!(mForwardFlag || mBackwardFlag || mLeftFlag || mRightFlag) || !onSurface) {
        Viewer::mSounds->stopFootsteps();
    }

    update();
}




// DRAWING STUFF
void Viewer::cubeSetup() {
    float cubeData[] = {
        //  X     Y     Z
        0.0f, 0.0f, 0.0f, // back face
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
        0.0f, 0.0f, 1.0f, // front face
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
    };

    for (int i = 0; i < 36*3; i++) {
        mCubeVerts.push_back(cubeData[i]);
    }

    QVector3D normals[6];
    normals[0] = QVector3D(0,0,-1);
    normals[1] = QVector3D(0,-1,0);
    normals[2] = QVector3D(-1,0,0);
    normals[3] = QVector3D(1,0,0);
    normals[4] = QVector3D(0,1,0);
    normals[5] = QVector3D(0,0,1);

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            mCubeNormals.push_back(normals[i].x());
            mCubeNormals.push_back(normals[i].y());
            mCubeNormals.push_back(normals[i].z());
        }
    }

    //UV values for texture mapping
    float cubeUVData[] = {
        // U   //V
        1.0f, 0.0f, // back face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f, // bottom face
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f, // left face
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f, // right face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f, // top face
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f, // front face
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    for (int i = 0; i < 36*2; i++) {
        mCubeUVCoords.push_back(cubeUVData[i]);
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

    // calc normals
    for (int i = 0; i < 3200*3; i++) {
        mSphereNormals.push_back(mSphereVerts[i]);
    }
}

QMatrix4x4 Viewer::getCameraMatrix() {
    QMatrix4x4 vMatrix;
    QVector3D lookAtPosition = mPlayer->getCameraLookAtPosition();

    QMatrix4x4 cameraInitialTransform;
    cameraInitialTransform.translate(0,mCameraHeight,20);

    mCameraPosition = (mCameraTransformation * mCameraRotation * 
                        cameraInitialTransform * mPlayer->getInitPosition()).toVector3D();
    QVector3D cameraUpDirection = QVector3D(0, 1, 0);

    vMatrix.lookAt(mCameraPosition, lookAtPosition, cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::draw_mesh(Mesh* mesh) {
    mProgram.bind();

    QMatrix4x4 transformMatrix = mesh->getTransform() * mesh->getRotationTransform();
    
    float reflectFactor;
    if (mDrawReflection) {
        // IMPORTANT: THIS ONLY WORKS FOR THE CLOSEST REFLECTING BOX...
        //            CAN'T DRAW MULTIPLE REFLECTIONS ON SEVERAL REFLECTING BOXES SIMULTANEOUSLY

        double distance;

        if (mDrawCharacterReflection && !mPlayer->isOverBox(&mReflectionType, &distance, &reflectFactor)) {
            return; // don't render reflection if not over box
        } else if (mDrawTrophyReflection && !mTrophy->isOverBox(&mReflectionType, &distance, &reflectFactor)) {
            return;
        }

        switch(mReflectionType) {
            case 0: {
                // back
                if (mCameraPosition.z() > mPlayer->getVertex1().z())
                    return;

                double shift = mesh->mVertex1.z() - mesh->mVertex2.z();
                transformMatrix.scale(1, 1, -1);
                transformMatrix.translate(0, 0, shift + distance*2);
                break;
            }
            case 1: {
                // bottom
                if (mCameraPosition.y() > mPlayer->getVertex2().y())
                    return;

                transformMatrix.translate(0, distance*2, 0);
                transformMatrix.scale(1, -1, 1);
                break;
            }
            case 2: {
                // left
                if (mCameraPosition.x() > mPlayer->getVertex2().x())
                    return;

                double shift = mesh->mVertex2.x() - mesh->mVertex1.x();
                transformMatrix.scale(-1, 1, 1);
                transformMatrix.translate(shift + distance*2, 0, 0);
                break;
            }
            case 3: {
                // right
                if (mCameraPosition.x() < mPlayer->getVertex1().x())
                    return;

                double shift = mesh->mVertex2.x() - mesh->mVertex1.x();
                transformMatrix.scale(-1, 1, 1);
                transformMatrix.translate(-shift - distance*2, 0, 0);
                break;
            }
            case 4: {   
                // top  
                if (mCameraPosition.y() < mPlayer->getVertex1().y())
                    return;

                QVector4D vertex = mesh->mVertex1;   
                transformMatrix.translate(0, vertex.y()*2 - distance*2,0);
                transformMatrix.scale(1, -1, 1);
                break;
            }
            case 5: {
                // front
                if (mCameraPosition.z() < mPlayer->getVertex2().z())
                    return;

                double shift = mesh->mVertex1.z() - mesh->mVertex2.z();
                transformMatrix.scale(1, 1, -1);
                transformMatrix.translate(0, 0, - shift - distance*2);
                break;
            }
            default: {
                break;
            }
        }

    }


    QMatrix4x4 modelViewMatrix = mTransformMatrix * transformMatrix;
    mProgram.setUniformValue(mMMatrixLocation, transformMatrix);
    mProgram.setUniformValue(mMvMatrixLocation, modelViewMatrix);
    mProgram.setUniformValue(mNormalMatrixLocation, modelViewMatrix.normalMatrix());
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);

    // LIGHTS
    mProgram.setUniformValueArray(mLightPositionLocation, mLights->mPositions, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightColourLocation, mLights->mColours, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightFalloffLocation, mLights->mFalloffs, NUM_LIGHTS);

    // CUBEMAP
    mProgram.setUniformValue(mDrawSkyBoxLocation, mDrawSkyBox);

    // REFLECTIONS
    mProgram.setUniformValue(mDrawReflectionLocation, mDrawReflection);
    mProgram.setUniformValue(mReflectFactorLocation, reflectFactor);

    // TRANSPARENCY
    mProgram.setUniformValue(mTransparencyLocation, 1);

    Textures* textures = mesh->mTextures;

    for (int i = 0; i < mesh->subMeshes.size(); i++) {
        SubMesh* subMesh = mesh->subMeshes[i];

        subMesh->mVBO.bind();
        mProgram.enableAttributeArray("vertexPosition");
        mProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

        subMesh->mNBO.bind();
        mProgram.enableAttributeArray("vertexNormal");
        mProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

        subMesh->mUVBO.bind();
        mProgram.enableAttributeArray("vertexTexCoord");
        mProgram.setAttributeBuffer("vertexTexCoord", GL_FLOAT, 0, 2);

        // TEXTURE MAPPING
        glActiveTexture(GL_TEXTURE1);
        mProgram.setUniformValue("tex1", 1);
        int index = subMesh->mMaterialIndex;
        glBindTexture(GL_TEXTURE_2D, textures->mIDs[index]);

        glActiveTexture(GL_TEXTURE2);
        mProgram.setUniformValue("tex2", 2);
        glBindTexture(GL_TEXTURE_2D, textures->mIDs[index]);


        glDrawArrays(GL_TRIANGLES, 0, subMesh->mNumTriangles);
    }
}

void Viewer::draw_cube(QMatrix4x4 transformMatrix, std::vector<int> textureIndices, float reflectFactor, float transparency) {
    if (mDrawStencil && reflectFactor == 0)
        return;

    mProgram.bind();

    mCubeBufferObject.bind();
    mProgram.enableAttributeArray("vertexPosition");
    mProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    mCubeNormalBufferObject.bind();
    mProgram.enableAttributeArray("vertexNormal");
    mProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

    mCubeUVBufferObject.bind();
    mProgram.enableAttributeArray("vertexTexCoord");
    mProgram.setAttributeBuffer("vertexTexCoord", GL_FLOAT, 0, 2);

    QMatrix4x4 modelViewMatrix = mTransformMatrix * transformMatrix;
    mProgram.setUniformValue(mMMatrixLocation, transformMatrix);
    mProgram.setUniformValue(mMvMatrixLocation, modelViewMatrix);
    mProgram.setUniformValue(mNormalMatrixLocation, modelViewMatrix.normalMatrix());
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);

    // LIGHTS
    mProgram.setUniformValueArray(mLightPositionLocation, mLights->mPositions, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightColourLocation, mLights->mColours, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightFalloffLocation, mLights->mFalloffs, NUM_LIGHTS);

    // CUBEMAP
    mProgram.setUniformValue(mDrawSkyBoxLocation, mDrawSkyBox);

    // REFLECTIONS
    mProgram.setUniformValue(mDrawReflectionLocation, mDrawReflection);
    mProgram.setUniformValue(mReflectFactorLocation, reflectFactor);

    // TRANSPARENCY
    mProgram.setUniformValue(mTransparencyLocation, transparency);

    // TEXTURE MAPPING
    for (int i = 0; i < textureIndices.size(); i++) {
        glActiveTexture(GL_TEXTURE1 + i);
        mProgram.setUniformValue(("tex" + std::to_string(i+1)).c_str(), i+1);

        int index = textureIndices[i];
        glBindTexture(GL_TEXTURE_2D, mTextures->mIDs[index]);
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Viewer::draw_sphere(QMatrix4x4 transformMatrix) {
    // NEED TO FIX
    mProgram.bind();

    mSphereBufferObject.bind();
    mProgram.enableAttributeArray("vertexPosition");
    mProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    mSphereNormalBufferObject.bind();
    mProgram.enableAttributeArray("vertexNormal");
    mProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

    QMatrix4x4 modelViewMatrix = mTransformMatrix * transformMatrix;
    mProgram.setUniformValue(mMMatrixLocation, transformMatrix);
    mProgram.setUniformValue(mMvMatrixLocation, modelViewMatrix);
    mProgram.setUniformValue(mNormalMatrixLocation, modelViewMatrix.normalMatrix());
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix() * transformMatrix);

    // LIGHTS
    mProgram.setUniformValueArray(mLightPositionLocation, mLights->mPositions, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightColourLocation, mLights->mColours, NUM_LIGHTS);
    mProgram.setUniformValueArray(mLightFalloffLocation, mLights->mFalloffs, NUM_LIGHTS);

    // CUBEMAP
    mProgram.setUniformValue(mDrawSkyBoxLocation, mDrawSkyBox);
    mProgram.setUniformValue(mWorldCameraPosLocation, QVector3D(0,0,0));

    glActiveTexture(GL_TEXTURE0);
    mProgram.setUniformValue("cubeMapTex", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap->mTexID);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3200);
}
