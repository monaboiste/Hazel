project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../Bin-Int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Src/**.h",
		"Src/**.cpp",
	}

	includedirs
	{
		"../Hazel/Src",
		"../Hazel/Vendor",
		"../Hazel/Vendor/spdlog/include",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"