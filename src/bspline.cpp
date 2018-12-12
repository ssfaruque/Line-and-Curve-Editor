#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>

#include "graphics.h"
#include "window.h"


#include "bspline.h"


Bspline::Bspline(const std::vector<Vertex>& points, int k, int numSegments):
Curve(points, numSegments), m_k(k)
{
}




float produceWeight(int pointNum, float u, float n, int k, const std::vector<int>& knots)
{
    if(k != 1)
    {
        float frac1 = float((u - knots[pointNum - 1]))   / float((knots[pointNum + k - 2] - knots[pointNum - 1]));
        float frac2 = float(knots[pointNum + k - 1] - u) / float(knots[pointNum  + k - 1] - knots[pointNum]);
        float weight1 = 0.0f, weight2 = 0.0f;

        if(frac1)   weight1 = produceWeight(pointNum, u, n, k - 1, knots) * frac1;  
        if(frac2)   weight2 = produceWeight(pointNum + 1, u, n, k - 1, knots) * frac2;

        return weight1 + weight2;
    }

    else
    {
        /* base case for recursion */
        if(u >= knots[pointNum - 1] && u < knots[pointNum]) return 1;
        return 0;
    }
}



/* implementation of de Boor algorithm */
void Bspline::draw() const
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(3.0f);

    /* draw control points */
    glBegin(GL_POINTS);
    glColor3f(0.1f, 0.1f, 1.0f);

    for(auto v : m_points)
    {
        clipToNormalCoords(&v.position.x, &v.position.y, -500, 500, -500, 500);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();





    glPointSize(1.0f);
    
    /* draw bspline curve using de boor algorithm */
    glBegin(GL_POINTS);
    glColor3f(0.6f, 0.2f, 0.9f);

    /* create knots vector that is equi-distant */
    std::vector<int> knots{};
    for(int i = 0; i < int(m_points.size()) + m_k; ++i)
        knots.push_back(i);


    float inc = 1.0f / m_numSegments;

    for(float t = 0.0f; t < 1.0001f; t += inc)
    {
        float u = t * (int(m_points.size()) - m_k + 1) + (m_k - 1);
        float x = 0.0f, y = 0.0f;

        for(int i = 1; i <= int(m_points.size()); ++i)
        {
            float w = produceWeight(i, u, int(m_points.size()), m_k, knots);
            x += (w * m_points[i - 1].position.x);  
            y += (w * m_points[i - 1].position.y);

        }

        clipToNormalCoords(&x, &y, -500, 500, -500, 500);

        glVertex3f(x, y, 0.0f);

    }

    glEnd();
}


void Bspline::printInfo() const
{
    std::cout << "Bspline Curve" << std::endl;
    std::cout << "K: " << m_k << std::endl;
    print();
}