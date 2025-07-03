#pragma once
#include "VertexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	
	unsigned int attributeCount = 0;
public:
	VertexArray() {
		glGenVertexArrays(1, &m_RendererID);
	}
	~VertexArray() {
	}

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(attributeCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			attributeCount++;
		}
	}


	void AddMatrixLayout(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(attributeCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			glVertexAttribDivisor(attributeCount, 1);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			attributeCount++;
		}
	}
	void Bind() const {
		glBindVertexArray(m_RendererID);
	}
	void UnBind() const {
		glBindVertexArray(0);
	}
};