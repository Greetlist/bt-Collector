add_rules("mode.debug", "mode.release")
add_requires("glog", "gflags", "nlohmann_json", "yaml-cpp")

target("bt_collector")
    set_kind("binary")
    add_packages("glog", "gflags", "nlohmann_json", "yaml-cpp")
    add_includedirs("src", "third_party/bencoding/include")
    set_languages("c++17")
    set_warnings("all", "error")
    add_files("src/**.cc")

    add_linkdirs("third_party/bencoding/lib")
    add_links("bencoding")
