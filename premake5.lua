local project_name = "projectname"

local is_visual_studio = _ACTION:find("^vs") ~= nil 

workspace ("__" .. project_name)
  characterset ("MBCS")
  configurations { "Debug", "DebugFast", "Release" }
  startproject "app.entry"
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

  filter "configurations:DebugFast"
    defines { "DEBUG", "EA_DEBUG" }
    symbols "Full"
    optimize "Size"
    targetsuffix "-df"

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

project "app.entry"
  warnings "Extra"
  targetname (project_name)
  links { "app.main" }
  files { "resource/*" }
  files { "source/app.entry/**" }
  filter "configurations:Debug"
    kind "ConsoleApp"
  filter "configurations:DebugFast"
    kind "ConsoleApp"
  filter "configurations:Release"
    kind "WindowedApp"

project "app.main"
  warnings "Extra"
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EaStdC/include" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  includedirs { "externals/include" }
  includedirs { "externals/include/imgui" }
  includedirs { "externals/include/ImGuiFileBrowser/FileBrowser" }
  includedirs { "externals/ros/include" }
  files { "source/app.main/**" }
  links {
    "lib.eaassert",
    "lib.eastdc",
    "lib.eastl",
    "lib.eathread",
    "lib.imgui",
    "lib.singleheaders"

  }

project "lib.singleheaders"
  includedirs { "externals/include" }
  files { "externals/include/**.h" }
  files { "source/lib.singleheaders/**" }
  -- ImGuiFileBrowser
  defines     { "ImGuiSelectableFlags_PressedOnClick=0" }
  includedirs { "externals/include/imgui" }
  includedirs { "externals/include/ImGuiFileBrowser/FileBrowser" }
  files       { "externals/include/ImGuiFileBrowser/FileBrowser/**.h" }
  files       { "externals/include/ImGuiFileBrowser/FileBrowser/**.cpp" }
  removefiles { "externals/include/ImGuiFileBrowser/FileBrowser/imgui_demo.cpp" }

project "lib.ros"
  kind "None"
  files { "externals/ros/include/**" }

project "lib.eabase"
  kind "None"
  files { "externals/EABase/include/Common/**" }

project "lib.eaassert"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  files { "externals/EAAssert/include/**.h" }
  files { "externals/EAAssert/source/**" }

project "lib.eastl"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  files { "externals/EASTL/include/**.h" }
  files { "externals/EASTL/source/**" }
  links { "lib.eaassert" }

project "lib.eathread"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  files { "externals/EAThread/include/**.h" }
  files { "externals/EAThread/source/*" } -- only take root level cpp files on purpose
  links { "lib.eastl", "lib.eaassert" }

project "lib.eastdc"
  includedirs { "externals/EABASE/include/Common" }
  includedirs { "externals/EAAssert/include/" }
  includedirs { "externals/EASTL/include" }
  includedirs { "externals/EAThread/include" }
  includedirs { "externals/EaStdC/include" }
  files { "externals/EAStdC/include/**.h" }
  files { "externals/EAStdC/source/**" }
  links { "lib.eastl", "lib.eaassert" }

project "lib.imgui"
  includedirs { "externals/include/imgui" }
  files { "externals/include/imgui/*.h" }
  files { "externals/include/imgui/*.cpp" }
