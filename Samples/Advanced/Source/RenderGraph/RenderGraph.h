#pragma once
#include <list>
#include <string>
#include "RGDefine.h"
#include "RenderPass/RGPass.h"
#include "RenderObject/RenderObject.h"

class RenderGraph
{
public:
    enum Status
    {
        Invalid,
        Initialized,
        Compiled,
    };

    RenderGraph();
    virtual ~RenderGraph();
    bool Execute(const std::list<RenderObject *> renderObjects);

    template <typename T>
    T *AddRenderGraphPass(RGPassIdx uPassIdx)
    {
        static_assert(std::is_base_of<RGPass, T>::value, "T must be a descendant of RenderGraphPass");
        T *pRenderPass = new T(uPassIdx);
        m_Passes.push_back(pRenderPass);
        return pRenderPass;
    }

private:
    void Compile();

protected:
    virtual void Initialize() = 0;

private:
    Status m_eStatus;
    std::list<RGPass *> m_Passes;
};
