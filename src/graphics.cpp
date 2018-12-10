#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include <cassert>
#include <iostream>
#include <algorithm>

#include "graphics.h"
#include "window.h"
#include "frameBuffer.h"
#include "math.h"



bool halfToningEnabled = false;

void enableHalfToning()
{
    halfToningEnabled = true;
}

void disableHalfToning()
{
    halfToningEnabled = false;
}


void bresenhamSmallSlope(uint x1, uint y1, uint x2, uint y2, Color color);
void bresenhamHighSlope(uint x1, uint y1, uint x2, uint y2, Color color);


inline uint absVal(int num) { return num > 0 ? num : (num * -1); }
inline float absVal(float num) { return num > 0 ? num : (num * -1); }


void normalToScreenCoords(float* x, float* y)
{
    Window *window = Window::getInstance();
    *x = (window->getFrameBuffer()->getWidth() / 2) * (*x + 1);
    *y = (window->getFrameBuffer()->getHeight() / 2) * (*y + 1);

    // clamp x and y to boundaries, index in array starts at 0
    *x = uint(*x) == window->getFrameBuffer()->getWidth()  ? window->getFrameBuffer()->getWidth()  - 1 : *x;
    *y = uint(*y) == window->getFrameBuffer()->getHeight() ? window->getFrameBuffer()->getHeight() - 1 : *y;
}


void normalToScreenCoords(float* x, float* y, int width, int height)
{
    *x = (width  / 2.0f)  * (*x + 1);
    *y = (height / 2.0f)  * (*y + 1);

    // *x = uint(*x) == width ? width  - 1 : *x;
    // *y = uint(*y) == height ? height - 1 : *y;
}



void clipToNormalCoords(float* x, float* y, float xmin, float xmax, float ymin, float ymax) 
{
    float clipWidth  = xmax - xmin;
    float clipHeight = ymax - ymin;

    *x = 2 * ((*x - xmax) / clipWidth)  + 1;
    *y = 2 * ((*y - ymax) / clipHeight) + 1;
} 



void worldToNormalCoords(float* x, float* y, float* z,  const BoundingBox& boundingBox)
{
    float boxWidth  = boundingBox.xmax - boundingBox.xmin;
    float boxHeight = boundingBox.ymax - boundingBox.ymin;
    float boxDepth  = boundingBox.zmax - boundingBox.zmin;

    *x = 2.0f * ((*x - boundingBox.xmin) / boxWidth)  - 1;
    *y = 2.0f * ((*y - boundingBox.ymin) / boxHeight) - 1;
    *z = 2.0f * ((*z - boundingBox.zmin) / boxDepth)  - 1;
}




void drawPixel(float x, float y, Color color, bool normalized)
{
    Window *window = Window::getInstance();
    if (normalized){normalToScreenCoords(&x, &y);}

    if (!(x >= 0 && x <= window->getFrameBuffer()->getWidth() && y >= 0 && y <= window->getFrameBuffer()->getHeight()))
    {
        std::cout << "** pixel out of bounds **" << std::endl;
        std::cout << "Screen coords -> "
                  << "x: " << x << ", y: " << y << std::endl
                  << std::endl;
    }

    window->getFrameBuffer()->setPixel(x, y, color.color);
}


void drawLine(uint x1, uint y1, uint x2, uint y2, Color color, bool drawWithBresenham)
{
    if(drawWithBresenham)   bresenham(x1, y1, x2, y2, color);
    else                    dda(x1, y1, x2, y2, color);
}


void dda(float x1, float y1, float x2, float y2, Color color)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int numSteps = absVal(deltaX) > absVal(deltaY) ? absVal(deltaX) : absVal(deltaY);
    float xStep = (float)(deltaX) / (float)(numSteps);
    float yStep = (float)(deltaY) / (float)(numSteps);
    float roundTerm = 0.5f; /* E.g. If number is 4.6, adding 0.5 will
                             * result in 5.1 and will be rounded
                             * down to 5 when casted to a uint
                             */
    float x = x1;
    float y = y1;

    for (int step = 0; step < numSteps; ++step)
    {
        drawPixel((x + roundTerm), (y + roundTerm), color, false);
        x += xStep;
        y += yStep;
    }
}


