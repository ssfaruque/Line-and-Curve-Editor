#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif


#include "graphics.h"
#include "window.h"

#include "bezier.h"


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
    int width = Window::getInstance()->getWidth();
    int height = Window::getInstance()->getHeight();
    float inc = 1.0f / m_numSegments;


    for(float t = 0.0f; t <= 1.0f; t += inc)
    {
        std::vector<Vertex> currGenPoints = m_points;

        do
        {
            std::vector<Vertex> nextGenPoints{};

            for(int i = 0; i < int(currGenPoints.size() - 1); ++i)
                nextGenPoints.push_back(produceBezierPoint(currGenPoints[i], currGenPoints[i + 1], t));

            currGenPoints = nextGenPoints;

        } while(int(currGenPoints.size() > 1 ));


        /* convert to clip, normalized, screen coords */

        /* draw currGenPoints[0] */

    }


}
