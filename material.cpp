#include "material.hpp"
#include "AppWindow.hpp"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl(QMatrix4x4 transformMatrix) const
{
	AppWindow::m_viewer->setMaterial(m_kd, m_ks, m_shininess, transformMatrix);
}