void bresenham(float x1, float y1, float x2, float y2, Color color)
{
    int absDeltaX = absVal(x2 - x1);
    int absDeltaY = absVal(y2 - y1);

    if(absDeltaY < absDeltaX)   // slope < 1
    {
        // switch order of parameters depending on order of vertices
        x1 < x2 ?
        bresenhamSmallSlope(x1, y1, x2, y2, color) :
        bresenhamSmallSlope(x2, y2, x1, y1, color);
    }

    else    // slope >= 1
    {
        // switch order of parameters depending on order of vertices
        y1 < y2 ?
        bresenhamHighSlope(x1, y1, x2, y2, color) :
        bresenhamHighSlope(x2, y2, x1, y1, color);
    }
}


// When slope < 1
void bresenhamSmallSlope(uint x1, uint y1, uint x2, uint y2, Color color)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int yInc = (deltaY >= 0) ? 1 : -1;
    deltaY = (deltaY >= 0) ? deltaY : -deltaY;

    int error = 2 * deltaY - deltaX;

    for (uint x = x1, y = y1; x <= x2; ++x)
    {
        drawPixel(x, y, color, false);

        if (error > 0)
        {
            y += yInc;
            error -= (2 * deltaX);
        }

        error += (2 * deltaY);
    }
}


// When slope >= 1
void bresenhamHighSlope(uint x1, uint y1, uint x2, uint y2, Color color)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    int xInc = (deltaX >= 0) ? 1 : -1;
    deltaX = (deltaX >= 0) ? deltaX : -deltaX;

    int error = 2 * deltaX - deltaY;

    for (uint y = y1, x = x1; y <= y2; ++y)
    {
        drawPixel(x, y, color, false);

        if (error > 0)
        {
            x += xInc;
            error -= (2 * deltaY);
        }

        error += (2 * deltaX);
    }
}


std::vector<Edge> createEdges(const std::vector<Vertex>& vertices, const std::vector<int> indices)
{
    std::vector<Edge> edges{};

    for(int i = 0; i < (int)indices.size(); i += 2)
    {
        float yMax, yMin, startingX, slopeInv;

        Vertex v1 = vertices[indices[i] - 1];       // supposed to contain ymin
        Vertex v2 = vertices[indices[i + 1] - 1];   // supposed to contain ymax

        if(uint(v1.position.y) > uint(v2.position.y))
        {
            Vertex tmp = v1;
            v1 = v2;
            v2 = tmp;
        }

        yMin = v1.position.y;
        yMax = v2.position.y;
        startingX = v1.position.x;
        slopeInv = uint(v1.position.x) < uint(v2.position.x) ?
                    (v2.position.x - v1.position.x) / (v2.position.y - v1.position.y) :
                    (v1.position.x - v2.position.x) /(v1.position.y - v2.position.y);

        // don't include horizontal edges
        if(uint(yMin) == uint(yMax)) continue;

        edges.emplace_back(yMax, yMin, startingX, slopeInv, v1, v2);

    }

    /*for(int i = 0; i < (int)vertices.size(); ++i)
    {
        float yMax, yMin, startingX, slopeInv;

        if(i == (int)(vertices.size() - 1))
        {
            yMax = uint(vertices[i].y) > uint(vertices[0].y) ? vertices[i].y : vertices[0].y;
            yMin = uint(vertices[i].y) < uint(vertices[0].y) ? vertices[i].y : vertices[0].y;
            startingX = uint(yMin) == uint(vertices[i].y) ? vertices[i].x : vertices[0]. x;
            slopeInv = uint(vertices[i].x) < uint(vertices[0].x) ?
                    (vertices[0].x - vertices[i].x) / (vertices[0].y - vertices[i].y) :
                    (vertices[i].x - vertices[0].x) / (vertices[i].y - vertices[0].y);                    
        }

        else
        {
            yMax = uint(vertices[i].y) > uint(vertices[i + 1].y) ? vertices[i].y : vertices[i + 1].y;
            yMin = uint(vertices[i].y) < uint(vertices[i + 1].y) ? vertices[i].y : vertices[i + 1].y;
            startingX = uint(yMin) == uint(vertices[i].y) ? vertices[i].x : vertices[i + 1].x;
            slopeInv = uint(vertices[i].x) < uint(vertices[i + 1].x) ?
                    (vertices[i + 1].x - vertices[i].x) / (vertices[i + 1].y - vertices[i].y) :
                    (vertices[i].x - vertices[i + 1].x) /(vertices[i].y - vertices[i + 1].y);
        }

        // don't include horizontal edges
        if(uint(yMin) == uint(yMax)) continue;

        edges.emplace_back(yMax, yMin, startingX, slopeInv);
    }*/

    std::sort(edges.begin(),
              edges.end(),
              [](const Edge& e1, const Edge& e2)
              {return uint(e1.yMin) < uint(e2.yMin);});

    return edges;
}


