workspace "RiverValleyRefined"
	architecture "x64"
	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "RiverValleyRefined"
	location"RiverValleyRefined"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/RiverValleyRefined.rc"
	}
	includedirs {
		"%{prj.name}/vendor/json/include",
		"%{prj.name}/vendor/SDL2/include",
		"%{prj.name}/vendor/SDL2_image/include",
		"%{prj.name}/vendor/SDL2_mixer/include",
		"%{prj.name}/vendor/SDL2_ttf/include",
		"%{prj.name}/source"
	}
	libdirs {
		"%{prj.name}/vendor/SDL2/lib/x64",
		"%{prj.name}/vendor/SDL2/lib/x64",
		"%{prj.name}/vendor/SDL2_image/lib/x64",
		"%{prj.name}/vendor/SDL2_mixer/lib/x64",
		"%{prj.name}/vendor/SDL2_ttf/lib/x64"
	}
	links {
		"SDL2",
		"SDL2main",
		"SDL2_image",
		"SDL2_mixer",
		"SDL2_ttf"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	filter "configurations:Debug"
		defines {
			"_DEBUG",
			"_CONSOLE"
		}
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines {
			"NDEBUG",
			"_CONSOLE"
		}
		buildoptions "/MD"
		optimize "On"