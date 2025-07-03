#pragma once
#include "VertexBufferLayout.h"


enum BufferType {
	STATIC,
	DYNAMIC,
	STREAM
};

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer() {

	}

	VertexBuffer(const void* data, unsigned int size, BufferType type) {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		switch (type) {
			case STATIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
				break;
			case DYNAMIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
				break;
			default:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
		
	}
	~VertexBuffer() {
	}

	void Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void UnBind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};