std::vector<std::vector<Edge>> createEdgeList(const std::vector<Edge>& edges)
{
    uint yMin = edges[0].yMin, yMax = edges[0].yMax;

    for(auto& edge : edges)
    {
        if(uint(yMax) < uint(edge.yMax)) yMax = edge.yMax;
        if(uint(yMin) > uint(edge.yMin)) yMin = edge.yMin;
    }

    // allocate size to total num of scanlines
    std::vector<std::vector<Edge>> edgeList(yMax - yMin + 1); 

    for(uint scanLine = yMin; scanLine <= yMax; ++scanLine)
    {
        int index = scanLine - yMin;

        for(auto& edge : edges)
        {
            // add to active edge list
            if(scanLine == uint(edge.yMin))
                edgeList[index].push_back(edge);
        }

        sort(edgeList[index].begin(),
             edgeList[index].end(),
             [](const Edge& e1, const Edge& e2)
             {return uint(e1.startingX) < uint(e2.startingX);});
    }

    return edgeList;   
}



void printEdges(const std::vector<Edge>& edgeList)
{
    std::cout << "** CONTENTS OF EDGE LIST **" << std::endl;
    for(auto& edge : edgeList)
    {
        std::cout << "starting x: " << edge.startingX << ", " 
                  << "slope: " << 1 / edge.slopeInv << ", "
                  << "yMin: " << edge.yMin << ", "
                  << "yMax: " << edge.yMax << ", " 
                  << "v1YMin: (" << edge.v1Ymin.position.x << ", " << edge.v1Ymin.position.y << "), " 
                  << "v2YMax: (" << edge.v2YMax.position.x << ", " << edge.v2YMax.position.y << ")" << std::endl;             
    }
    std::cout << std::endl;
}


// removes all edges that have a yMax equivalent to the given scanLine value
void cleanActiveEdgeList(std::vector<Edge>& activeEdgeList, uint scanLine)
{
    auto newEnd = std::remove_if(activeEdgeList.begin(),
                        activeEdgeList.end(),
                        [scanLine](const Edge& e) {return uint(e.yMax) == uint(scanLine);});
    activeEdgeList.erase(newEnd, activeEdgeList.end());
}


/* implementation of Gourad shading using interpolation */
void interpolateVertices(const Edge& e1, const Edge& e2, float i, float il, float ir, float j, float* depth, Vector3f* intensity)
{
    /* get approproiate v1, v2, v3 vertices */
    Vertex v1, v2, v3;
    v1 = v2 = v3 = e1.v1;

    v1 = v1.position.y < e1.v2.position.y ? e1.v2 : v1;
    v1 = v1.position.y < e2.v1.position.y ? e2.v1 : v1;
    v1 = v1.position.y < e2.v2.position.y ? e2.v2 : v1;

    v3 = v3.position.y > e1.v2.position.y ? e1.v2 : v3;
    v3 = v3.position.y > e2.v1.position.y ? e2.v1 : v3;
    v3 = v3.position.y > e2.v2.position.y ? e2.v2 : v3;

    uint index1 = v1.index, index3 = v3.index;
    if     (e1.v1.index != index1 && e1.v1.index != index3) {v2 = e1.v1;}
    else if(e1.v2.index != index1 && e1.v2.index != index3) {v2 = e1.v2;}
    else if(e2.v1.index != index1 && e2.v1.index != index3) {v2 = e2.v1;}
    else if(e2.v2.index != index1 && e2.v2.index != index3) {v2 = e2.v2;}


    float j1 = v1.position.y;
    float j2 = v2.position.y;
    float j3 = v3.position.y;


    /*************** calculate interpolated intensity ***************/

    float fraction1L = absVal((j - j2) / (j1 - j2));
    float fraction2L = absVal((j1 - j) / (j1 - j2));
    float fraction1R = absVal((j - j3) / (j1 - j3));
    float fraction2R = absVal((j1 - j) / (j1 - j3));
    float fraction1I = absVal((ir - i) / (ir - il));
    float fraction2I = absVal((i - il) / (ir - il));

    fraction1L = uint(j1) == uint(j2) ? 0 : fraction1L;
    fraction2L = uint(j1) == uint(j2) ? 0 : fraction2L;
    fraction1R = uint(j1) == uint(j3) ? 0 : fraction1R;
    fraction2R = uint(j1) == uint(j3) ? 0 : fraction2R;
    fraction1I = uint(ir) == uint(il) ? 0 : fraction1I;
    fraction2I = uint(ir) == uint(il) ? 0 : fraction2I;


    Vector3f I1 = v1.intensity;
    Vector3f I2 = v2.intensity;
    Vector3f I3 = v3.intensity;


    Vector3f IL = (fraction1L * I1) + (fraction2L * I2);
    Vector3f IR = (fraction1R * I1) + (fraction2R * I3);
    Vector3f I = (fraction1I * IL) + (fraction2I * IR);

    *intensity = I;

    /*****************************************************************/

    /***************** calculate interpolated depth *****************/
    float z1 = v1.position.z;
    float z2 = v2.position.z;
    float z3 = v3.position.z;

    float zL = fraction1L * z1 + fraction2L * z2;
    float zR = fraction1R * z1 + fraction2R * z3;         
    float z  = fraction1I * zL + fraction2I * zR;

    *depth = z;

    /****************************************************************/
}



