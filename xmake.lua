add_requires( "boost", {regex = True}) 
add_requires( "fmt" ) 

add_rules("mode.debug", "mode.release")

if is_mode('debug') then 
    set_runtimes("MDd")  
else
    set_runtimes("MD")
end

if is_plat("windows") then 
    add_cxxflags("/utf-8","/Zc:__cplusplus" ) -- MSVC 专属 flags
end 

set_languages("cxx17")
set_warnings("all")

target("mpc")
    set_default(true)
    set_kind("binary")
    add_files("./src/mpc/**.cpp") 
    add_includedirs("./src") 
    add_packages("boost","fmt")

target("asm")
    set_kind("binary")
    add_files("./src/asm/**.cpp") 
    add_includedirs("./src") 
    add_packages("boost","fmt")

target("test")
    set_kind("binary")
    add_files("./test/test_io.cpp") 
    add_includedirs("./src") 
    add_packages("boost","fmt")