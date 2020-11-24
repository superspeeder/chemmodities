#define CHE_CORE_DEBUG
#include <All.hpp>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

class Game : public che::Application {
private:

    che::SpriteBatch *spriteBatch;

    che::ShaderProgram *program, *screenProgram;

    che::Texture *tex, *fbTex;

    che::Framebuffer* fbuffer;

    che::RenderableFrameBuffer* rframeb;
public:

    Game();

    virtual void create() override;
    virtual void render(double dt) override;
    virtual void destroy() override;
};