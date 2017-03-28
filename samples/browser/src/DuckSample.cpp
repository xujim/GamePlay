//
//  DuckSample.cpp
//  sample-browser
//
//  Created by yujie on 2017/3/27.
//
//

#include "DuckSample.hpp"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
ADD_SAMPLE("Graphics", "Duck", DuckSample, 14);
#endif

DuckSample::DuckSample():_scene(NULL), _cameraNode(NULL){
    
}

void DuckSample::initialize(){
    Bundle* bundle = Bundle::create("res/common/duck.gpb");
    _scene = bundle->loadScene();
    SAFE_RELEASE(bundle);
    
    
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    
    // Get handles to the nodes of interest in the scene
    _duckNode = _scene->findNode("duck");
//    _lightNode = _scene->findNode("directionalLight1");
    _cameraNode = _scene->findNode("camera1");
    
    Model* model = dynamic_cast<Model*>(_duckNode->getDrawable());
    Material* material = model->setMaterial("res/common/duck.material");
    // Get light node
    Node* lightNode = _scene->findNode("directionalLight1");
    Light* light = lightNode->getLight();
    material->getParameter("u_directionalLightColor[0]")->setValue(light->getColor());
    material->getParameter("u_directionalLightDirection[0]")->setValue(lightNode->getForwardVectorView());
}

void DuckSample::update(float elapsedTime)
{
    // Rotate the model
//    if (!_touched){
        _duckNode->rotateY(elapsedTime * MATH_DEG_TO_RAD(0.05f));
//}
}

void DuckSample::render(float elapsedTime)
{
    // Clear the buffers to black
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    
    // Visit all the nodes in the scene, drawing the models/mesh.
//    _scene->visit(this, &DuckSample::drawScene);
    _scene->visit(this, &DuckSample::drawScene, true);
}

bool DuckSample::drawScene(Node* node, bool drawWater)
{
    // This method is called for each node in the scene.
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    return true;
}

void DuckSample::finalize()
{
//    setMouseCaptured(false);
    SAFE_RELEASE(_duckNode);
    SAFE_RELEASE(_cameraNode);
    SAFE_RELEASE(_scene);
}