void rasterize(const std::vector<Vertex>& vertices, const std::vector<int> indices, Color color)
{
    // supposed to add only non horizontal edges to table
    std::vector<Edge> edges = createEdges(vertices, indices);
    printEdges(edges);
    std::vector<std::vector<Edge>> edgeList = createEdgeList(edges);

    int yMin = edges[0].yMin, yMax = edges[0].yMax;

    for(auto& edge : edges)
    {
        if(uint(yMax) < uint(edge.yMax)) yMax = edge.yMax;
        if(uint(yMin) > uint(edge.yMin)) yMin = edge.yMin;
    }

    std::vector<Edge> activeEdgeList{};

    for(int scanLine = yMin; scanLine <= yMax; ++scanLine)
    {
        int index = scanLine - yMin;

        // adds new edges to active edge list
        if(!edgeList[index].empty())
        {
            for(auto& edge : edgeList[index])
                activeEdgeList.push_back(edge);
        }

        cleanActiveEdgeList(activeEdgeList, scanLine);
        std::sort(activeEdgeList.begin(),
                  activeEdgeList.end(),
                  [](const Edge& e1, const Edge& e2){return uint(e1.startingX) < uint(e2.startingX);});

        if(activeEdgeList.size() > 1 && scanLine != yMin)
        {
            for(int i = 0; i < (int)activeEdgeList.size(); i += 2)
            {
                for(uint x1 = activeEdgeList[i].startingX + 1,
                    x2 = activeEdgeList[i + 1].startingX;
                    x1 < x2;
                     ++x1)
                     {
                        Edge e1 = activeEdgeList[i];
                        Edge e2 = activeEdgeList[i + 1];
                        float z;
                        Vector3f intensity;


                        interpolateVertices(e1, e2, x1, activeEdgeList[i].startingX, activeEdgeList[i + 1].startingX, scanLine, &z, &intensity);

                        if(z > Window::getInstance()->getFrameBuffer()->getDepth(x1, scanLine))
                        {
                            Vector3f colorVector = Vector3f(color.r, color.g, color.b) * intensity;
                            Color drawColor = Color(colorVector.r, colorVector.g, colorVector.b, 0xFF);
                            drawPixel(x1, scanLine, drawColor, false);
                            Window::getInstance()->getFrameBuffer()->setDepth(x1, scanLine, z);                           
                        } 
                     }
            }
        }
        
        cleanActiveEdgeList(activeEdgeList, scanLine);

        for(auto& edge : activeEdgeList)
            edge.startingX += edge.slopeInv;
    }
}



/* I did not have time for half-toning so pseudo code
is written below for what I would have done */ 
void rasterizeWithHalfToning(const Triangle& triangle, int intensityLevel)
{
    /* Divide frame buffer into 3 by 3 mega pixels */

    /* Perform all the same operations as in rasterizeTriangle function except until
       the time when drawPixel is called */

    /* The megapixel to be mapped will utilize a modulo (%) operator most likely */

    /* The number of physical pixels to turn on within the megapixel is based on
       the intensityLevel parameter and a random number generator will be
       utilized to turn on the specified number of pixels in a random fashion */
}




