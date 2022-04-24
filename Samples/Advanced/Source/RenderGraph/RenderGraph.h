#pragma once

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
    virtual bool Execute();
protected:
    virtual void InitializeComplete() = 0;
private:
    bool m_eStatus;
};