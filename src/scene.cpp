#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif


#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "scene.h"
#include "object.h"
#include "math.h"
#include "window.h"
#include "frameBuffer.h"
#include "camera.h"


BoundingBox Scene::m_boundingBox = {-500, 500, -500, 500, -500, 500};


Scene::Scene():
 m_objects({}),
 m_clippingWindow({}),
 m_clipXmin(-1.0f), m_clipXmax(1.0f), m_clipYmin(-1.0f), m_clipYmax(1.0f), m_clipZmin(-1.0f), m_clipZmax(1.0f)
{
    m_boundingBox = {-500, 500, -500, 500, -500, 500};

    m_light = new Light(Vector3f(0.0f, 350.0f, 0.0f), Vector3f(500.0, 500.0f, 500.0f));
    m_camera = new Camera(Vector3f(0.0f, 0.0f, 100000000.0f));
}


Scene::Scene(std::string loadFileName):
m_objects({}),
m_clippingWindow({}),
 m_clipXmin(-1.0f), m_clipXmax(1.0f), m_clipYmin(-1.0f), m_clipYmax(1.0f)

{
    m_boundingBox = {-500, 500, -500, 500, -500, 500};

    load(loadFileName);
}


float clampVal(float val, float minVal, float maxVal)
{
    val = val < minVal ? minVal : val;
    val = val > maxVal ? maxVal : val;
    return val;
}


/* phong lighting implementation,
   calculates intensities for r, g, b all at once using vectors

   Note: two vectors multiplied using '*' multiplies each vector
   element wise and returns a vector
   whereas the function 'dotProduct' computes the dot
   product of two vectors and returns a scalar result*/
void Scene::calcVertexIntensities()
{
    for(Object* object : m_objects)
    {
        for(Vertex& vertex : object->m_vertices)
        {
            Vector3f intensity(0.0f);
            Vector3f ambComp(0.0f);
            Vector3f diffComp(0.0f);
            Vector3f specComp(0.0f);

            Vector3f l = normalize(m_light->m_pos - vertex.position);
            Vector3f normal = normalize(vertex.normal);
            Vector3f r = normalize(reflect(l, normal));    
            Vector3f v = normalize(m_camera->getPos() - vertex.position);

            float K = m_light->m_K;
            Vector3f fraction = (m_light->m_sourceIntensity * 255.0f) / (magnitude(m_camera->getPos() - vertex.position) + K);

            float rvProd = dotProduct(r, v);
            float nlProd = dotProduct(normal, l);
            float nvProd = dotProduct(normal, v);

            ambComp  = m_light->m_ka * m_light->m_ambIntensity;
            diffComp = fraction * (m_light->m_kd * nlProd);
            specComp = rvProd > 0 ? fraction * (m_light->m_ks * powf(rvProd, m_light->m_phongConstant)) : Vector3f(0.0f, 0.0f, 0.0f);

            if((nlProd > 0 && nvProd < 0) || (nlProd < 0 && nvProd > 0))
            {
                diffComp = Vector3f(0.0f, 0.0f, 0.0f);
                specComp = Vector3f(0.0f, 0.0f, 0.0f);
            }
            
            // clamp values
            diffComp.r = clampVal(diffComp.r, 0.0f, 1.0f);
            diffComp.g = clampVal(diffComp.g, 0.0f, 1.0f);
            diffComp.b = clampVal(diffComp.b, 0.0f, 1.0f);
            specComp.r = clampVal(specComp.r, 0.0f, 1.0f);
            specComp.g = clampVal(specComp.g, 0.0f, 1.0f);
            specComp.b = clampVal(specComp.b, 0.0f, 1.0f);

            intensity = ambComp + diffComp + specComp;

            intensity.r = clampVal(intensity.r, 0.0f, 1.0f);
            intensity.g = clampVal(intensity.g, 0.0f, 1.0f);
            intensity.b = clampVal(intensity.b, 0.0f, 1.0f);
        }
    }
}



Scene::~Scene()
{
}




void Scene::addObject(Object* object)
{
    m_objects.push_back(object);
}


void Scene::clean()
{
    m_objects.clear();
}


void Scene::draw()
{
    calcVertexIntensities();

    for(auto& object : m_objects)
    {
        object->drawWithXYProj(m_boundingBox, m_camera);
    }
}


void Scene::drawWithProjectionXY()
{
    // for(auto& object : m_objects)
    //     object->drawWithProjectionXY(m_boundingBox);
}


void Scene::drawWithProjectionXZ()
{

}


void Scene::drawWithProjectionYZ()
{

}







void Scene::displayObjects() const
{
    std::cout << "*** SCENE CONTENTS ***" << std::endl;

    for(auto& object : m_objects)
        std::cout << *object << std::endl;

    std::cout << "*************************************************" << std::endl;    
}


