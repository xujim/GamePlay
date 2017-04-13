#include "Base.h"
#include "Pass.h"
#include "Technique.h"
#include "Material.h"
#include "Node.h"

namespace gameplay
{

Pass::Pass(const char* id, Technique* technique) :
    _id(id ? id : ""), _technique(technique), _effect(NULL), _vaBinding(NULL)
{
    RenderState::_parent = _technique;
}

Pass::~Pass()
{
    SAFE_RELEASE(_effect);
    SAFE_RELEASE(_vaBinding);
}

bool Pass::initialize(const char* vshPath, const char* fshPath, const char* defines)
{
    GP_ASSERT(vshPath);
    GP_ASSERT(fshPath);

    SAFE_RELEASE(_effect);
    SAFE_RELEASE(_vaBinding);

    // Attempt to create/load the effect.
    //Effect类似与glsl的program，代表着shader程序
    _effect = Effect::createFromFile(vshPath, fshPath, defines);
    if (_effect == NULL)
    {
        GP_WARN("Failed to create effect for pass. vertexShader = %s, fragmentShader = %s, defines = %s", vshPath, fshPath, defines ? defines : "");
        return false;
    }

    return true;
}

const char* Pass::getId() const
{
    return _id.c_str();
}

Effect* Pass::getEffect() const
{
    return _effect;
}

void Pass::setVertexAttributeBinding(VertexAttributeBinding* binding)
{
    SAFE_RELEASE(_vaBinding);

    if (binding)
    {
        _vaBinding = binding;
        binding->addRef();
    }
}

VertexAttributeBinding* Pass::getVertexAttributeBinding() const
{
    return _vaBinding;
}

void Pass::bind()
{
    GP_ASSERT(_effect);

    // Bind our effect.
    _effect->bind();//使用当前program

    //注意：下一行是关键，用于将当前RenderState中的参数（glsl相关参数，如mvp matrix等）绑定到当前pass的effect（即glsl）中
    // Bind our render state
    //此处调用父类实现，因为父类的bind不显示调用，会被隐藏
    RenderState::bind(this);

    //绑定节点属性，如坐标，颜色，法向量，纹理等等
    // If we have a vertex attribute binding, bind it
    if (_vaBinding)
    {
        _vaBinding->bind();
    }
}

void Pass::unbind()
{
    // If we have a vertex attribute binding, unbind it
    if (_vaBinding)
    {
        _vaBinding->unbind();
    }
}

Pass* Pass::clone(Technique* technique, NodeCloneContext &context) const
{
    GP_ASSERT(_effect);
    _effect->addRef();

    Pass* pass = new Pass(getId(), technique);
    pass->_effect = _effect;

    RenderState::cloneInto(pass, context);
    pass->_parent = technique;
    return pass;
}

}
