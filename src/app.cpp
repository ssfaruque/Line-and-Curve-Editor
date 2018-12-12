#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <string>

#include "app.h"
#include "window.h"
#include "frameBuffer.h"
#include "types.h"
#include "graphics.h"
#include "scene.h"
#include "math.h"
#include "cli.h"
#include "object.h"



App* App::m_app = nullptr;


void renderScene();
void showScreen();
void renderProjections();

App::App(const char* title, const int width, const int height,
            int* argc, char** argv):
m_running(true)
{
    Window::create(width, height, title, argc, argv);
    glutDisplayFunc(renderScene);
    m_window = Window::getInstance();
    m_scene = new Scene;
    m_cli = new Cli(m_scene);
}


App::~App()
{
    if(m_window)
        Window::destroy();

    if(m_scene)
        delete m_scene;

    if(m_cli)
        delete m_cli;
}


void App::create(const char* title, const int width, const int height,
                 int* argc, char** argv)
{
    if(!App::m_app)
        m_app = new App(title, width, height, argc, argv);
}


App* App::getInstance()
{
    return m_app;
}


void App::run()
{
    std::cout << "Creating application..." << std::endl << std::endl;
    glutMainLoop();
}


void renderScene()
{
    Window* window = Window::getInstance();
    window->clear();

    App* app = App::getInstance();
    app->m_cli->processInput();
    app->m_scene->draw();

    showScreen();
    glutPostRedisplay();
}


void showScreen()
{
    // Window* window = Window::getInstance();

    // glDrawPixels(window->getFrameBuffer()->getWidth(),
    //              window->getFrameBuffer()->getHeight(),
    //              GL_RGBA,
    //              GL_UNSIGNED_INT_8_8_8_8,
    //              window->getFrameBuffer()->getBuffer());

    glutSwapBuffers();    
}



