#include "SceneLoadSample.h"
#include "SamplesGame.h"
#include "Grid.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Scene Loading", SceneLoadSample, 16);
#endif

static Mesh * createAxis(float size){
    unsigned int vertexCount = 6;
    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);
    
    float vertex[] = {
        0,0,0,    1,0,0,
        size,0,0,  1,0,0,
        0,0,0,      0,1,0,
        0,size,0,   0,1,0,
        0,0,0,  0,0,1,
        0,0,size,   0,0,1
    };
    
    vertices.assign(vertex, vertex+36);
    
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    //
    mesh->setVertexData(&vertices[0], 0, vertexCount);
    return mesh;
    
}

static void drawAxis1(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);
    glPushMatrix();             //NOTE: There is a bug on Mac misbehaviours of
    //      the light position when you draw GL_LINES
    //      and GL_POINTS. remember the matrix.
    
    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);//red
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);//x-
    glColor3f(0, 1, 0);//green
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);//y-
    glColor3f(0, 0, 1);//blue
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);//z-
    glEnd();
    glLineWidth(1);
    
    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);
    
    // restore default settings
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}


SceneLoadSample::SceneLoadSample()
    : _font(NULL), _scene(NULL), _wireFrame(false), _translate(false), _lastY(0), _lastX(0), _lastMouseY(0)
{
    
}

static Mesh* createLinesMesh()
{
    float scale = 0.2f;
    unsigned int vertexCount = 40;
    
    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);
    for (unsigned int i = 0; i < vertexCount; ++i)
    {
        // x, y, z, r, g, b
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_MINUS1_1() * scale);
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1());
        vertices.push_back(MATH_RANDOM_0_1());
    }
    
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh.");
        return NULL;
    }
    mesh->setPrimitiveType(Mesh::LINES);
    //
    mesh->setVertexData(&vertices[0], 0, vertexCount);
    return mesh;
}

static Material* createMaterial()
{
    Material* material = Material::create("res/shaders/colored.vert", "res/shaders/colored.frag", "VERTEX_COLOR");
    GP_ASSERT(material && material->getStateBlock());
    return material;
}

static void loadGrid(Scene* scene)
{
    assert(scene);
    Model* gridModel = createGridModel();
    assert(gridModel);
    gridModel->setMaterial("res/common/grid.material");
    Node* node = scene->addNode("grid");
    node->setDrawable(gridModel);
    SAFE_RELEASE(gridModel);
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
    
    Bundle* bundle = Bundle::create("res/common/desk2.gpb");
    _scene = bundle->loadScene();
    SAFE_RELEASE(bundle);
    
    
    Mesh* axisMesh = createAxis(40);
    _axis = Model::create(axisMesh);
    SAFE_RELEASE(axisMesh);
    Node *axisNode = _scene->addNode("axis");
    axisNode->setDrawable(_axis);
    _axis->setMaterial("res/shaders/colored.vert", "res/shaders/colored.frag", "VERTEX_COLOR");
    
    loadGrid(_scene);


    // Create the camera.
    Camera* camera = Camera::createPerspective(120.0f, getAspectRatio(), 0.1f, 10000.0f);
    Node* cameraNode = _scene->addNode("camera");
    
    // Attach the camera to a node. This determines the position of the camera.
    cameraNode->setCamera(camera);
    
    // Make this the active camera of the scene.
    _scene->setActiveCamera(camera);
    
    //    Node *cameraNode = _scene->getActiveCamera()->getNode();
//    cameraNode->translateY(20000*0.01f);
    cameraNode->translateZ(50.0f);
    
    
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
//    cameraNode->rotate(m);
//    cameraNode->translate(vec);
    
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
    
    SAFE_RELEASE(_axis);
}

void SceneLoadSample::update(float elapsedTime)
{
    
}

void SceneLoadSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);
    Node *cameraNode = _scene->getActiveCamera()->getNode();
    if(_translate){
        cameraNode->translateY(elapsedTime*0.01f);
        cameraNode->translateZ(elapsedTime*0.06f);
    }
    
    //draw camera to target position
    glLineWidth(60);
    unsigned int vertexCount = 2;
    std::vector<float> vertices;
    vertices.reserve(vertexCount * 6);
    Vector3 v = cameraNode->getTranslation();
    
    gameplay::print("-----camera pos:%f,%f,%f\n",v.x,v.y,v.z);
    float vertex[] = {
        v.x,v.y,v.z,    1,0.5,0,
        0,0,0,  1,0.6,0
    };
    
    vertices.assign(vertex, vertex+12);
    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::COLOR, 3)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, false);
    if (mesh != NULL)
    {
        mesh->setPrimitiveType(Mesh::LINES);
        mesh->setVertexData(&vertices[0], 0, vertexCount);
    }
    Model *m = Model::create(mesh);
    m->draw();
    SAFE_RELEASE(m);
    glLineWidth(1);

    
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
            Node *node = _scene->getActiveCamera()->getNode();
//            Node *node = _scene->getFirstNode();//findNode("Group001");
            const Matrix wm = node->getWorldMatrix();
            Matrix rotate;
//            wm.rotateX(MATH_DEG_TO_RAD(45), &dst);
            rotate.rotateX(MATH_DEG_TO_RAD(0.01*(y-_lastY)), &rotate);
            rotate.rotateY(MATH_DEG_TO_RAD(0.01*(x-_lastX)));
            rotate.invert();
            Matrix dst;
            Matrix::multiply(rotate, wm, &dst);
            Quaternion qua;
            dst.getRotation(&qua);
            Vector3 trans;
            dst.getTranslation(&trans);
            node->set(Vector3(1,1,1), qua, trans);
            
            gameplay::print("-----camera:%f,%f,%f\n",trans.x,trans.y,trans.z);
            
            Vector4 p(0,0,0,1);
            p*=dst;
            gameplay::print("-----test:%f,%f,%f\n",p.x,p.y,p.z);
            
//            node->translate(trans);
//            node->rotateX(MATH_DEG_TO_RAD(0.01*(y-_lastY)));
//            node->rotateY(MATH_DEG_TO_RAD(0.01*(x-_lastX)));
//            const Matrix m = node->getWorldMatrix();
//            Matrix dst;
//            m.rotateX(MATH_DEG_TO_RAD(-0.01*(y-_lastY)), &dst);
//            dst.rotateY(MATH_DEG_TO_RAD(-0.01*(x-_lastX)));
//            Vector3 v = node->getTranslationWorld();
            
        break;
    };
}

bool SceneLoadSample::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
        case Mouse::MOUSE_WHEEL:
            Node *node = _scene->getActiveCamera()->getNode();
            //            Node *node = _scene->getFirstNode();//findNode("Group001");
            node->translateForward(wheelDelta);
//            node->translateZ(wheelDelta);
            break;
    };
    _lastMouseY = y;
    return false;
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
