#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include "graphics.h"
#include "window.h"


#include "bspline.h"


Bspline::Bspline(const std::vector<Vertex>& points, int numSegments):
Curve(points, numSegments)
{
}



/* implementation of de Boor algorithm */
void Bspline::draw() const
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(3.0f);

    /* draw control points */
    glBegin(GL_POINTS);
    glColor3f(0.5f, 0.5f, 0.5f);

    for(auto v : m_points)
    {
        clipToNormalCoords(&v.position.x, &v.position.y, -500, 500, -500, 500);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();




    




}