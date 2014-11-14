#include "primitive.hpp"
#include "AppWindow.hpp"

Primitive::~Primitive()
{
}


Cube::~Cube()
{
}

void Cube::walk_gl(QMatrix4x4 transformMatrix) const
{
	AppWindow::m_viewer->draw_cube(transformMatrix);
}


Sphere::~Sphere()
{
}

void Sphere::walk_gl(QMatrix4x4 transformMatrix) const
{
	AppWindow::m_viewer->draw_sphere(transformMatrix);
}

