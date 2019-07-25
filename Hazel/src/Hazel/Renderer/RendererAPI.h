#pragma once

#include <glm\glm.hpp>
#include "VertexArray.h"

namespace Hazel {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		inline static API GetAPI() { return ms_API; }
	private:
		static API ms_API;
	};

}