#ifndef _VERTEXARRAY_H_
#define _VERTEXARRAY_H_
#include "Buffer.h"
#include <vector>
#include <memory>
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;
    void addVertexBuffer(const std::shared_ptr<VertexBuffer> vb, const VertexBufferLayout& layout);
    void setIndexBuffer(const std::shared_ptr<IndexBuffer> ib);
    const std::shared_ptr<IndexBuffer>& getIndexBuffer() const;
    const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBufferArray() const;
private:
    unsigned int m_RendererID;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

#endif