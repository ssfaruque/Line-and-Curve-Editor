#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>

#include "types.h"


/*
 * Converts normalized device coordinates
 * into screen coordinates
 * for the main window
 */
void normalToScreenCoords(float* x, float* y);


/*
 * Converts normalized device coordinates
 * into screen coordinates for the projection window
 */
void normalToScreenCoords(float* x, float* y, int width, int height);


/*
 * Converts clipped coordinates
 * into normalized device coordinates
 */
void clipToNormalCoords(float* x, float* y, float xmin, float xmax, float ymin, float ymax); 


void worldToNormalCoords(float* x, float* y, float* z, const BoundingBox& boundingBox); 


/*
 * Draws a single pixel to the frame buffer
 */
void drawPixel(float x, float y, Color color, bool normalized = false);


/*
 * Draws a line to the frame buffer given starting and ending coordinates
 */
void drawLine(uint x1, uint y1, uint x2, uint y2, Color color, bool drawWithBresenham);


/*
 * Implementation of DDA algorithm
 */
void dda(float x1, float y1, float x2, float y2, Color color);


/*
 * Implementation of Bresenham algorithm
 */
void bresenham(float x1, float y1, float x2, float y2, Color color);


/*
 * Implementation of rasterization
 */
void rasterize(const std::vector<Vertex>& vertices, const std::vector<int> indices, Color color);



void rasterizeTriangle(const Triangle& triangle);



/*
 * Implementation of Cohen-Sutherland line clipping algorithm
 */
void cohenSutherlandClipping(Vertex* v1, Vertex* v2, float xmin, float xmax, float ymin, float ymax);


/*
 * Implementation of Sutherland-Hodgman line clipping algorithm
 */
std::vector<Vertex> sutherlandHodgmanClipping(const std::vector<Vertex>& vertices, const std::vector<Vertex>& clippingWindow);



void enableHalfToning();
void disableHalfToning();

void rasterizeWithHalfToning(const Triangle& triangle, int intensityLevel);


#endif  // GRAPHICS_H