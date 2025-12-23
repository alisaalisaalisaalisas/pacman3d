#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture();
    ~Texture();
    
    bool load(const std::string& filepath);
    void bind(unsigned int slot = 0) const;
    void unbind() const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    unsigned int getID() const { return textureID; }
    
private:
    unsigned int textureID;
    int width;
    int height;
    int channels;
};

#endif // TEXTURE_H
