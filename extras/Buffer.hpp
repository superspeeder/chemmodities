#pragma once
#include <type_traits>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <Macros.hpp>


enum VertexDataType {
    Position = 3, Color = 4, Texture = 2, Normal = 3, Custom = 1
};

enum VertBufTargetAction {
    Draw = 0x0, Copy = 0x2, Read = 0x1
};

enum VertBufIOMode {
    Static = 0x4, Dynamic = 0x8, Stream = 0x0
};

enum PrimitiveType {
    Points = 1,
    Lines = 2,
    Triangles = 3,
    Other = 0
};

unsigned int formMode(VertBufTargetAction action, VertBufIOMode iomode);

template <PrimitiveType T>
class IndexBuffer;


template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
class VertexDataBuffer {
private:
    GLuint m_BufferID;
    const GLuint m_Elements;
    std::vector<float> m_BufferData;

    size_t m_StartID = 0;
    size_t m_EndID = 0;


    bool m_Dirty = true;
    bool m_NeedsResize = true;
public:

    VertexDataBuffer();
    VertexDataBuffer(const GLuint elements);

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

    constexpr GLenum getMode()  { return 0x88E0 + static_cast<int>(A) + static_cast<int>(IO); };
    
};

template <PrimitiveType P>
class VertexArray {
private:
    GLuint m_VertexArrayID;
    IndexBuffer<P>* m_IndexBuffer;
public:
    
    VertexArray();

