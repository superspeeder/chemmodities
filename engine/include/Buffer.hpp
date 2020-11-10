#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <DebugUtils.hpp>
#include <iostream>

enum BufferDataMode {
    StaticDraw = (int)GL_STATIC_DRAW,
    StaticCopy = (int)GL_STATIC_COPY,
    StaticRead = (int)GL_STATIC_READ,

    StreamDraw = (int)GL_STREAM_DRAW,
    StreamCopy = (int)GL_STREAM_COPY,
    StreamRead = (int)GL_STREAM_READ,

    DynamicDraw = (int)GL_DYNAMIC_DRAW,
    DynamicCopy = (int)GL_DYNAMIC_COPY,
    DynamicRead = (int)GL_DYNAMIC_READ
};

std::ostream& operator<<(std::ostream& os, BufferDataMode mode);


class VertexBuffer : DebuggableObject {
private:
    GLuint m_BufferID;
    GLuint m_CurrentBufferSize;

    std::vector<float> m_BufferData;

    BufferDataMode m_BufferMode;
public:
    
    /** Create a vertex buffer
     * 
     * @param mode the initial mode for the vertexbuffer
     * @see BufferDataMode
     */
    VertexBuffer(BufferDataMode mode);

    /** 
     * Update the buffer.
     * runs faster than pushing because it doesn't need to reallocate any memory for the buffer
     */
    void updateBuffer();

    /** 
     * Update the buffer with data between start and end
     * runs faster than pushing because it doesn't need to reallocate any memory for the buffer and it doesn't have to push the entire dataset to the buffer
     * @param start the offset for the beginning of the buffer
     * @param end the offset for the end of the buffer
     */
    void updateBuffer(GLsizei start, GLsizei end);

    /**
     * Push data to the buffer. Required when buffer needs to grow/shrink, but is slower beceause it reallocates the memory.
     * Uses the current buffer mode..
     */
    void pushBuffer();

    /**
     * Push data to the buffer. Required when buffer needs to grow/shrink, but is slower beceause it reallocates the memory
     * @param newMode the new mode for the buffer to be in
     * @see BufferDataMode
     */
    void pushBuffer(BufferDataMode newMode);


    /**
     * Bind the buffer to the opengl arraybuffer slot
     */
    void bind();

    /**
     * Unbind the buffer from the opengl arraybuffer slot.
     * Technically will unbind any buffer bound to the arraybuffer slot even if it isn't this buffer
     */
    void unbind();

    /**
     * Push a float to the end of the buffer
     * Push to buffer to be able to use the pushed data
     * @param f the float being pushed
     * @see pushBuffer()
     */
    void pushFloat(float f);

    /**
     * Set the value of the float at an index
     * @param index the index of the float to set
     * @param f the float to set the index to
     */
    void setFloat(size_t index, float f);

    /**
     * Erase the float at an index 
     * @param index the index of the float to erase
     */
    void eraseFloat(size_t index);

    /**
     * Push multiple floats to the end of the buffer
     * @param f an array of floats to push
     * @param count the number of floats in the array
     */
    void pushFloats(float* f, size_t count);

    /**
     * Set multiple floats starting at some index.
     * @param f the values to set into the buffer
     * @param count the number of floats being set
     * @param front the index of the first float to set
     */
    void setFloats(float* f, size_t count, size_t front);
 
    /**
     * Erase a range from the buffer
     * @param begin the start of the range
     * @param end the end of the range
     */
    void eraseRange(size_t begin, size_t end);

    /**
     * Push a 2 component vector of floats to the end of the buffer
     * @param x the x component of the vector
     * @param y the y component of the vector
     */
    void pushVec2f(float x, float y);

    /**
     * Push a 4 component vector of floats to the end of the buffer
     * @param x the x component of the vector
     * @param y the y component of the vector
     * @param z the z component of the vector
     */
    void pushVec3f(float x, float y, float z);

