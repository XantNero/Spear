#include "../pch.h"
#include "Buffer.h"
#include "debug.h"

/*
----------------------------------------------------------------------
|||||||||||||||||||||||VertexBuffer|||||||||||||||||||||||||||||||||||
----------------------------------------------------------------------
*/

VertexBuffer::VertexBuffer(unsigned int size, const void* data) 
    :m_Offset(0), m_MaxSize(size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, data == 0 ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::addData(unsigned int size, const void* data)
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, m_Offset, size, data));
    m_Offset += size;
}

void VertexBuffer::resetData()
{
    m_Offset = 0;
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

/*
----------------------------------------------------------------------
|||||||||||||||||||||||VertexBufferLayout|||||||||||||||||||||||||||||
----------------------------------------------------------------------
*/

VertexBufferLayout::VertexBufferLayout()
: m_Elements(), m_Stride(0) { }

VertexBufferLayout::~VertexBufferLayout() { }

void VertexBufferLayout::push(unsigned int gltype, unsigned int count, unsigned int size)
{
    m_Elements.push_back({gltype, count, false, size});
    m_Stride += size;
}
unsigned int VertexBufferLayout::getStride() const
{
    return m_Stride;
}
const std::vector<VertexBufferElement>& VertexBufferLayout::getElementArray() const
{
    return m_Elements;
}

/*
----------------------------------------------------------------------
|||||||||||||||||||||||IndexBuffer||||||||||||||||||||||||||||||||||||
----------------------------------------------------------------------
*/

IndexBuffer::IndexBuffer(unsigned int count, unsigned int* data)
    : m_Count(0), m_MaxCount(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), (void*)data,
        data == 0 ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    if (data != 0)
        m_Count = count;
}
IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::addData(unsigned int count, unsigned int* data)
{
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int),
        count * sizeof(unsigned int), data));
    m_Count += count;
}

void IndexBuffer::ResetData()
{
    // GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
    //     m_Count * sizeof(unsigned int), 0));
    m_Count = 0;
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::getCount() const
{
    return m_Count;
}