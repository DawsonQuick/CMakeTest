#pragma once
#include <Common/GLInclude.h>
#include <iostream>
#include <vector>

class IndexBuffer {
public:
    IndexBuffer() {

    }

    IndexBuffer(const unsigned int* data, unsigned int count)
        : m_Count(count), m_BufferSize(count * sizeof(unsigned int)) {
       
        glGenBuffers(1, &m_Id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, data, GL_STATIC_DRAW);
    }

    ~IndexBuffer() {
    }

    void Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    }

    void Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

private:
    unsigned int m_Id;        // OpenGL buffer ID
    unsigned int m_Count;     // Number of indices in the buffer
    unsigned int m_BufferSize; // Size of the buffer in bytes
};
