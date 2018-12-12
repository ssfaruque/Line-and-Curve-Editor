#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>

#include "graphics.h"
#include "window.h"

#include "bezier.h"


Bezier::Bezier(const std::vector<Vertex>& points, int numSegments):
Curve(points, numSegments)
{
}


Vertex produceBezierPoint(const Vertex& v1, const Vertex& v2, float t)
{
    Vertex result;
    result.position.x = (1 - t) * v1.position.x + t * v2.position.x;
    result.position.y = (1 - t) * v1.position.y + t * v2.position.y;
    result.position.z = (1 - t) * v1.position.z + t * v2.position.z;
    return result;
}


/* implementation of de Casteljau algorithm */
void Bezier::draw() const
{
    //int width = Window::getInstance()->getWidth();
    //int height = Window::getInstance()->getHeight();

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(3.0f);

    /* draw control points */
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);

    for(auto v : m_points)
    {
        clipToNormalCoords(&v.position.x, &v.position.y, -500, 500, -500, 500);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();




    glPointSize(1.0f);

    /* draw bezier curve using de Casteljau algorithm*/
    glBegin(GL_POINTS);
    glPointSize(1.0f);
    glColor3f(1.0f, 1.0f, 0.0f);

    float inc = 1.0f / m_numSegments;

    for(float t = 0.0f; t <= 1.00001f; t += inc)
    {
        std::vector<Vertex> currGenPoints = m_points;

        do
        {
            std::vector<Vertex> nextGenPoints{};

            for(int i = 0; i < int(currGenPoints.size() - 1); ++i)
                nextGenPoints.push_back(produceBezierPoint(currGenPoints[i], currGenPoints[i + 1], t));

            currGenPoints = nextGenPoints;

        } while(int(currGenPoints.size() > 1 ));


        clipToNormalCoords(&currGenPoints[0].position.x, &currGenPoints[0].position.y, -500, 500, -500, 500);
        glVertex3f(currGenPoints[0].position.x, currGenPoints[0].position.y, currGenPoints[0].position.z);

    }

    glEnd();
}


void Bezier::printInfo() const
{
    std::cout << "Bezier Curve" << std::endl;
    print();
}