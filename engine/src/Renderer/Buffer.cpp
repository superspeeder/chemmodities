#include "Renderer/Buffer.hpp"
#include <sstream>

#include "Application.hpp"

std::ostream& operator<<(std::ostream& os, che::BufferDataMode mode) {
    switch (mode) {
        case che::BufferDataMode::StaticDraw:
            return os << "StaticDraw";
        case che::BufferDataMode::StaticCopy:
            return os << "StaticCopy";
        case che::BufferDataMode::StaticRead:
            return os << "StaticRead";
        case che::BufferDataMode::StreamDraw:
            return os << "StreamDraw";
        case che::BufferDataMode::StreamCopy:
            return os << "StreamCopy";
        case che::BufferDataMode::StreamRead:
            return os << "StreamRead";
        case che::BufferDataMode::DynamicDraw:
            return os << "DynamicDraw";
        case che::BufferDataMode::DynamicCopy:
            return os << "DynamicCopy";
        case che::BufferDataMode::DynamicRead:
            return os << "DynamicRead";
    }
    return os;
}

void che::VertexBuffer::reserve(size_t floats) {

    bind();
    glBufferData(GL_ARRAY_BUFFER, floats * sizeof(GLfloat), NULL, static_cast<GLenum>(m_BufferMode));
    unbind();
}

che::VertexBuffer::VertexBuffer(che::BufferDataMode mode) {
    glGenBuffers(1, &m_BufferID);
    m_BufferMode = mode;
    bind();
    unbind();
    
    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1,&m_BufferID);
    spdlog::debug("Deleted VertexBuffer {}", m_BufferID);
}

void che::VertexBuffer::updateBuffer() {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_BufferData.size() * sizeof(float), m_BufferData.data());
    unbind();
}

void che::VertexBuffer::updateBuffer(GLsizei start, GLsizei end) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, start * sizeof(float), (end - start) * sizeof(float), m_BufferData.data() + start);
    unbind();
}

void che::VertexBuffer::pushBuffer() {
    bind();
    glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), static_cast<unsigned int>(m_BufferMode));
    unbind();
}

void che::VertexBuffer::pushBuffer(che::BufferDataMode newMode) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, m_BufferData.size() * sizeof(float), m_BufferData.data(), static_cast<int>(newMode));
    m_BufferMode = newMode;
    unbind();
}

void che::VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void che::VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void che::VertexBuffer::pushFloat(float f) {
    m_BufferData.push_back(f);
}

void che::VertexBuffer::setFloat(size_t index, float f) {
    m_BufferData.at(index) = f;
}

void che::VertexBuffer::eraseFloat(size_t index) {
    m_BufferData.erase(m_BufferData.begin() + index);
}

void che::VertexBuffer::pushFloats(float* f, size_t count) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.push_back(f[i]);
    }
}

void che::VertexBuffer::setFloats(float* f, size_t count, size_t front) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.at(front + i) = f[i];
    }
}

void che::VertexBuffer::eraseRange(size_t begin, size_t end) {
    m_BufferData.erase(m_BufferData.begin() + begin, m_BufferData.begin() + end);
}

void che::VertexBuffer::pushVec2f(float x, float y) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
}

void che::VertexBuffer::pushVec3f(float x, float y, float z) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
}

void che::VertexBuffer::pushVec4f(float x, float y, float z, float w) {
    m_BufferData.push_back(x);
    m_BufferData.push_back(y);
    m_BufferData.push_back(z);
    m_BufferData.push_back(w);
}

void che::VertexBuffer::setVec2f(size_t index, float x, float y) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
}

void che::VertexBuffer::setVec3f(size_t index, float x, float y, float z) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
    m_BufferData.at(index+2) = z;

}

void che::VertexBuffer::setVec4f(size_t index, float x, float y, float z, float w) {
    m_BufferData.at(index) = x;
    m_BufferData.at(index+1) = y;
    m_BufferData.at(index+2) = z;
    m_BufferData.at(index+3) = w;
}

std::string che::VertexBuffer::getDebugTitle() {
    std::ostringstream ss;
    ss << "<VertexBuffer " << m_BufferID << " ; " << m_BufferData.size() <<" * floats ; " << m_BufferMode << ">";
    return ss.str();
}

std::string che::VertexBuffer::getFullDebugInfo() {
    std::ostringstream ss;
    ss << "VertexBuffer:\n\tID: " << m_BufferID << "\n\tSize: " << m_BufferData.size() << "\n\tData Type: float\n\tBuffer Mode: " << m_BufferMode << "\n";
    return ss.str();
}

float che::VertexBuffer::getValue(size_t index) {
    return m_BufferData.at(index);
}

glm::vec2 che::VertexBuffer::getVec2(size_t index) {
    return glm::vec2(getValue(index), getValue(index + 1));
}

glm::vec3 che::VertexBuffer::getVec3(size_t index) {
    return glm::vec3(getValue(index), getValue(index + 1), getValue(index + 2));
}

