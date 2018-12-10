#include <iostream>
#include <cstring>

#include "frameBuffer.h"

FrameBuffer::FrameBuffer(uint width, uint height):
m_width(width), m_height(height)
{
    std::cout << "Creating frame buffer..." << std::endl;
    m_buffer = new u32[width * height];
    m_depthBuffer = new float[width * height];
    memset(m_buffer, 0, sizeof(u32) * width * height);   
    memset(m_buffer, 0xFFFFFFFF, sizeof(float) * width * height); 

    m_viewportX = 0;
    m_viewportY = 0;
    m_viewportWidth  = width;
    m_viewportHeight = height; 
}


FrameBuffer::~FrameBuffer()
{
    if(m_buffer)
        delete [] m_buffer;
}


void FrameBuffer::setPixel(uint x, uint y, u32 data)
{
    /* This method of indexing results in
     * coordinate (0,0) being in the lower left hand
     * corner of the screen with x increasing from left
     * to right and y increasing from bottom to top
     */
    uint index = y * m_width + x;
    // uint start = m_viewportY * m_width + m_viewportX;
    // uint index = start + (y * m_viewportWidth) + x;
    m_buffer[index] = data;
}


void FrameBuffer::setDepth(uint x, uint y, float depth)
{
    uint index = y * m_width + x;
    m_depthBuffer[index] = depth;
}


float FrameBuffer::getDepth(uint x, uint y)
{
    return m_depthBuffer[y * m_width + x];
}


void FrameBuffer::clear(Color color)
{
    for(uint i = 0; i < m_width * m_height; ++i)
    {
        m_buffer[i] = color.color;
        m_depthBuffer[i] = -2000000000;  // infinity
    }
}


void FrameBuffer::resizeBuffer(uint width, uint height)
{
    m_width = width;
    m_height = height;
    delete [] m_buffer;
    m_buffer = new u32[m_width * m_height];
    clear(0);
}



void FrameBuffer::setViewport(uint x, uint y, uint w, uint h)
{
    m_viewportX      = x;
    m_viewportY      = y;
    m_viewportWidth  = w;
    m_viewportHeight = h;
}