    /**
     * Push a 4 component vector of floats to the end of the buffer
     * @param x the x component of the vector
     * @param y the y component of the vector
     * @param z the z component of the vector
     * @param w the w component of the vector
     */
    void pushVec4f(float x, float y, float z, float w);

    /**
     * Set a 2 component vector at a position within the buffer
     * @param index the starting index of the vector
     * @param x the x component of the vector
     * @param y the y component of the vector
     */
    void setVec2f(size_t index, float x, float y);
    
    /**
     * Set a 3 component vector at a position within the buffer
     * @param index the starting index of the vector
     * @param x the x component of the vector
     * @param y the y component of the vector
     * @param z the z component of the vector
     */
    void setVec3f(size_t index, float x, float y, float z);
    
    /**
     * Set a 4 component vector at a position within the buffer
     * @param index the starting index of the vector
     * @param x the x component of the vector
     * @param y the y component of the vector
     * @param z the z component of the vector
     * @param w the w component of the vector
     */
    void setVec4f(size_t index, float x, float y, float z, float w);

    /**
     * Returns a string in the format of `<VertexBuffer {id} ; {size} * floats ; {stringify(buffer_mode)}>`
     * @return the debug title
     */
    std::string getDebugTitle() override;

    /**
     * Returns a fully detailed set of info on this buffer
     * format:
     * ```
     * VertexBuffer:
     *   ID: {buffer_id}
     *   Size: {buffer_size}
     *   Data Type: float
     *   Buffer Mode: {stringify(buffer_mode)}
     * ```
     * @return the debug info
     */
    std::string getFullDebugInfo() override;

    /**
     * Get the number of values in the buffer
     * @return number of values
     */
    inline size_t getSize() { return m_BufferData.size(); };

    /**
     * Get the mode of the buffer
     * @return the mode of the buffer
     * @see BufferDataMode
     */
    inline BufferDataMode getMode() { return m_BufferMode; };

    /**
     * Get the value at an index
     * @param index the index to get the value of
     * @return the value at index
     */
    float getValue(size_t index);

    /**
     * Get a pointer to the data in the buffer
     * @return a pointer to the entire buffer data
     */
    inline float* getData() { return m_BufferData.data(); };

    /**
     * Get a 2 component glm vector at index
     * @param index the index to get the vector at
     * @return the 2 component vector
     */
    glm::vec2 getVec2(size_t index);
    
    /**
     * Get a 3 component glm vector at index
     * @param index the index to get the vector at
     * @return the 3 component vector
     */
    glm::vec3 getVec3(size_t index);
    
    /**
     * Get a 4 component glm vector at index
     * @param index the index to get the vector at
     * @return the 4 component vector
     */
    glm::vec4 getVec4(size_t index);

    /**
     * Push a 2 component glm vector to the end of the buffer
     * @param vec the vector
     */
    inline void pushVec2f(glm::vec2 vec) { pushVec2f(vec.x,vec.y); };

    /**
     * Push a 3 component glm vector to the end of the buffer
     * @param vec the vector
     */
    inline void pushVec3f(glm::vec3 vec) { pushVec3f(vec.x, vec.y, vec.z); };

    /**
     * Push a 4 component glm vector to the end of the buffer
     * @param vec the vector
     */
    inline void pushVec4f(glm::vec4 vec) { pushVec4f(vec.x, vec.y, vec.z, vec.w); };

    /**
     * Set a 2 component glm vector at a position within the buffer
     * @param index the starting index of the vector
     * @param vec the vector
     */
    inline void setVec2f(size_t index, glm::vec2 vec) { setVec2f(index, vec.x, vec.y); };
    
    /**
     * Set a 3 component glm vector at a position within the buffer
     * @param index the starting index of the vector
     * @param vec the vector
     */
    inline void setVec3f(size_t index, glm::vec3 vec) { setVec3f(index, vec.x, vec.y, vec.z); };

    /**
     * Set a 4 component glm vector at a position within the buffer
     * @param index the starting index of the vector
     * @param vec the vector
     */
    inline void setVec4f(size_t index, glm::vec4 vec) { setVec4f(index, vec.x, vec.y, vec.z, vec.w); };

};


