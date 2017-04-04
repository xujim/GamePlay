#ifndef SCENELOADSAMPLE_H_
#define SCENELOADSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample for loading a scene from a .scene file.
 */
class SceneLoadSample : public Sample
{
public:

    SceneLoadSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool initializeMaterials(Node* node);

    bool drawScene(Node* node);

    Font* _font;
    Scene* _scene;
    Model* _axis;
    bool _wireFrame;
    bool _translate;
    float _lastY, _lastX, _lastMouseY;
};

#endif
