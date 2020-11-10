#include "Buffer.hpp"
#include <sstream>


std::ostream& operator<<(std::ostream& os, BufferDataMode mode) {
    switch (mode) {
        case BufferDataMode::StaticDraw:
            return os << "StaticDraw";
        case BufferDataMode::StaticCopy:
            return os << "StaticCopy";
        case BufferDataMode::StaticRead:
            return os << "StaticRead";
        case BufferDataMode::StreamDraw:
            return os << "StreamDraw";
        case BufferDataMode::StreamCopy:
            return os << "StreamCopy";
        case BufferDataMode::StreamRead:
            return os << "StreamRead";
        case BufferDataMode::DynamicDraw:
            return os << "DynamicDraw";
        case BufferDataMode::DynamicCopy:
            return os << "DynamicCopy";
        case BufferDataMode::DynamicRead:
            return os << "DynamicRead";
    }
    return os;
}

VertexBuffer::VertexBuffer(BufferDataMode mode) {
    glGenBuffers(1, &m_BufferID);
    m_BufferMode = mode;
    bind();
    unbind();
}

void VertexBuffer::updateBuffer() {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_BufferData.size() * sizeof(float), m_BufferData.data());
    unbind();
}

void VertexBuffer::updateBuffer(GLsizei start, GLsizei end) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, start * sizeof(float), (end - start) * sizeof(float), m_BufferData.data() + start);
    unbind();
}

void VertexBuffer::pushBuffer() {
    bind();
    glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), static_cast<unsigned int>(m_BufferMode));
    unbind();
}

void VertexBuffer::pushBuffer(BufferDataMode newMode) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), static_cast<int>(newMode));
    m_BufferMode = newMode;
    unbind();
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::pushFloat(float f) {
    m_BufferData.push_back(f);
}

void VertexBuffer::setFloat(size_t index, float f) {
    m_BufferData.at(index) = f;
}

void VertexBuffer::eraseFloat(size_t index) {
    m_BufferData.erase(m_BufferData.begin() + index);
}

void VertexBuffer::pushFloats(float* f, size_t count) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.push_back(f[i]);
    }
}

void VertexBuffer::setFloats(float* f, size_t count, size_t front) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.at(front + i) = f[i];
    }
}

void VertexBuffer::eraseRange(size_t begin, size_t end) {
    m_BufferData.erase(m_BufferData.begin() + begin, m_BufferData.begin() + end);
}

void VertexBuffer::pushVec2f(float x, float y) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
}

void VertexBuffer::pushVec3f(float x, float y, float z) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
}

void VertexBuffer::pushVec4f(float x, float y, float z, float w) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
    m_BufferData.push_back(w);
}

void VertexBuffer::setVec2f(size_t index, float x, float y) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
}

void VertexBuffer::setVec3f(size_t index, float x, float y, float z) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
    m_BufferData.at(index+2) = z;

}

void VertexBuffer::setVec4f(size_t index, float x, float y, float z, float w) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
    m_BufferData.at(index+2) = z;
    m_BufferData.at(index+3) = w;
}

std::string VertexBuffer::getDebugTitle() {
    std::ostringstream ss;
    ss << "<VertexBuffer " << m_BufferID << " ; " << m_BufferData.size() <<" * floats ; " << m_BufferMode << ">";
    return ss.str();
}

std::string VertexBuffer::getFullDebugInfo() {
    std::ostringstream ss;
    ss << "VertexBuffer:\n\tID: " << m_BufferID << "\n\tSize: " << m_BufferData.size() << "\n\tData Type: float\n\tBuffer Mode: " << m_BufferMode << "\n";
    return ss.str();
}

float VertexBuffer::getValue(size_t index) {
    return m_BufferData.at(index);
}

glm::vec2 VertexBuffer::getVec2(size_t index) {
    return glm::vec2(getValue(index), getValue(index + 1));
}

glm::vec3 VertexBuffer::getVec3(size_t index) {
    return glm::vec3(getValue(index), getValue(index + 1), getValue(index + 2));
}

glm::vec4 VertexBuffer::getVec4(size_t index) {
    return glm::vec4(getValue(index), getValue(index + 1), getValue(index + 2), getValue(index + 3));
}


IndexBuffer::IndexBuffer(BufferDataMode mode) {
    glGenBuffers(1, &m_BufferID);
    m_BufferMode = mode;
    bind();
    unbind();
}

void IndexBuffer::updateBuffer() {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data());
    unbind();
}

void IndexBuffer::updateBuffer(GLsizei start, GLsizei end) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start * sizeof(unsigned int), (end - start) * sizeof(unsigned int), m_BufferData.data() + start);
    unbind();
}

void IndexBuffer::pushBuffer() {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data(), static_cast<unsigned int>(m_BufferMode));
    unbind();
}

void IndexBuffer::pushBuffer(BufferDataMode newMode) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data(), static_cast<int>(newMode));
    m_BufferMode = newMode;
    unbind();
}

void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::pushValue(unsigned int value) {
    m_BufferData.push_back(value);
}

void IndexBuffer::setFloat(size_t index, unsigned int v) {
    m_BufferData.at(index) = v;
}

void IndexBuffer::eraseValue(size_t index) {
    m_BufferData.erase(m_BufferData.begin() + index);
}

void IndexBuffer::pushValues(unsigned int* values, size_t count) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.push_back(values[i]);
    }
}

void IndexBuffer::setValues(unsigned int* v, size_t count, size_t front) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.at(front + i) = v[i];
    }
}

void IndexBuffer::eraseRange(size_t begin, size_t end) {
    m_BufferData.erase(m_BufferData.begin() + begin, m_BufferData.begin() + end);
}

std::string IndexBuffer::getDebugTitle() {
    std::ostringstream ss;
    ss << "<IndexBuffer " << m_BufferID << " ; " << m_BufferData.size() <<" * unsigned ints ; " << m_BufferMode << ">";
    return ss.str();
}

std::string IndexBuffer::getFullDebugInfo() {
    std::ostringstream ss;
    ss << "IndexBuffer:\n\tID: " << m_BufferID << "\n\tSize: " << m_BufferData.size() << "\n\tData Type: unsigned int\n\tBuffer Mode: " << m_BufferMode << "\n";
    return ss.str();
}

unsigned int IndexBuffer::getValue(size_t index) {
    return m_BufferData.at(index);
}