void Scene::load(std::string loadFileName)
{
    std::ifstream inf(loadFileName);
    std::string line;
    std::stringstream iss;

    if(inf.is_open())
    {
        std::getline(inf, line);
        int numObjects = std::stoi(line);
        std::getline(inf, line);    // empty line
        
        for(int i = 0; i < numObjects; ++i)
        {
            std::vector<Vertex> vertices{};
            std::vector<int> indices{};
            std::vector<Triangle> triangles{};

            std::getline(inf, line);
            int numPoints = std::stoi(line);

            for(int j = 0; j < numPoints; ++j)
            {
                float x, y, z;

                // world position
                std::getline(inf, line);
                iss << line;
                iss >> x >> y >> z;
                iss.clear();
                Vector3f position(x, y, z);

                // normal vector
                std::getline(inf, line);
                iss << line;
                iss >> x >> y >> z;
                iss.clear();
                Vector3f normal(x, y, z);

                vertices.emplace_back(position, normal, 0xFFFFFFFF, j + 1);
            }

            std::getline(inf, line);
            int numTriangles = std::stoi(line);

            for(int k = 0; k < numTriangles; ++k)
            {
                std::getline(inf, line);
                iss << line;
                int p1, p2, p3;
                iss >> p1 >> p2 >> p3;
                iss.clear();
                indices.push_back(p1);
                indices.push_back(p2);
                indices.push_back(p3);

                triangles.emplace_back(vertices[p1 - 1], vertices[p2 - 1], vertices[p3 - 1]);
            }

            m_objects.push_back(new Object(vertices, indices, triangles));

            std::getline(inf, line);

        }
    }

    else
        std::cout << "Failed to open file: " << loadFileName << std::endl;

    inf.close();


}


void Scene::save(std::string saveFileName) const
{
    std::ofstream outf(saveFileName);
    std::string line;
    std::stringstream iss;

    outf << m_objects.size() << std::endl << std::endl;

    for(auto& object : m_objects)
    {
        outf << object->m_vertices.size() << std::endl;

        for(auto& vertex : object->m_vertices)
        {
            outf << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
            outf << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << std::endl;
        }

        outf << object->m_triangles.size() << std::endl;

        for(Triangle& triangle : object->m_triangles)
        {
            outf << triangle.v1.index << " " << triangle.v2.index << " " << triangle.v3.index << std::endl;
        }

        outf << std::endl;
    }

    outf.close();
}


void Scene::setClippingWindow(float clipXmin, float clipXmax, float clipYmin, float clipYmax)
{
    m_clipXmin = clipXmin;  m_clipXmax = clipXmax;
    m_clipYmin = clipYmin;  m_clipYmax = clipYmax;

    /*Vertex v1(clipXmin, clipYmax);
    Vertex v2(clipXmax, clipYmax);
    Vertex v3(clipXmax, clipYmin);
    Vertex v4(clipXmin, clipYmin);
*/
    m_clippingWindow.clear();

  /*  m_clippingWindow.push_back(v1);
    m_clippingWindow.push_back(v2);
    m_clippingWindow.push_back(v3);
    m_clippingWindow.push_back(v4);
    */
}


void Scene::setBoundingBox(float clipXmin, float clipXmax, float clipYmin, float clipYmax, float clipZmin, float clipZmax)
{
    m_clipXmin = clipXmin;  m_clipXmax = clipXmax;
    m_clipYmin = clipYmin;  m_clipYmax = clipYmax;
    m_clipZmin = clipZmin;  m_clipZmax = clipZmax;    
}




void Scene::removeObject(uint id)
{
    auto newEnd = std::remove_if(m_objects.begin(),
                                 m_objects.end(),
                                 [id](Object* object){return object->getId() == id;});
    m_objects.erase(newEnd, m_objects.end());    
}


void Scene::loadProjectionMatrix(Matrix4f projection)
{
    m_projection = projection;
}



void Scene::resizeBoundingBox(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
    m_boundingBox.xmin = xmin;  m_boundingBox.xmax = xmax; 
    m_boundingBox.ymin = ymin;  m_boundingBox.ymax = ymax; 
    m_boundingBox.zmin = zmin;  m_boundingBox.zmax = zmax; 
}


void Scene::displayVertices(uint id)
{
    for(auto& object : m_objects)
    {
        if(object->m_id == id)
        {
            std::cout << "- Vertices-" << std::endl;

            for(auto& vertex : object->m_vertices)
            {
                std::cout << "(" 
                          << vertex.position.x << ", " 
                          << vertex.position.y << ", "
                          << vertex.position.z << ")" << std::endl; 
            }

            return;
        }
    }

    std::cout << "Object with id " << id << " not found!" << std::endl;
}