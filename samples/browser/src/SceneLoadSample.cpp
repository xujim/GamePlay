#include "SceneLoadSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Scene Loading", SceneLoadSample, 16);
#endif

SceneLoadSample::SceneLoadSample()
    : _font(NULL), _scene(NULL), _wireFrame(false), _translate(false), _lastY(0), _lastX(0)
{
    
}

static Material* createMaterial()
{
    Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", "VERTEX_COLOR");
    GP_ASSERT(material && material->getStateBlock());
    return material;
}

void SceneLoadSample::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

//    _scene = Scene::load("res/common/sample.scene");

    // Update the aspect ratio for our scene's camera to match the current device resolution
//    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());

    // Visit all the nodes in the scene, drawing the models/mesh.
//    _scene->visit(this, &SceneLoadSample::initializeMaterials);
    
    Bundle* bundle = Bundle::create("res/common/shafa3.gpb");
    _scene = bundle->loadScene();
    SAFE_RELEASE(bundle);

    // Create the camera.
    Camera* camera = Camera::createPerspective(120.0f, getAspectRatio(), 0.1f, 10000.0f);
    Node* cameraNode = _scene->addNode("camera");
    
    // Attach the camera to a node. This determines the position of the camera.
    cameraNode->setCamera(camera);
    
    // Make this the active camera of the scene.
    _scene->setActiveCamera(camera);
    
    //    Node *cameraNode = _scene->getActiveCamera()->getNode();
//    cameraNode->translateY(20000*0.01f);
//    cameraNode->translateZ(1200.0f);
    
    
    // Rotate the node x/z to face the camera
//    Camera *camera = _scene->getActiveCamera();
    Node *node = _scene->getFirstNode();//findNode("Group001");
    Matrix m;
//    Matrix::createBillboard(node->getTranslationWorld(), camera->getNode()->getTranslationWorld(), camera->getNode()->getUpVectorWorld(), &m);
//    Quaternion q;
//    m.getRotation(&q);
//    node->setRotation(q);
    
    const BoundingSphere &bs = node->getBoundingSphere();
    Vector3 unitV;
    bs.center.normalize(&unitV);
    Vector3 forwardV = bs.center;
    Vector3 leftV(1,0,0);
    Vector3 upV;
    Vector3::cross(forwardV, leftV, &upV);
    Vector3 vec(bs.center-unitV*bs.radius);
//    cameraNode->setTranslation(vec);
    Matrix::createLookAt(Vector3(0,0,0), bs.center, upV, &m);
    cameraNode->rotate(m);
    cameraNode->translate(vec);
    
//    cameraNode->rotate(m);
//    cameraNode->getUpVectorWorld()
    
    SAFE_RELEASE(camera);
    
    Material* material = NULL;//createMaterial();
//    material = Material::create("res/common/shafa3.material");
    Camera *c = _scene->getActiveCamera();
    Node *mynode = NULL;
    while((mynode = _scene->getNext())){
        const char * id = mynode->getId();
        gameplay::print("--%s\n",id);
        
//        Node *group001 = _scene->findNode("Group001");
//        if (material)
//        {
//            Model *model = dynamic_cast<Model*>(mynode->getDrawable());
//            //        int partIndex = model->getMesh()->getPartCount() > 0 ? i : -1;
//            if(model){
//                model->setMaterial(material);
//                gameplay::print("!!!mater---%s\n",id);
//            }
//        }
    }
    
    SAFE_RELEASE(material);
}


bool SceneLoadSample::initializeMaterials(Node* node)
{
    Model* model = dynamic_cast<Model*>(node->getDrawable());
    if (model)
    {
        Material* material = model->getMaterial();
        // For this sample we will only bind a single light to each object in the scene.
        MaterialParameter* colorParam = material->getParameter("u_directionalLightColor[0]");
        colorParam->setValue(Vector3(0.75f, 0.75f, 0.75f));
        MaterialParameter* directionParam = material->getParameter("u_directionalLightDirection[0]");
        directionParam->setValue(Vector3(0, -1, 0));
    }
    return true;
}

void SceneLoadSample::finalize()
{
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_scene);
}

void SceneLoadSample::update(float elapsedTime)
{
    
}

void SceneLoadSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    if(_translate){
        Node *cameraNode = _scene->getActiveCamera()->getNode();
        cameraNode->translateY(elapsedTime*0.01f);
        cameraNode->translateZ(elapsedTime*0.06f);
    }
    // Visit all the nodes in the scene, drawing the models/mesh.
    _scene->visit(this, &SceneLoadSample::drawScene);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void SceneLoadSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
            _lastY = y;
            _lastX = x;
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
            return;
        }
            _translate = false;
        break;
    case Touch::TOUCH_RELEASE:
            _translate = false;
        break;
    case Touch::TOUCH_MOVE:
//            Node *node = _scene->getActiveCamera()->getNode();
            Node *node = _scene->getFirstNode();//findNode("Group001");
            node->rotateX(MATH_DEG_TO_RAD(0.01*(y-_lastY)));
            node->rotateY(MATH_DEG_TO_RAD(0.01*(x-_lastX)));
        break;
    };
}

void SceneLoadSample::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
        case Keyboard::KEY_CAPITAL_W:
            _wireFrame = !_wireFrame;
            break;
        }
    }
}

bool SceneLoadSample::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    
    
    if (drawable)
        drawable->draw(_wireFrame);
    return true;
}
