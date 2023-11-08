#include <iostream>
#include <gflags/gflags.h>

#include "spider/spider.h"

DEFINE_string(config_path, "./conf/config.yaml", "config path");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  YAML::Node server_config = YAML::LoadFile(FLAGS_config_path);
  Spider* spider = new Spider(server_config);
  spider->Init();
  spider->Start();

  delete spider;
  return 0;
}
