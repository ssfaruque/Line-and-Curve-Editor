#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <algorithm>

#include "object.h"
#include "graphics.h"
#include "scene.h"
#include "window.h"
#include "frameBuffer.h"
#include "camera.h"

#include "app.h"

static uint nextID = 0;


Object::Object(const std::vector<Vertex>& vertices, const std::vector<int>& indices, const std::vector<Triangle>& triangles):
m_vertices(vertices), m_indices(indices), m_triangles(triangles), m_id(nextID++)
{
}




void Object::buildInternalTriangles()
{
    m_triangles.clear();

    for(uint i = 0; i < m_indices.size(); i += 3)
    {
        Triangle triangle(m_vertices[m_indices[i] - 1], m_vertices[m_indices[i + 1] - 1], m_vertices[m_indices[i + 2] - 1]);
        m_triangles.push_back(triangle);
    }
}


float triangleMinz(const Triangle& triangle)
{
    float z = triangle.v1.position.z;
    z = z > triangle.v2.position.z ? triangle.v2.position.z : z;
    z = z > triangle.v3.position.z ? triangle.v3.position.z : z;
    return z;
}

template<typename T>
void swapVal(T& val1, T& val2)
{
    T tmp = val1;
    val1 = val2;
    val2 = tmp;
}


void changeTriangleToYZProj(Triangle& triangle)
{
    swapVal(triangle.v1.position.x, triangle.v1.position.y);
    swapVal(triangle.v1.position.y, triangle.v1.position.z);
    swapVal(triangle.v1.normal.x, triangle.v1.normal.y);
    swapVal(triangle.v1.normal.y, triangle.v1.normal.z);

    swapVal(triangle.v2.position.x, triangle.v2.position.y);
    swapVal(triangle.v2.position.y, triangle.v2.position.z);
    swapVal(triangle.v2.normal.x, triangle.v2.normal.y);
    swapVal(triangle.v2.normal.y, triangle.v2.normal.z);

    swapVal(triangle.v3.position.x, triangle.v3.position.y);
    swapVal(triangle.v3.position.y, triangle.v3.position.z);
    swapVal(triangle.v3.normal.x, triangle.v3.normal.y);
    swapVal(triangle.v3.normal.y, triangle.v3.normal.z);
}


void changeTriangleToXZProj(Triangle& triangle)
{
    swapVal(triangle.v1.position.y, triangle.v1.position.z);
    swapVal(triangle.v1.normal.y, triangle.v1.normal.z);

    swapVal(triangle.v2.position.y, triangle.v2.position.z);
    swapVal(triangle.v2.normal.y, triangle.v2.normal.z);

    swapVal(triangle.v3.position.y, triangle.v3.position.z);
    swapVal(triangle.v3.normal.y, triangle.v3.normal.z);
}


