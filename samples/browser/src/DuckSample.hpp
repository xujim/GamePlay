//
//  DuckSample.hpp
//  sample-browser
//
//  Created by yujie on 2017/3/27.
//
//

#ifndef DuckSample_hpp
#define DuckSample_hpp

#include <stdio.h>
#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample creating a water effect.
 */
class DuckSample : public Sample
{
public:
    
    DuckSample();
    
//    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
//    
//    void keyEvent(Keyboard::KeyEvent evt, int key);
//    
//    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
//    
//    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);
    
protected:
    
    void initialize();
    
    void finalize();
    
    void update(float elapsedTime);
    
    void render(float elapsedTime);
    bool drawScene(Node* node, bool drawWater);
    
private:
    Scene* _scene;
    Node* _cameraNode;
    Node* _duckNode;
};
#endif /* DuckSample_hpp */