class IndexBuffer : DebuggableObject {
private:
    GLuint m_BufferID;
    GLuint m_CurrentBufferSize;

    std::vector<unsigned int> m_BufferData;

    BufferDataMode m_BufferMode;
public:
    
    /** Create an index buffer
     * 
     * @param mode the initial mode for the indexbuffer
     * @see BufferDataMode
     */
    IndexBuffer(BufferDataMode mode);

    /** 
     * Update the buffer.
     * runs faster than pushing because it doesn't need to reallocate any memory for the buffer
     */
    void updateBuffer();

    /** 
     * Update the buffer with data between start and end
     * runs faster than pushing because it doesn't need to reallocate any memory for the buffer and it doesn't have to push the entire dataset to the buffer
     * @param start the offset for the beginning of the buffer
     * @param end the offset for the end of the buffer
     */
    void updateBuffer(GLsizei start, GLsizei end);

    /**
     * Push data to the buffer. Required when buffer needs to grow/shrink, but is slower beceause it reallocates the memory.
     * Uses the current buffer mode..
     */
    void pushBuffer();

    /**
     * Push data to the buffer. Required when buffer needs to grow/shrink, but is slower beceause it reallocates the memory
     * @param newMode the new mode for the buffer to be in
     * @see BufferDataMode
     */
    void pushBuffer(BufferDataMode newMode);


    /**
     * Bind the buffer to the opengl elementarraybuffer slot
     */
    void bind();

    /**
     * Unbind the buffer from the opengl elementarraybuffer slot.
     * Technically will unbind any buffer bound to the elementarraybuffer slot even if it isn't this buffer
     */
    void unbind();

    /**
     * Push a value to the end of the buffer
     * Push to buffer to be able to use the pushed data
     * @param v the value being pushed
     * @see pushBuffer()
     */
    void pushValue(unsigned int v);

    /**
     * Set the value at an index
     * @param index the index of the value to set
     * @param v the unsigned int to set the index to
     */
    void setFloat(size_t index, unsigned int v);

    /**
     * Erase the value at an index 
     * @param index the value of the float to erase
     */
    void eraseValue(size_t index);

    /**
     * Push multiple floats to the end of the buffer
     * @param f an array of floats to push
     * @param count the number of floats in the array
     */
    void pushValues(unsigned int* f, size_t count);

    /**
     * Set multiple floats starting at some index.
     * @param v the values to set into the buffer
     * @param count the number of floats being set
     * @param front the index of the first float to set
     */
    void setValues(unsigned int* v, size_t count, size_t front);
 
    /**
     * Erase a range from the buffer
     * @param begin the start of the range
     * @param end the end of the range
     */
    void eraseRange(size_t begin, size_t end);

    /**
     * Returns a string in the format of `<VertexBuffer {id} ; {size} * floats ; {stringify(buffer_mode)}>`
     * @return the debug title
     */
    std::string getDebugTitle() override;

    /**
     * Returns a fully detailed set of info on this buffer
     * format:
     * ```
     * VertexBuffer:
     *   ID: {buffer_id}
     *   Size: {buffer_size}
     *   Data Type: float
     *   Buffer Mode: {stringify(buffer_mode)}
     * ```
     * @return the debug info
     */
    std::string getFullDebugInfo() override;

    /**
     * Get the number of values in the buffer
     * @return number of values
     */
    inline size_t getSize() { return m_BufferData.size(); };

    /**
     * Get the mode of the buffer
     * @return the mode of the buffer
     * @see BufferDataMode
     */
    inline BufferDataMode getMode() { return m_BufferMode; };

    /**
     * Get the value at an index
     * @param index the index to get the value of
     * @return the value at index
     */
    unsigned int getValue(size_t index);

    /**
     * Get a pointer to the data in the buffer
     * @return a pointer to the entire buffer data
     */
    inline unsigned int* getData() { return m_BufferData.data(); };
};