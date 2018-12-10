#ifndef LIGHT_H
#define LIGHT_H

#include "types.h"
#include "math.h"

struct Light
{
    Vector3f m_pos;
    Vector3f m_sourceIntensity;
    Vector3f m_ambIntensity;
    float m_K;  // non negative constant K
    float m_phongConstant;
    Color m_color;

    Vector3f m_ka;
    Vector3f m_kd;
    Vector3f m_ks;

    Light(const Vector3f& pos, Vector3f intensity, const Color color = Color(0xFFFFFFFF));
    Light();
    inline void setCoeffs(Vector3f ka, Vector3f kd, Vector3f ks);
    inline void setConstants(float K, float phongConstant);
    inline void setPos(const Vector3f& pos);



    void setSourceIntensity(const Vector3f intensity);
    void setAmbIntensity(const Vector3f intensity);
    void setK(const float K);
    void setPhongConstant(const float phongConstant);
    void setKA(const Vector3f vec);
    void setKD(const Vector3f vec);
    void setKS(const Vector3f vec);

};






#endif  // LIGHT_H