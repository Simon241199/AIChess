project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++23"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   -- pchheader "TorchPch.h"
   -- pchsource "Core/Source/Core/TorchPch.cpp"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs {
      "Source",
      "%{prj.location}/Vendor/libtorch/%{cfg.buildcfg}/include",
      "%{prj.location}/Vendor/libtorch/%{cfg.buildcfg}/include/torch/csrc/api/include"
   }

   libdirs {
       "%{prj.location}/vendor/libtorch/%{cfg.buildcfg}/lib"
   }

   links {
       "torch",
       "c10",
       "torch_cpu"
   }

   postbuildcommands {
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/c10.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/fbgemm.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/torch.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/uv.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/asmjit.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/cupti64_2022.3.0.dll" "%{cfg.targetdir}/../App"',
        '{COPY} "%{prj.location}vendor/libtorch/%{cfg.buildcfg}/lib/torch_cpu.dll" "%{cfg.targetdir}/../App"',
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"