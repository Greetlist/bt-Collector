#ifndef __DHT_H_
#define __DHT_H_

#include <yaml-cpp/yaml.h>
#include <string>
#include <glog/logging.h>

#include "bencoding/bencoding.h"

class DHT {
public:
  DHT();
  ~DHT();
  void JoinDHTNet();
  void OnPacketRecv(std::string* raw_data);
private:
  YAML::Node server_config_;
};

#endif
