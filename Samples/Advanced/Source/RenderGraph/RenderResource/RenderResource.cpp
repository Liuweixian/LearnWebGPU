#include "RenderResource.h"

RenderResource::RenderResource()
{
    m_AllTextureDescs.clear();
}

RenderResource::~RenderResource()
{

}
 /*
{
 hash = 0
for num in array:
    hash = hash * 31 + num
return hash
}*/

static RenderResource *g_pRenderResource = nullptr;
RenderResource *GetRenderResource()
{
    if (g_pRenderResource == nullptr)
        g_pRenderResource = new RenderResource();
    return g_pRenderResource;
}