    template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO> void attachBuffer(VertexDataBuffer<T,A,IO>* buffer, unsigned int index) {
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

    size_t m_StartID = 0;
    size_t m_EndID = 0;

    const unsigned int m_PrimitiveSize; // 0 for infinite
    const unsigned int m_Mode;

    friend void VertexArray<T>::bindElementArray(IndexBuffer<T>*);
public:

    IndexBuffer(const unsigned int primitiveSize,  unsigned int mode);
    IndexBuffer(const unsigned int mode);

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

    inline size_t getSize() { return m_BufferData.size(); };
};

/* BEGIN TEMPLATE IMPLEMENTATION*/

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
VertexDataBuffer<T,A,IO>::VertexDataBuffer() : m_Elements{static_cast<int>(T)} {
    std::cout << static_cast<int>(T) << "\n";
    init();
} 

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
VertexDataBuffer<T,A,IO>::VertexDataBuffer(const GLuint elements) {
    static_assert(T == VertexDataType::Custom, "Custom vertex sized require VertexDataType::Custom");
    m_Elements = elements;
    init();
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::init() {
    glGenBuffers(1, &m_BufferID);
    bind();
    pushToBuffer(true);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushToBuffer(bool unbindAfter) {
    if (isDirty()) {
        bind();
        if (m_NeedsResize) {
            std::cout << "Size: " << std::to_string(m_BufferData.size() * sizeof(GLfloat)) << "\n";
            glBufferData(GL_ARRAY_BUFFER,m_BufferData.size() * sizeof(GLfloat), m_BufferData.data(),getMode());
        } else {
            std::cout << "subsize: " << std::to_string(m_StartID * sizeof(GLfloat)) << " " << std::to_string((m_StartID - m_EndID) * sizeof(GLfloat)) << "\n";
            glBufferSubData(GL_ARRAY_BUFFER, m_StartID * sizeof(GLfloat), (m_StartID - m_EndID) * sizeof(GLfloat), (const void*)(&(m_BufferData.at(m_StartID))));
        }
        if (unbindAfter) unbind();

        m_StartID = m_BufferData.size();
        m_EndID = 0;
        m_Dirty = false;
    }
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::markDirty() {
    std::cout << "marked me as dirty\n";
    m_Dirty = true;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
bool VertexDataBuffer<T,A,IO>::isDirty() {
    return m_Dirty;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
constexpr GLuint VertexDataBuffer<T,A,IO>::getElementsPerVertex() {
    return m_Elements;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
GLuint VertexDataBuffer<T,A,IO>::getVertexCount() {
    return getElementCount() / getElementsPerVertex();
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
GLuint VertexDataBuffer<T,A,IO>::getElementCount() {
    return (GLuint)(m_BufferData.size());
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y, float z, float w) {
    runtime_msg_assert(m_Elements == 4, "Buffer doesn't support 4-element vertex data. Size: " + std::to_string(m_Elements));
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
    m_BufferData.push_back(w);

}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y, float z) {
    runtime_msg_assert(m_Elements == 3, "Buffer doesn't support 3-element vertex data. Size: " + std::to_string(m_Elements));
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);

}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y) {
    runtime_msg_assert(m_Elements == 2, "Buffer doesn't support 2-element vertex data. Size: " + std::to_string(m_Elements));
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x) {
    runtime_msg_assert(m_Elements == 1, "Buffer doesn't support 1-element vertex data. Size: " + std::to_string(m_Elements));
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float* vert) {
    runtime_msg_assert(vert != nullptr, "Can't push null vertex data to buffer");
    m_NeedsResize = true;
    markDirty();

    for (unsigned int i = 0 ; i < m_Elements ; i++) {
        m_BufferData.push_back(vert[i]);
    }
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x, float y, float z, float w) {
    runtime_msg_assert(m_Elements == 4, "Buffer doesn't support 4-element vertex data. Size: " + std::to_string(m_Elements));
    if (m_StartID > id*4) m_StartID = id*4;
    if (m_EndID < id*4 + 4) m_EndID = id*4 + 4;
    markDirty();

    m_BufferData[id*4] = x;
    m_BufferData[id*4+1] = y;
    m_BufferData[id*4+2] = z;
    m_BufferData[id*4+3] = w;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x, float y, float z) {
    runtime_msg_assert(m_Elements == 3, "Buffer doesn't support 3-element vertex data. Size: " + std::to_string(m_Elements));

    if (m_StartID > id*3) m_StartID = id*3;
    if (m_EndID < id*3 + 3) m_EndID = id*3 + 3;
    markDirty();

    m_BufferData[id*3] = x;
    m_BufferData[id*3+1] = y;
    m_BufferData[id*3+2] = z;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x, float y) {
    runtime_msg_assert(m_Elements == 2, "Buffer doesn't support 2-element vertex data. Size: " + std::to_string(m_Elements));
    if (m_StartID > id*2) m_StartID = id*2;
    if (m_EndID < id*2 + 2) m_EndID = id*2 + 2;
    markDirty();

    m_BufferData[id*2] = x;
    m_BufferData[id*2+1] = y;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x) {
    runtime_msg_assert(m_Elements == 1, "Buffer doesn't support 1-element vertex data. Size: " + std::to_string(m_Elements));

    if (m_StartID > id) m_StartID = id;
    if (m_EndID < id + 1) m_EndID = id + 1;
    markDirty();

    m_BufferData[id] = x;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editValue(unsigned int id, float value) {
    if (m_StartID > id) m_StartID = id;
    if (m_EndID < id + 1) m_EndID = id + 1;

    m_BufferData[id] = value;
}

// end exlusive
template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editValues(unsigned int start, unsigned int end, float* values) {
    if (m_StartID > start) m_StartID = start;
    if (m_EndID < end) m_EndID = end;
    markDirty();

    if (values == nullptr) {
        m_BufferData.erase(m_BufferData.begin() + start, m_BufferData.begin() + end);
    } else if (end - start == 0) {
        return; 
    } else if (end - start == 1) { 
        m_BufferData[start] = values[0]; // only one value
    } else {
        std::copy(values, values + (end-start), m_BufferData.begin() + start);
    }
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::bind() {
    std::cout << "binded me\n";
    glBindBuffer(GL_ARRAY_BUFFER,m_BufferID);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER,0);
}


template <PrimitiveType P> VertexArray<P>::VertexArray() {
    glCreateVertexArrays(1,&m_VertexArrayID);
}

template <PrimitiveType P> void VertexArray<P>::enableAttribute(unsigned int index) {
    glEnableVertexArrayAttrib(m_VertexArrayID, index);
}

template <PrimitiveType P> void VertexArray<P>::bind() {
    glBindVertexArray(m_VertexArrayID);
    std::cout << "binded me vao\n";
}

template <PrimitiveType P> void VertexArray<P>::unbind() {
    glBindVertexArray(0);
}

template <PrimitiveType P> void VertexArray<P>::bindElementArray(IndexBuffer<P>* buffer) {
    glVertexArrayElementBuffer(m_VertexArrayID, buffer->m_IndexBufferID);
}

template <PrimitiveType T> IndexBuffer<T>::IndexBuffer(const unsigned int primitiveSize,  unsigned int mode) : m_PrimitiveSize{primitiveSize}, m_Mode{mode} {
    static_assert(T == PrimitiveType::Other, "Advanced primitive types need PrimitiveType::Other");
    init();
};
template <PrimitiveType T> IndexBuffer<T>::IndexBuffer(const unsigned int mode) : m_PrimitiveSize{static_cast<int>(T)}, m_Mode{mode} { init(); };


template <PrimitiveType T> void IndexBuffer<T>::init() {
    glGenBuffers(1,&m_IndexBufferID);
    bind();
    pushToBuffer(true);
}

template<PrimitiveType T>
void IndexBuffer<T>::pushToBuffer(bool unbindAfter) {
    if (isDirty()) {
        bind();
        if (m_NeedsResize) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_BufferData.size() * sizeof(GLfloat), m_BufferData.data(),m_Mode);
        } else {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_StartID * sizeof(GLfloat), (m_StartID - m_EndID) * sizeof(GLfloat), (const void*)(&(m_BufferData.at(m_StartID))));
        }
        if (unbindAfter) unbind();

        m_StartID = m_BufferData.size();
        m_EndID = 0;
        m_Dirty = false;
    }
}


template<PrimitiveType T>
void IndexBuffer<T>::markDirty() {
    m_Dirty = true;
}

template<PrimitiveType T>
bool IndexBuffer<T>::isDirty() {
    return m_Dirty;
}

template <PrimitiveType T> void IndexBuffer<T>::bind() {
    std::cout << "binded me ibo\n";
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
}

template <PrimitiveType T> void IndexBuffer<T>::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

template <PrimitiveType T> constexpr unsigned int IndexBuffer<T>::getPrimitiveSize() {
    return m_PrimitiveSize;
} 
template <PrimitiveType T> void IndexBuffer<T>::pushIndex(unsigned int ind) {
    m_BufferData.push_back(ind);
}

template <PrimitiveType T> void IndexBuffer<T>::editIndex(unsigned int where, unsigned int new_index) {
    m_BufferData.at(where) = new_index;
}

template <PrimitiveType T> void IndexBuffer<T>::pushPrimitive(unsigned int* inds) {
    for (size_t p = 0; p < getPrimitiveSize() ; p++) {
        m_BufferData.push_back(inds[p]);
    }
}

template <PrimitiveType T> void IndexBuffer<T>::editPrimitive(unsigned int where, unsigned int* new_prim) {
    for (size_t p = 0; p < getPrimitiveSize() ; p++) {
        m_BufferData.at(where + p) = new_prim[p];
    }
}

template <PrimitiveType T> void IndexBuffer<T>::pushIndicies(unsigned int* inds, unsigned int count) {
    for (size_t i = 0; i < count ; i++) {
        m_BufferData.push_back(inds[i]);
    }
}

template <PrimitiveType T> void IndexBuffer<T>::removeIndex(unsigned int where) {
    m_BufferData.erase(m_BufferData.begin() + where);
}

template <PrimitiveType T> void IndexBuffer<T>::removePrimitive(unsigned int where) {
    m_BufferData.erase(m_BufferData.begin() + where * m_PrimitiveSize, m_BufferData.begin() + where * m_PrimitiveSize + m_PrimitiveSize);
}
