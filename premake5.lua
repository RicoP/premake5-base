local is_visual_studio = _ACTION:find("^vs") ~= nil 

workspace "projectname"
  characterset ("MBCS")
  configurations { "Debug", "Release" }
  startproject "app.projectname"
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
  removefiles { "source/**.cpp", "source/**.h" }

project "app.projectname"
  kind "ConsoleApp"
  language "C++"
  warnings "Extra"
  includedirs { "externals/include" }
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  includedirs { "externals/EaStdC/include" }
  files { "source/app.projectname/**.h", "source/app.projectname/**.c", "source/app.projectname/**.cpp" }
  links {  "lib.singleheaders", "lib.eastl", "lib.eaassert", "lib.eastdc", "lib.eathread" }

project "lib.singleheaders"
  includedirs { "externals/include" }
  files { "externals/include/**.h" }
  files { "source/lib.singleheaders/**.cpp" }

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
  files { "externals/EAThread/source/*.cpp" }
  files { "externals/EAThread/source/pc/*.cpp" }
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
