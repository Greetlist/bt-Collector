add_rules("mode.debug", "mode.release")
add_requires("glog", "gflags", "nlohmann_json")

target("bt_collector")
    set_kind("binary")
    add_includedirs("src")
    set_languages("c++17")
    set_warnings("all", "error")
    add_files("src/*.cc")

