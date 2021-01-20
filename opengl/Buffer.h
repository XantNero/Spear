#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;
    unsigned int size;
};

class VertexBuffer
{
private:
    unsigned int m_RendererID;
    int m_Offset;
    unsigned int m_MaxSize;
public:
    VertexBuffer(unsigned int size, const void* data = nullptr);
    ~VertexBuffer();
    void addData(unsigned int size, const void* data);
    void resetData();

    void bind() const;
    void unbind() const;
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout();
    ~VertexBufferLayout();
    void push(unsigned int gltype, unsigned int count, unsigned int size);
    unsigned int getStride() const;
    const std::vector<VertexBufferElement>& getElementArray() const;
};

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
    unsigned int m_MaxCount;
public:
    IndexBuffer(unsigned int count, unsigned int* data = nullptr);
    ~IndexBuffer();
    void addData(unsigned int count, unsigned int* data);
    void ResetData();

    void bind() const;
    void unbind() const;
    unsigned int getCount() const;
};

#endif