glm::vec4 che::VertexBuffer::getVec4(size_t index) {
    return glm::vec4(getValue(index), getValue(index + 1), getValue(index + 2), getValue(index + 3));
}


che::IndexBuffer::IndexBuffer(che::BufferDataMode mode) {
    glGenBuffers(1, &m_BufferID);
    m_BufferMode = mode;
    bind();
    unbind();

    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}

che::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1,&m_BufferID);
    spdlog::debug("Deleted IndexBuffer {}", m_BufferID);
}

void che::IndexBuffer::updateBuffer() {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data());
    unbind();
}

void che::IndexBuffer::updateBuffer(GLsizei start, GLsizei end) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start * sizeof(unsigned int), (end - start) * sizeof(unsigned int), m_BufferData.data() + start);
    unbind();
}

void che::IndexBuffer::pushBuffer() {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data(), static_cast<unsigned int>(m_BufferMode));
    unbind();
}

void che::IndexBuffer::pushBuffer(che::BufferDataMode newMode) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferData.size() * sizeof(unsigned int), m_BufferData.data(), static_cast<int>(newMode));
    m_BufferMode = newMode;
    unbind();
}

void che::IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void che::IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void che::IndexBuffer::pushValue(unsigned int value) {
    m_BufferData.push_back(value);
}

void che::IndexBuffer::setFloat(size_t index, unsigned int v) {
    m_BufferData.at(index) = v;
}

void che::IndexBuffer::eraseValue(size_t index) {
    m_BufferData.erase(m_BufferData.begin() + index);
}

void che::IndexBuffer::pushValues(unsigned int* values, size_t count) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.push_back(values[i]);
    }
}

void che::IndexBuffer::setValues(unsigned int* v, size_t count, size_t front) {
    for (size_t i = 0; i < count; i++) {
        m_BufferData.at(front + i) = v[i];
    }
}

void che::IndexBuffer::eraseRange(size_t begin, size_t end) {
    m_BufferData.erase(m_BufferData.begin() + begin, m_BufferData.begin() + end);
}

std::string che::IndexBuffer::getDebugTitle() {
    std::ostringstream ss;
    ss << "<IndexBuffer " << m_BufferID << " ; " << m_BufferData.size() <<" * unsigned ints ; " << m_BufferMode << ">";
    return ss.str();
}

std::string che::IndexBuffer::getFullDebugInfo() {
    std::ostringstream ss;
    ss << "IndexBuffer:\n\tID: " << m_BufferID << "\n\tSize: " << m_BufferData.size() << "\n\tData Type: unsigned int\n\tBuffer Mode: " << m_BufferMode << "\n";
    return ss.str();
}

unsigned int che::IndexBuffer::getValue(size_t index) {
    return m_BufferData.at(index);
}

che::VertexArray::VertexArray() {
    glGenVertexArrays(1,&m_VertexArrayID);
    bind();
    unbind();
    
    if (che::Disposable::s_autoDisposable) {
        che::Disposable::s_mainDisposer->registerDisposableObject(this);
    }
}


che::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1,&m_VertexArrayID);
    spdlog::debug("Deleted VertexArray {}", m_VertexArrayID);
}


void che::VertexArray::bind() {
    glBindVertexArray(m_VertexArrayID);
}

void che::VertexArray::unbind() {
    glBindVertexArray(0);
}

void che::VertexBuffer::_pushBuffer() {
    glBufferData(GL_ARRAY_BUFFER,m_BufferData.size() * sizeof(GLfloat), m_BufferData.data(), m_BufferMode);
}

std::string stringifyAttribute(che::VertexAttribute attribute) {
    std::ostringstream ss;
    ss << "[VertexAttribute " << attribute.index << " " << attribute.buffer->getDebugTitle() << " {Vertex size:" << attribute.vertex_size << "} >";
    return ss.str();
}

void che::VertexArray::pushAttribute(che::VertexAttribute attribute, bool enable) {
    bind();
    attribute.buffer->bind();
    attribute.buffer->_pushBuffer();
    glVertexAttribPointer(attribute.index, attribute.vertex_size, GL_FLOAT, false, attribute.vertex_size * sizeof(GLfloat), 0);
    unbind();
    attribute.buffer->unbind();

    if (enable) enableAttribute(attribute.index);

    spdlog::debug("Pushed attribute {} to VertexArray {}", stringifyAttribute(attribute), m_VertexArrayID);
}

void che::VertexArray::enableAttribute(unsigned int index) {
    glEnableVertexArrayAttrib(m_VertexArrayID, index);
}

void che::VertexArray::disableAttribute(unsigned int index) {
    glDisableVertexArrayAttrib(m_VertexArrayID, index);
}

void che::VertexArray::attachIndexBuffer(che::IndexBuffer* ib) {
    glVertexArrayElementBuffer(m_VertexArrayID, ib->m_BufferID);
    spdlog::debug("Set Element Buffer for VertexArray {} to {}", m_VertexArrayID, ib->getDebugTitle());
}