void Object::draw(const BoundingBox& boundingBox, Camera* camera)
{
    Matrix4f projection = createOrthoMatrix(boundingBox);

    Window* window = Window::getInstance();
    float width  = window->getFrameBuffer()->getWidth();
    float height = window->getFrameBuffer()->getHeight();

    /* copy of original vertices in world coordinates */
    //std::vector<Vertex> verticesCopy = m_vertices;

    buildInternalTriangles();

    printTriangleMeshData();

    std::vector<Triangle> trianglesCopy = m_triangles;

    std::sort(trianglesCopy.begin(),
              trianglesCopy.end(),
              [](const Triangle& t1, const Triangle& t2){return triangleMinz(t1) < triangleMinz(t2);});


    /* Apply ortho projection to each of the vertices in each triangle
     * and rasterize each triangle */
    for(Triangle& triangle : trianglesCopy)
    {
        triangle.v1.position = Vector3f(projection * Vector4f(triangle.v1.position.x, triangle.v1.position.y, triangle.v1.position.z, 1.0f));
        triangle.v1.normal   = Vector3f(projection * Vector4f(triangle.v1.normal.x, triangle.v1.normal.y, triangle.v1.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v1.position.x, &triangle.v1.position.y, width, height);

        triangle.v2.position = Vector3f(projection * Vector4f(triangle.v2.position.x, triangle.v2.position.y, triangle.v2.position.z, 1.0f));
        triangle.v2.normal   = Vector3f(projection * Vector4f(triangle.v2.normal.x, triangle.v2.normal.y, triangle.v2.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v2.position.x, &triangle.v2.position.y, width, height);

        triangle.v3.position = Vector3f(projection * Vector4f(triangle.v3.position.x, triangle.v3.position.y, triangle.v3.position.z, 1.0f));
        triangle.v3.normal   = Vector3f(projection * Vector4f(triangle.v3.normal.x, triangle.v3.normal.y, triangle.v3.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v3.position.x, &triangle.v3.position.y, width, height);

        // dda(triangle.v1.position.x, triangle.v1.position.y,
        //     triangle.v2.position.x, triangle.v2.position.y, 0x00FF00FF);
        // dda(triangle.v2.position.x, triangle.v2.position.y,
        //     triangle.v3.position.x, triangle.v3.position.y, 0x00FF00FF);
        // dda(triangle.v1.position.x, triangle.v1.position.y,
        //     triangle.v3.position.x, triangle.v3.position.y, 0x00FF00FF);
        
        rasterizeTriangle(triangle);
    }
}



void Object::drawWithXYProj(const BoundingBox& boundingBox, Camera* camera)
{
    Matrix4f projection = createOrthoMatrix(boundingBox);

    Window* window = Window::getInstance();
    float width  = window->getFrameBuffer()->getWidth();
    float height = window->getFrameBuffer()->getHeight();

    buildInternalTriangles();

    std::vector<Triangle> trianglesCopy = m_triangles;

    /* hidden surface removal by sorting triangles by their depths */
    std::sort(trianglesCopy.begin(),
              trianglesCopy.end(),
              [](const Triangle& t1, const Triangle& t2){return triangleMinz(t1) < triangleMinz(t2);});


    /* Apply ortho projection to each of the vertices in each triangle
     * and rasterize each triangle */
    for(Triangle& triangle : trianglesCopy)
    {
        triangle.v1.position = Vector3f(projection * Vector4f(triangle.v1.position.x, triangle.v1.position.y, triangle.v1.position.z, 1.0f));
        triangle.v1.normal   = Vector3f(projection * Vector4f(triangle.v1.normal.x, triangle.v1.normal.y, triangle.v1.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v1.position.x, &triangle.v1.position.y, width, height);

        triangle.v2.position = Vector3f(projection * Vector4f(triangle.v2.position.x, triangle.v2.position.y, triangle.v2.position.z, 1.0f));
        triangle.v2.normal   = Vector3f(projection * Vector4f(triangle.v2.normal.x, triangle.v2.normal.y, triangle.v2.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v2.position.x, &triangle.v2.position.y, width, height);

        triangle.v3.position = Vector3f(projection * Vector4f(triangle.v3.position.x, triangle.v3.position.y, triangle.v3.position.z, 1.0f));
        triangle.v3.normal   = Vector3f(projection * Vector4f(triangle.v3.normal.x, triangle.v3.normal.y, triangle.v3.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v3.position.x, &triangle.v3.position.y, width, height);

        rasterizeTriangle(triangle);
    }
}


void Object::drawWithYZProj(const BoundingBox& boundingBox, Camera* camera)
{
    Matrix4f projection = createOrthoMatrix(boundingBox);

    Window* window = Window::getInstance();
    float width  = window->getFrameBuffer()->getWidth();
    float height = window->getFrameBuffer()->getHeight();

    buildInternalTriangles();

    printTriangleMeshData();

    std::vector<Triangle> trianglesCopy = m_triangles;

    for(Triangle& triangle : trianglesCopy)
        changeTriangleToYZProj(triangle);

    std::sort(trianglesCopy.begin(),
              trianglesCopy.end(),
              [](const Triangle& t1, const Triangle& t2){return triangleMinz(t1) < triangleMinz(t2);});

    /* Apply ortho projection to each of the vertices in each triangle
     * and rasterize each triangle */
    for(Triangle& triangle : trianglesCopy)
    {
        triangle.v1.position = Vector3f(projection * Vector4f(triangle.v1.position.x, triangle.v1.position.y, triangle.v1.position.z, 1.0f));
        triangle.v1.normal   = Vector3f(projection * Vector4f(triangle.v1.normal.x, triangle.v1.normal.y, triangle.v1.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v1.position.x, &triangle.v1.position.y, width, height);

        triangle.v2.position = Vector3f(projection * Vector4f(triangle.v2.position.x, triangle.v2.position.y, triangle.v2.position.z, 1.0f));
        triangle.v2.normal   = Vector3f(projection * Vector4f(triangle.v2.normal.x, triangle.v2.normal.y, triangle.v2.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v2.position.x, &triangle.v2.position.y, width, height);

        triangle.v3.position = Vector3f(projection * Vector4f(triangle.v3.position.x, triangle.v3.position.y, triangle.v3.position.z, 1.0f));
        triangle.v3.normal   = Vector3f(projection * Vector4f(triangle.v3.normal.x, triangle.v3.normal.y, triangle.v3.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v3.position.x, &triangle.v3.position.y, width, height);

        rasterizeTriangle(triangle);
    }
}


void Object::drawWithXZProj(const BoundingBox& boundingBox, Camera* camera)
{
    Matrix4f projection = createOrthoMatrix(boundingBox);

    Window* window = Window::getInstance();
    float width  = window->getFrameBuffer()->getWidth();
    float height = window->getFrameBuffer()->getHeight();

    buildInternalTriangles();

    std::vector<Triangle> trianglesCopy = m_triangles;

    std::sort(trianglesCopy.begin(),
              trianglesCopy.end(),
              [](const Triangle& t1, const Triangle& t2){return triangleMinz(t1) < triangleMinz(t2);});


    /* Apply ortho projection to each of the vertices in each triangle
     * and rasterize each triangle */
    for(Triangle& triangle : trianglesCopy)
    {
        triangle.v1.position = Vector3f(projection * Vector4f(triangle.v1.position.x, triangle.v1.position.y, triangle.v1.position.z, 1.0f));
        triangle.v1.normal   = Vector3f(projection * Vector4f(triangle.v1.normal.x, triangle.v1.normal.y, triangle.v1.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v1.position.x, &triangle.v1.position.y, width, height);

        triangle.v2.position = Vector3f(projection * Vector4f(triangle.v2.position.x, triangle.v2.position.y, triangle.v2.position.z, 1.0f));
        triangle.v2.normal   = Vector3f(projection * Vector4f(triangle.v2.normal.x, triangle.v2.normal.y, triangle.v2.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v2.position.x, &triangle.v2.position.y, width, height);

        triangle.v3.position = Vector3f(projection * Vector4f(triangle.v3.position.x, triangle.v3.position.y, triangle.v3.position.z, 1.0f));
        triangle.v3.normal   = Vector3f(projection * Vector4f(triangle.v3.normal.x, triangle.v3.normal.y, triangle.v3.normal.z, 0.0f));
        normalToScreenCoords(&triangle.v3.position.x, &triangle.v3.position.y, width, height);

        changeTriangleToXZProj(triangle);

        rasterizeTriangle(triangle);
    }
}






void Object::translate(const Vector3f& vec)
{
    Matrix4f translationMatrix = createTranslationMatrix4f(vec);

    for(auto& vertex : m_vertices)
        vertex.position = Vector3f(translationMatrix * Vector4f(vertex.position, 1.0f));
}



void Object::scale(const Vector3f& vec)
{
    Matrix4f scaleMatrix = createScaleMatrix4f(vec);
    Vector3f centroid = calcCentroid();
    translate(Vector3f(-centroid.x, -centroid.y, -centroid.z));

    for(auto& vertex : m_vertices) 
        vertex.position = Vector3f(scaleMatrix * Vector4f(vertex.position, 1.0f));

    translate(Vector3f(centroid.x, centroid.y, centroid.z));
}


void Object::rotate(const Vector3f& p0, const Vector3f& p1, float angle)
{
    angle = toRadians(angle);

    Matrix4f rotationMatrix = create3DRotateMatrix(p0, p1, angle);

    for(auto& vertex : m_vertices)
    {
        vertex.position = Vector3f(rotationMatrix * Vector4f(vertex.position, 1.0f));
        vertex.normal = Vector3f(rotationMatrix * Vector4f(vertex.normal, 0.0f));
    }
}

Vector3f Object::calcCentroid() const 
{
    float x = 0.0f, y = 0.0f, z = 0.0f;

    for(const auto& vertex : m_vertices)
    {
        x += vertex.position.x;
        y += vertex.position.y;
        z += vertex.position.z;
    }

    x /= m_vertices.size();
    y /= m_vertices.size();
    z /= m_vertices.size();

    return Vector3f(x, y, z);
}


std::ostream& operator<<(std::ostream& os, const Object& object)
{
    os << "ID: " << object.getId() << ", "
       << "NumVertices: " << object.m_vertices.size() << ", " 
       << "NumEdges: "  << object.m_indices.size() / 2;
         
    return os;
}


void Object::printTriangleMeshData() const
{
    std::cout << "***** TRIANGLE MESH DATA *****" << std::endl;
    std::cout << "Object ID: " << m_id << std::endl;

    for(uint i = 0; i < m_triangles.size(); ++i)
    {
        std::cout << "Triangle" << " " << i << std::endl;
        std::cout << "Indices: " << m_triangles[i].v1.index << " " 
                                 << m_triangles[i].v2.index << " "
                                 << m_triangles[i].v3.index << std::endl;

        std::cout << "World coordinates: " << "(" << m_triangles[i].v1.position.x << ", " << m_triangles[i].v1.position.y << ", " << m_triangles[i].v1.position.z << ") " 
                                           << "(" << m_triangles[i].v2.position.x << ", " << m_triangles[i].v2.position.y << ", " << m_triangles[i].v2.position.z << ") " 
                                           << "(" << m_triangles[i].v3.position.x << ", " << m_triangles[i].v3.position.y << ", " << m_triangles[i].v3.position.z << ") " 
                                           << std::endl;

        std::cout << "Vertex intensities: " << "(" << m_triangles[i].v1.intensity.x << ", " << m_triangles[i].v1.intensity.y << ", " << m_triangles[i].v1.intensity.z << ") " 
                                            << "(" << m_triangles[i].v2.intensity.x << ", " << m_triangles[i].v2.intensity.y << ", " << m_triangles[i].v2.intensity.z << ") " 
                                            << "(" << m_triangles[i].v3.intensity.x << ", " << m_triangles[i].v3.intensity.y << ", " << m_triangles[i].v3.intensity.z << ") " 
                                            << std::endl;
        std::cout << std::endl;                         
    }
}