#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <vector>

#include "math.h"

using uint = unsigned int;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;


struct Color
{
    union
    {
        u32 color;

        struct
        {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
    };

    Color(u32 pColor) {color = pColor;}
    Color(u8 pr, u8 pg, u8 pb, u8 pa = 0xFF) {r = pr; g = pg; b = pb; a = pa;}
    Color() {color = 0;}
};



struct Vertex
{
    Vector3f position;
    Vector3f normal;
    Vector3f intensity;
    Color color;

    uint index;

    Vertex(const Vector3f& pos, const Vector3f& norm, const Color& col, uint ind = 0):
    position(pos), normal(norm), color(col), index(ind)
    {
        intensity = (Vector3f(1.0f, 1.0f, 1.0f));
    }   

    Vertex()
    {}
};


struct Edge
{
    float yMax;
    float yMin;
    float startingX;
    float slopeInv;

    Vertex v1Ymin;
    Vertex v2YMax;

    Vertex v1;
    Vertex v2;

    Edge(float pyMax = 0.0f, float pyMin = 0.0f, float pstartingX = 0.0f, float pslopeInv = 0.0f, Vertex p_v1 = Vertex(), Vertex p_v2 = Vertex()):
    yMax(pyMax), yMin(pyMin), startingX(pstartingX), slopeInv(pslopeInv)
    {
        v1 = p_v1; 
        v2 = p_v2;
    }
};


struct BoundingBox
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
};


struct Triangle
{
    Vertex v1;
    Vertex v2;
    Vertex v3;

    Triangle(const Vertex& p_v1, const Vertex& p_v2, const Vertex& p_v3):
    v1(p_v1), v2(p_v2), v3(p_v3)
    {}
};



/*float triangleMaxz(const Triangle& triangle)
{
    float z = triangle.v1.position.z;
    z = z < triangle.v2.position.z ? triangle.v2.position.z : z;
    z = z < triangle.v3.position.z ? triangle.v3.position.z : z;
    return z;
}*/


#endif  // TYPES_H