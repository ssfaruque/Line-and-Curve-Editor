#include "light.h"


Light::Light(const Vector3f& pos, Vector3f intensity, const Color color):
m_pos(pos), m_sourceIntensity(intensity), m_color(color),
m_ka(1.0f, 0.0f, 0.0f),
m_kd(0.0f, 1.0f, 0.0f),
m_ks(0.0f, 0.0f, 1.0f)
{
    m_ambIntensity = Vector3f(0.05f, 0.05f, 0.05f);
    setConstants(500, 4);
}


Light::Light()
{}

void Light::setCoeffs(Vector3f ka, Vector3f kd, Vector3f ks)
{
    m_ka = ka;
    m_kd = kd;
    m_ks = ks;
}



void Light::setConstants(float K, float phongConstant)
{
    m_K = K;
    m_phongConstant = phongConstant;
}


void Light::setPos(const Vector3f& pos)
{
    m_pos = pos;
}



void Light::setSourceIntensity(const Vector3f intensity)
{
    m_sourceIntensity = intensity;
}


void Light::setAmbIntensity(const Vector3f intensity)
{
    m_ambIntensity = intensity;
}


void Light::setK(const float K)
{
    m_K = K;
}


void Light::setPhongConstant(const float phongConstant)
{
    m_phongConstant = phongConstant;
}


void Light::setKA(const Vector3f vec)
{
    m_ka = vec;
}


void Light::setKD(const Vector3f vec)
{
    m_kd = vec;
}

void Light::setKS(const Vector3f vec)
{
    m_ks = vec;
}