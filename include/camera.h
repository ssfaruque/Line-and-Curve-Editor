#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"


struct Camera
{

        Vector3f m_pos;


        Camera(const Vector3f& pos);

        inline Vector3f getPos() const{return m_pos;};

        friend class Scene;
};








#endif  // CAMERA_H