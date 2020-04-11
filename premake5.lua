local name = "projectname"

local is_visual_studio = _ACTION:find("^vs") ~= nil 
local project_name = "app." .. name

workspace(name)
  characterset ("MBCS")
  configurations { "Debug", "Release" }
  startproject (project_name)
  location ".build/projects"
  targetdir ".build/bin/%{cfg.buildcfg}"
  debugdir "bin"
  kind "StaticLib"
  language "C++"
  warnings "Off"
  architecture "x64"
  cppdialect "C++17"

  if is_visual_studio then
    linkoptions {
      "/ignore:4006", -- F already defined in X.lib; second definition ignored
    }
  end

  defines { "EA_SSE=4", "SOKOL_NO_ENTRY" }

  filter "configurations:Debug"
    defines { "DEBUG", "EA_DEBUG" }
    symbols "Full"
    optimize "Off"
    targetsuffix "-d"

  filter "configurations:Release"
    defines { "RELEASE", "NDEBUG" }
    symbols "Off"
    optimize "Size"

project "_root"
  kind "None"
  files { "**" }
  removefiles { ".git/**" }
  removefiles { ".build/**" }
  removefiles { "externals/**" }
  removefiles { "**.cpp", "**.h", "**.c" }

project (project_name)
  kind "ConsoleApp"
  language "C++"
  warnings "Extra"
  includedirs { "externals/include" }
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  includedirs { "externals/EaStdC/include" }
  includedirs { "externals/ros/include" }
  files { "source/" .. project_name .. "/**.h", "source/" .. project_name .. "/**.c", "source/" .. project_name .. "/**.cpp" }
  links {  "lib.singleheaders", "lib.eastl", "lib.eaassert", "lib.eastdc", "lib.eathread" }

project "lib.singleheaders"
  includedirs { "externals/include" }
  files { "externals/include/**.h" }
  files { "source/lib.singleheaders/**.c" }
  files { "source/lib.singleheaders/**.cpp" }

project "lib.ros"
  kind "None"
  files { "externals/ros/include/**.h" }
  files { "externals/ros/include/**.cpp" }

project "lib.eabase"
  kind "None"
  files { "externals/EABase/include/Common/**.h" }

project "lib.eaassert"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  files { "externals/EAAssert/include/**.h" }
  files { "externals/EAAssert/source/**.cpp" }

project "lib.eastl"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  files { "externals/EASTL/include/**.h" }
  files { "externals/EASTL/source/**.cpp" }
  links { "lib.eaassert" }

project "lib.eathread"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  files { "externals/EAThread/include/**.h" }
  files { "externals/EAThread/source/*.cpp" } -- only take root level cpp files on purpose
  links { "lib.eastl", "lib.eaassert" }

project "lib.eastdc"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  includedirs { "externals/EaStdC/include" }
  files { "externals/EAStdC/include/**.h" }
  files { "externals/EAStdC/source/**.cpp" }
  links { "lib.eastl", "lib.eaassert" }
