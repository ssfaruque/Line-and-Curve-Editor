#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include "types.h"
#include "math.h"


class Camera;

void renderProjections();


void changeTriangleToYZProj(Triangle& triangle);
void changeTriangleToXZProj(Triangle& triangle);


class Object
{
    private:
        std::vector<Vertex> m_vertices;
        std::vector<int> m_indices;
        std::vector<Triangle> m_triangles;
        uint m_id;

    private:
        Vector3f calcCentroid() const;

        void calcNormals();

        void buildInternalTriangles();

    public:
        Object(const std::vector<Vertex>& vertices, const std::vector<int>& indices, const std::vector<Triangle>& triangles);
        ~Object() = default;
        void draw(const BoundingBox& boundingBox, Camera* camera);
        void drawWithXYProj(const BoundingBox& boundingBox, Camera* camera);
        void drawWithYZProj(const BoundingBox& boundingBox, Camera* camera);
        void drawWithXZProj(const BoundingBox& boundingBox, Camera* camera);


        void translate(const Vector3f& vec);
        void scale(const Vector3f& vec);
        void rotate(const Vector3f& p0, const Vector3f& p1, float angle);

        inline uint getId() const {return m_id;}


        friend std::ostream& operator<<(std::ostream& os, const Object& object);

        void printTriangleMeshData() const;

        

        friend class Scene;
};

#endif  // OBJECT_H