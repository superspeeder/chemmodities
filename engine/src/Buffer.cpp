#include "Buffer.hpp"
#include <algorithm>

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::init() {
    glGenBuffers(1, &m_BufferID);
    bind();
    pushToBuffer();
    unbind();
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushToBuffer(bool unbindAfter) {
    if (isDirty()) {
        bind();
        if (m_NeedsResize) {
            glBufferData(GL_ARRAY_BUFFER,m_BufferData.size() * sizeof(GLfloat), m_BufferData.data(),getMode());
        } else {
            glBufferSubData(GL_ARRAY_BUFFER,m_StartID * sizeof(GLfloat), (m_StartID - m_EndID) * sizeof(GLfloat), *(m_BufferData.at(m_StartID));
        }
        if (unbindAfter) unbind();

        m_StartID = m_BufferData.size();
        m_EndID = 0;
        m_Dirty = false;
    }
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::markDirty() {
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
    return m_BufferData.size();
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y, float z, float w) {
    static_assert(m_Elements == 4, "Buffer doesn't support 4-element vertex data");
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
    m_BufferData.push_back(w);

}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y, float z) {
    static_assert(m_Elements == 3, "Buffer doesn't support 3-element vertex data");
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);

}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x, float y) {
    static_assert(m_Elements == 2, "Buffer doesn't support 2-element vertex data");
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float x) {
    static_assert(m_Elements == 1, "Buffer doesn't support 1-element vertex data");
    m_NeedsResize = true;
    markDirty();
    m_BufferData.push_back(x);
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::pushVertex(float* vert) {
    static_assert(vert != nullptr, "Can't push null vertex data to buffer");
    m_NeedsResize = true;
    markDirty();

    for (unsigned int i = 0 ; i < m_Elements ; i++) {
        m_BufferData.push_back(vert[i]);
    }
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x, float y, float z, float w) {
    static_assert(m_Elements == 4, "Buffer doesn't support 4-element vertex data");
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
    static_assert(m_Elements == 3, "Buffer doesn't support 3-element vertex data");

    if (m_StartID > id*3) m_StartID = id*3;
    if (m_EndID < id*3 + 3) m_EndID = id*3 + 3;
    markDirty();

    m_BufferData[id*3] = x;
    m_BufferData[id*3+1] = y;
    m_BufferData[id*3+2] = z;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x, float y) {
    static_assert(m_Elements == 2, "Buffer doesn't support 2-element vertex data");
    if (m_StartID > id*2) m_StartID = id*2;
    if (m_EndID < id*2 + 2) m_EndID = id*2 + 2;
    markDirty();

    m_BufferData[id*2] = x;
    m_BufferData[id*2+1] = y;
}

template<VertexDataType T,VertBufTargetAction A,VertBufIOMode IO>
void VertexDataBuffer<T,A,IO>::editVertex(unsigned int id, float x) {
    static_assert(m_Elements == 1, "Buffer doesn't support 1-element vertex data");

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
        m_BufferData.erase(m_BufferData.begin() + start, m_BufferData.back() + end);
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
}

template <PrimitiveType P> void VertexArray<P>::unbind() {
    glBindVertexArray(0);
}

template <PrimitiveType P> void VertexArray<P>::bindElementArray(IndexBuffer<P>* buffer) {
    glVertexArrayElementBuffer(m_VertexArrayID, buffer->m_IndexBufferID);
}

template <PrimitiveType T> void IndexBuffer<T>::init() {
    glGenBuffers(1,&m_IndexBufferID);
}

template <PrimitiveType T> void IndexBuffer<T>::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
}

template <PrimitiveType T> void IndexBuffer<T>::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}