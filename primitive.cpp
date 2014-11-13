#include "primitive.hpp"
#include "AppWindow.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(int colourID, QMatrix4x4 transformMatrix) const
{
	// AppWindow::m_viewer->draw_sphere(colourID, transformMatrix);
}