std::vector<Edge> createEdgesFromTriangle(const Triangle& triangle)
{
    std::vector<Edge> edges{};
    std::vector<Vertex> vertices{};

    Vertex v1 = triangle.v1;
    Vertex v2 = triangle.v2;
    Vertex v3 = triangle.v3;

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    for(int i = 0; i < (int)vertices.size(); ++i)
    {
        float yMax, yMin, startingX, slopeInv;
        Vertex v1, v2;

        if(i == (int)(vertices.size() - 1))
        {
            v1 = vertices[i];
            v2 = vertices[0];

            yMax = uint(vertices[i].position.y) > uint(vertices[0].position.y) ? vertices[i].position.y : vertices[0].position.y;
            yMin = uint(vertices[i].position.y) < uint(vertices[0].position.y) ? vertices[i].position.y : vertices[0].position.y;
            startingX = uint(yMin) == uint(vertices[i].position.y) ? vertices[i].position.x : vertices[0].position.x;
            slopeInv = uint(vertices[i].position.x) < uint(vertices[0].position.x) ?
                    (vertices[0].position.x - vertices[i].position.x) / (vertices[0].position.y - vertices[i].position.y) :
                    (vertices[i].position.x - vertices[0].position.x) / (vertices[i].position.y - vertices[0].position.y);                    
        }

        else
        {
            v1 = vertices[i];
            v2 = vertices[i + 1];

            yMax = uint(vertices[i].position.y) > uint(vertices[i + 1].position.y) ? vertices[i].position.y : vertices[i + 1].position.y;
            yMin = uint(vertices[i].position.y) < uint(vertices[i + 1].position.y) ? vertices[i].position.y : vertices[i + 1].position.y;
            startingX = uint(yMin) == uint(vertices[i].position.y) ? vertices[i].position.x : vertices[i + 1].position.x;
            slopeInv = uint(vertices[i].position.x) < uint(vertices[i + 1].position.x) ?
                    (vertices[i + 1].position.x - vertices[i].position.x) / (vertices[i + 1].position.y - vertices[i].position.y) :
                    (vertices[i].position.x - vertices[i + 1].position.x) /(vertices[i].position.y - vertices[i + 1].position.y);
        }

        // don't include horizontal edges
        if(uint(yMin) == uint(yMax)) continue;

        edges.emplace_back(yMax, yMin, startingX, slopeInv, v1, v2);
    }

    std::sort(edges.begin(),
              edges.end(),
              [](const Edge& e1, const Edge& e2)
              {return uint(e1.yMin) < uint(e2.yMin);});

    return edges;
}


















void rasterizeTriangle(const Triangle& triangle)
{
    // supposed to add only non horizontal edges to table
    std::vector<Edge> edges = createEdgesFromTriangle(triangle);
    std::vector<std::vector<Edge>> edgeList = createEdgeList(edges);

    int yMin = edges[0].yMin, yMax = edges[0].yMax;

    for(auto& edge : edges)
    {
        if(uint(yMax) < uint(edge.yMax)) yMax = edge.yMax;
        if(uint(yMin) > uint(edge.yMin)) yMin = edge.yMin;
    }

    std::vector<Edge> activeEdgeList{};

    for(int scanLine = yMin; scanLine <= yMax; ++scanLine)
    {
        int index = scanLine - yMin;

        // adds new edges to active edge list
        if(!edgeList[index].empty())
        {
            for(auto& edge : edgeList[index])
                activeEdgeList.push_back(edge);
        }

        cleanActiveEdgeList(activeEdgeList, scanLine);
        std::sort(activeEdgeList.begin(),
                  activeEdgeList.end(),
                  [](const Edge& e1, const Edge& e2){return uint(e1.startingX) < uint(e2.startingX);});

        if(activeEdgeList.size() > 1 && scanLine != yMin)
        {
            for(int i = 0; i < (int)activeEdgeList.size(); i += 2)
            {
                for(uint x1 = activeEdgeList[i].startingX + 1,
                    x2 = activeEdgeList[i + 1].startingX;
                    x1 < x2;
                     ++x1)
                     {
                        Edge e1 = activeEdgeList[i];
                        Edge e2 = activeEdgeList[i + 1];
                        float z;
                        Vector3f intensity(0.0f, 0.0f, 0.0f);

                        interpolateVertices(e1, e2, x1, activeEdgeList[i].startingX, activeEdgeList[i + 1].startingX, scanLine, &z, &intensity);

                        u32 r = intensity.r * 255;
                        u32 g = intensity.g * 255;
                        u32 b = intensity.b * 255;

                        Color drawColor = ((r << 24) | (g << 16) | (b << 8) | 0x000000FF);

                        if(z > Window::getInstance()->getFrameBuffer()->getDepth(x1, scanLine))
                        {
                            drawPixel(x1, scanLine, drawColor, false);
                            Window::getInstance()->getFrameBuffer()->setDepth(x1, scanLine, z);                           
                        }
                     }
            }
        }
        
        cleanActiveEdgeList(activeEdgeList, scanLine);

        for(auto& edge : activeEdgeList)
            edge.startingX += edge.slopeInv;
    }
}








