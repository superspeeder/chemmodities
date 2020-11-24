#define CHE_CORE_DEBUG
#include <All.hpp>


class Game : public che::Application {
private:
    che::VertexBuffer *vboPosition, *vboColor, *vboUV, *vboFBPos, *vboFBColor, *vboFBUV;
    che::IndexBuffer *iboFB, *ibo;
    che::VertexArray *vao, *vaoFB;

    che::VertexAttribute positionAttr;
    che::VertexAttribute colorAttr;
    che::VertexAttribute uvAttr;

    che::VertexAttribute fb_posAttr;
    che::VertexAttribute fb_uvAttr;
    che::VertexAttribute fb_colorAttr;

    che::ShaderProgram *program, *screenProgram;

    che::Texture *tex, *fbTex;

    che::Framebuffer* fbuffer;

public:

    Game();

    virtual void create() override;
    virtual void render(double dt) override;
    virtual void destroy() override;
};