#ifndef RGBA_STORAGE_H
#define RGBA_STORAGE_H

#include <memory>
#include <SDL.h>
#include <GL/gl.h>

class RGBAStorage {
public:
    virtual ~RGBAStorage() = default;
    virtual uint8_t* getData() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getPitch() const = 0;
};

class SDLSurfaceStorage : public RGBAStorage {
public:
    SDLSurfaceStorage(SDL_Surface* surface) : surface_(surface) {}
    ~SDLSurfaceStorage() {
        if (surface_) {
            SDL_FreeSurface(surface_);
        }
    }

    uint8_t* getData() override {
        return static_cast<uint8_t*>(surface_->pixels);
    }

    int getWidth() const override {
        return surface_->w;
    }

    int getHeight() const override {
        return surface_->h;
    }

    int getPitch() const override {
        return surface_->pitch;
    }

private:
    SDL_Surface* surface_;
};

class OpenGLTextureStorage : public RGBAStorage {
public:
    OpenGLTextureStorage(GLuint texture, int width, int height)
        : texture_(texture), width_(width), height_(height) {}

    ~OpenGLTextureStorage() {
        if (texture_) {
            glDeleteTextures(1, &texture_);
        }
    }

    uint8_t* getData() override {
        // OpenGL textures do not provide direct access to pixel data
        return nullptr;
    }

    int getWidth() const override {
        return width_;
    }

    int getHeight() const override {
        return height_;
    }

    int getPitch() const override {
        // OpenGL textures do not have a pitch value
        return 0;
    }

private:
    GLuint texture_;
    int width_;
    int height_;
};

#endif // RGBA_STORAGE_H
