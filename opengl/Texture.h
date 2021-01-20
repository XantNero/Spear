#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::string &filePath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    int getWidth() const;
    int getHeight() const;
};

#endif