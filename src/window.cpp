#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>

#include "window.h"
#include "frameBuffer.h"
#include "graphics.h"


Window* Window::m_window = nullptr;


Window::Window(uint width, uint height, std::string title, int* argc, char** argv):
m_width(width), m_height(height), m_title(title)
{
    std::cout << "Creating window..." << std::endl;

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
    glutInitWindowSize(width, height);
    glutInitWindowPosition(-1, -1);
    glutCreateWindow(title.c_str());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, width, height);

    m_frameBuffer = new FrameBuffer(width, height);
}


Window::~Window()
{
    if(m_frameBuffer)
        delete m_frameBuffer;
}


void Window::create(uint width, uint height, std::string title, int* argc, char** argv)
{
    if(!Window::m_window)
        Window::m_window = new Window(width, height, title, argc, argv);
}


Window* Window::getInstance()
{
    if(Window::m_window)
        return Window::m_window;

    return nullptr;
}


void Window::destroy()
{
    if(Window::m_window)
    {
        delete Window::m_window;
        Window::m_window = nullptr;
    }
}


void Window::clear(Color color)
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_frameBuffer->clear(color);
}


void Window::resizeFrameBuffer(uint width, uint height)
{
    m_frameBuffer->resizeBuffer(width, height);
}