#pragma once
#include <type_traits>
#include <vector>

#include <glad/glad.h>

enum VertexDataType {
    Position, Color, Texture, Normal, Custom
};

enum VertBufTargetAction {
    DRAW = 0x0, COPY = 0x2, READ = 0x1
};

enum VertBufIOMode {
    STATIC = 0x4, DYNAMIC = 0x8, STREAM = 0x0
};

enum PrimitiveType {
    Points = 1,
    Lines = 2,
    Triangles = 3,
    Other = 0
};

template <PrimitiveType T=PrimitiveType::Other>
class IndexBuffer;


template<VertexDataType T,VertBufTargetAction A=VertBufTargetAction::DRAW,VertBufIOMode IO=VertBufIOMode::STATIC>
class VertexDataBuffer {
private:
    GLuint m_BufferID;
    constexpr GLuint m_Elements;
    std::vector<float> m_BufferData;

    unsigned int m_StartID = 0;
    unsigned int m_EndID = 0;


    bool m_Dirty = true;
    bool m_NeedsResize = true;
public:

    template<VertBufTargetAction A,VertBufIOMode IO> VertexDataBuffer<VertexDataType::Position,A,IO>() : m_Elements{3} { init(); };
    template<VertBufTargetAction A,VertBufIOMode IO> VertexDataBuffer<VertexDataType::Color,A,IO>() : m_Elements{4} { init(); };
    template<VertBufTargetAction A,VertBufIOMode IO> VertexDataBuffer<VertexDataType::Texture,A,IO>() : m_Elements{2} { init(); };
    template<VertBufTargetAction A,VertBufIOMode IO> VertexDataBuffer<VertexDataType::Normal,A,IO>() : m_Elements{3} { init(); };
    template<VertBufTargetAction A,VertBufIOMode IO> VertexDataBuffer<VertexDataType::Custom,A,IO>(GLuint elements) : m_Elements{elements} { init(); };

    void init();
    void pushToBuffer(bool unbindAfter);

    void markDirty();
    bool isDirty();

    constexpr GLuint getElementsPerVertex();
    GLuint getVertexCount();

    GLuint getElementCount();

    void pushVertex(float x, float y, float z, float w);
    void pushVertex(float x, float y, float z);
    void pushVertex(float x, float y);
    void pushVertex(float x);
    void pushVertex(float* vert);

    void editVertex(unsigned int id, float x, float y, float z, float w);
    void editVertex(unsigned int id, float x, float y, float z);
    void editVertex(unsigned int id, float x, float y);
    void editVertex(unsigned int id, float x);
    
    void editValue(unsigned int id, float value);
    void editValues(unsigned int start, unsigned int end, float* values);

    void bind();
    void unbind();

    constexpr GLenum getMode()  { 0x88E0 + static_cast<int>(A) + static_cast<int>(IO); };
    
};

template <PrimitiveType P=PrimitiveType::Other>
class VertexArray {
private:
    GLuint m_VertexArrayID;
    IndexBuffer<P>* m_IndexBuffer;
public:
    
    VertexArray();

    template<VertexDataType T,VertBufTargetAction A=VertBufTargetAction::DRAW,VertBufIOMode IO=VertBufIOMode::STATIC> void attachBuffer(VertexDataBuffer<T,A,IO>* buffer, unsigned int index) {
        bind();
        buffer->bind();
        glVertexAttribPointer(index,buffer->getElementsPerVertex(),GL_FLOAT,GL_FALSE,buffer->getElementCount() * sizeof(GLfloat),NULL);
        unbind();
        buffer->unbind();
    };

    void enableAttribute(unsigned int index);
    void bind();
    void unbind();

    void bindElementArray(IndexBuffer<P>* buffer);
};

template <PrimitiveType T>
class IndexBuffer {
private:
    GLuint m_IndexBufferID;
    std::vector<unsigned int> m_BufferData;

    bool m_Dirty = true;
    bool m_NeedsResize = true;

    unsigned int m_StartID = 0;
    unsigned int m_EndID = 0;

    constexpr unsigned int m_PrimitiveSize; // 0 for infinite
    constexpr unsigned int m_Mode;

    friend void VertexArray<T>::bindElementArray(IndexBuffer<T>*);
public:

    IndexBuffer<PrimitiveType::Other>(constexpr unsigned int primitiveSize, constexpr unsigned int mode) : m_PrimitiveSize{primitiveSize}, m_Mode{mode} { init(); };
    template <PrimitiveType T> IndexBuffer<T>(constexpr unsigned int mode) : m_PrimitiveSize(static_cast<int>(T)) : m_Mode{mode} { init(); };

    void init();
    void pushToBuffer(bool unbindAfter);

    void markDirty();
    bool isDirty();

    void bind();
    void unbind();

    constexpr unsigned int getPrimitiveSize(); 

    void pushIndex(unsigned int ind);

    void editIndex(unsigned int where, unsigned int new_index);

    void pushPrimitive(unsigned int* inds);
    void editPrimitive(unsigned int where, unsigned int* new_prim);

    void pushIndicies(unsigned int* inds, unsigned int count);

    void removeIndex(unsigned int where);
    void removePrimitive(unsigned int where);
};