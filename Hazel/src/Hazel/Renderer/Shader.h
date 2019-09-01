#pragma once

#include <string>

namespace Hazel {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vetexSource, const std::string& fragmentSource);
		static Shader* Create(const std::string& filepath);
	};

}