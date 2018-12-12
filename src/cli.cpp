#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>
#include <sstream>
#include <GL/gl.h>


#include "cli.h"
#include "window.h"
#include "scene.h"
#include "frameBuffer.h"
#include "object.h"
#include "camera.h"
#include "curve.h"
#include "bezier.h"
#include "bspline.h"


Cli::Cli(Scene* scene):
m_scene(scene)
{}


std::vector<std::string> Cli::getTokensFromLine(const std::string& line) const
{
    std::vector<std::string> tokens{};

    std::istringstream iss(line);
    std::string token;

    while(std::getline(iss, token, ' '))
        tokens.push_back(token);

    return tokens;
}

void Cli::processInput()
{
    std::string inputLine;
    std::cout << ">> ";
    std::getline(std::cin, inputLine);

    std::vector<std::string> tokens = getTokensFromLine(inputLine);

    std::string command = !tokens.empty() ? tokens[0] : "";
    std::stringstream ss;

    if(command == "load" && tokens.size() == 2)
        cmdLoad(tokens[1]);    

    else if(command == "save" && tokens.size() == 2)
        cmdSave(tokens[1]); 

    else if(command == "displayCurveInfo" && tokens.size() == 1)
        cmdDisplayCurveInfo();

    else if(command == "addPoint" && tokens.size() == 4)
        cmdAddPoint(std::stoi(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "insertPoint" && tokens.size() == 5)
        cmdInsertPoint(std::stoi(tokens[1]), std::stoi(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4]));
    
    else if(command == "deletePoint" && tokens.size() == 3)
        cmdDeletePoint(std::stoi(tokens[1]), std::stoi(tokens[2]));

    else if(command == "modifyPoint" && tokens.size() == 5)
        cmdModifyPoint(std::stoi(tokens[1]), std::stoi(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4]));
    
    else if(command == "setNumSegments" && tokens.size() == 3)
        cmdSetNumSegments(std::stoi(tokens[1]), std::stoi(tokens[2]));

    else if(command == "bezier" && tokens.size() >= 4)
        cmdBezier(tokens);
    

    else if(command == "help" && tokens.size() == 1)
        cmdHelp();

    else if(command == "exit")
        exit(0);

    else if(tokens.size() > 0)
        std::cout << "Invalid command: " << inputLine << std::endl;
}


void Cli::cmdLoad(const std::string fileName)
{
    m_scene->load(fileName);
}


void Cli::cmdSave(const std::string fileName)
{
    m_scene->save(fileName);
}



void Cli::cmdTranslate(int id, float xTrans, float yTrans, float zTrans)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->translate(Vector3f(xTrans, yTrans, zTrans));
            break;
        }
    }
}



void Cli::cmdScale(int id, float xScale, float yScale, float zScale)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->scale(Vector3f(xScale, yScale, zScale));
            break;
        }
    }    
}

void Cli::cmdRotate(int id, float x1, float y1, float z1, float x2, float y2, float z2, float angle)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->rotate(Vector3f(x1, y1, z1), Vector3f(x2, y2, z2), angle);
            break;
        }
    }
}


void Cli::cmdRemove(int id)
{
}


void Cli::cmdDisplayObjects() const
{
    m_scene->displayObjects();
}


void screenToNormalCoords(float* x, float* y)
{
    Window *window = Window::getInstance();
    *x = ((2.0f / window->getWidth())  * (*x)) - 1;
    *y = ((2.0f / window->getHeight()) * (*y)) - 1;
}


void Cli::cmdHelp() const
{
    std::cout << "---------- LIST OF COMMANDS ----------" << std::endl;

    std::cout << "load: 'load fileName'" << std::endl;
    std::cout << "save: 'save fileName'" << std::endl;
    std::cout << "displayCurveInfo: 'displayCurveInfo'" << std::endl;
    std::cout << "addPoint: 'addPoint id x y'" << std::endl;
    std::cout << "insertPoint: 'insertPoint id pointNum x y'" << std::endl;
    std::cout << "deletePoint: 'deletePoint id pointNum'" << std::endl;
    std::cout << "modifyPoint: 'modifyPoint id pointNum x y'" << std::endl;
    std::cout << "setNumSegments: 'setNumSegments id numSegments'" << std::endl;


    std::cout << "exit: 'exit'" << std::endl;

    std::cout << "--------------------------------------" << std::endl;
}


void Cli::cmdSetLightPos(float x, float y, float z)
{
    Vector3f vec(x, y, z);
    m_scene->m_light->m_pos = vec;
}


void Cli::cmdSetLightIntensity(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_sourceIntensity = vec;
}


void Cli::cmdSetLightAmbIntensity(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ambIntensity = vec;
}


void Cli::cmdSetK(float K)
{
    m_scene->m_light->m_K = K;
}


void Cli::cmdSetPhongConstant(float phongConstant)
{
    m_scene->m_light->m_phongConstant = phongConstant;
}


void Cli::cmdSetKA(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ka = vec;
}


void Cli::cmdSetKD(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_kd = vec;
}


void Cli::cmdSetKS(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ks = vec;
}


void Cli::cmdSetFrom(float x, float y, float z)
{
    Vector3f vec(x, y, z);
    m_scene->m_camera->m_pos = vec;
}


void Cli::cmdDisplayVertices(int id) const
{
    m_scene->displayVertices(id);
}




void Cli::cmdDisplayCurveInfo() const
{
    std::cout << "\n***** SCENE CURVE INFORMATION *****\n" << std::endl;

    for(auto& curve : m_scene->m_curves)
    {
        std::cout << "-----------------------------------------" << std::endl;
        curve->print();
        std::cout << "-----------------------------------------\n" << std::endl;

    }
}


void Cli::cmdAddPoint(int id, float x, float y)
{
    for(auto& curve : m_scene->m_curves)
    {
        if(curve->getId() == id)
        {
            Vertex point(x, y, 0);
            curve->addPoint(point);
            break;
        }
    }
}


void Cli::cmdInsertPoint(int id, int pointNum, float x, float y)
{
    for(auto& curve : m_scene->m_curves)
    {
        if(curve->getId() == id)
        {
            Vertex point(x, y, 0);
            curve->insertPoint(pointNum, point);
            break;
        }
    }
}


void Cli::cmdDeletePoint(int id, int pointNum)
{
    for(auto& curve : m_scene->m_curves)
    {
        if(curve->getId() == id)
        {
            curve->deletePoint(pointNum);
            break;
        }
    }
}


void Cli::cmdModifyPoint(int id, int pointNum, float x, float y)
{
    for(auto& curve : m_scene->m_curves)
    {
        if(curve->getId() == id)
        {
            Vertex point(x, y, 0);
            curve->modifyPoint(pointNum, point);
            break;
        }
    }
}


void Cli::cmdSetNumSegments(int id, int numSegments)
{
    for(auto& curve : m_scene->m_curves)
    {
        if(curve->getId() == id)
        {
            curve->setNumSegments(numSegments);
            break;
        }
    }  
}



void Cli::cmdBezier(const std::vector<std::string>& tokens)
{
    int numSegments = std::stoi(tokens[1]);
    std::vector<Vertex> points{};

    for(int i = 2; i < int(tokens.size() - 1); i += 2)
    {
        float x = std::stof(tokens[i]);
        float y = std::stof(tokens[i + 1]);
        points.push_back(Vertex(x, y, 0));
    }

    Curve* curve = new Bezier(points, numSegments);

    m_scene->m_curves.push_back(curve);
}

