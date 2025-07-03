#pragma once
#include <Common/GLInclude.h>
#include <vector>
template <typename T>
class ComputeBuffer {
private:
    GLuint m_BufferId;
    size_t m_CurrentBufferSize = 0;

public:
    ComputeBuffer(){
         glGenBuffers(1, &m_BufferId);
    }

    void bind(int base){
         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, base, m_BufferId);
    }

    std::vector<T> retreiveData(){
        if (m_CurrentBufferSize == 0) {
            std::cerr << "Buffer size is zero. No data to read.\n";
            return {};
        }

        std::vector<T> returnData(m_CurrentBufferSize);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferId);

        void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        if (ptr) {
            std::memcpy(returnData.data(), ptr, m_CurrentBufferSize * sizeof(T));
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        } else {
            std::cerr << "Failed to map buffer for readback\n";
        }

        return returnData;
    }

    void updateSSBOData(const std::vector<T>& newData, bool forceUpdate){
        // Check if we need to resize the SSBO
        if(forceUpdate || newData.size() != m_CurrentBufferSize){
            m_CurrentBufferSize = newData.size();
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferId);
            glBufferData(GL_SHADER_STORAGE_BUFFER, m_CurrentBufferSize * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        }

        // Update the SSBO with the new data
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferId);
        void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        if (ptr) {
            std::memcpy(ptr, newData.data(), m_CurrentBufferSize * sizeof(T));
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        }else {
            std::cerr << "Failed to map buffer for write\n";
        }
    }

    void cleanup(){
        glDeleteBuffers(1, &m_BufferId);
    }
};