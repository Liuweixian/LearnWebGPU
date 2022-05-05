#pragma once
#include <list>
#include <string>
#include "RenderPass/RenderGraphPass.h"
#include "RenderObject/RenderObject.h"

class RenderGraph
{
public:
    enum Status
    {
        Invalid,
        Initialized,
    };

    RenderGraph();
    virtual ~RenderGraph();
    void Initialize();
    bool Execute();

    template <typename T>
    T* AddRenderPass()
    {
        static_assert(std::is_base_of<RenderGraphPass, T>::value, "T must be a descendant of RenderGraphPass");
        T* pRenderPass = new T();
        m_Passes.push_back(pRenderPass);
        return pRenderPass;
    }

    template <typename T>
    T* AddRenderObject(std::string szName)
    {
        static_assert(std::is_base_of<RenderObject, T>::value, "T must be a descendant of RenderObject");
        T* pRenderObject = new T(szName);
        m_RenderObjects.push_back(pRenderObject);
        return pRenderObject;
    }

protected:
    virtual void InitializeComplete() = 0;

private:
    bool m_eStatus;
    std::list<RenderGraphPass *> m_Passes;
    std::list<RenderObject *> m_RenderObjects;
};