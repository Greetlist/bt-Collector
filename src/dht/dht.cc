#include "dht/dht.h"

DHT::DHT() {
}

DHT::~DHT() {
}

void DHT::JoinDHTNet() {
  YAML::Node primary_nodes = server_config_["primary_nodes"];
  for (const auto& node : primary_nodes) {
    std::string total_str = node["host"].as<std::string>() + ":" + node["port"].as<std::string>();
    LOG(INFO) << total_str;
  }
}

void DHT::OnPacketRecv(std::string* raw_data) {
  auto data = bencoding::decode(*raw_data);
  std::string repr = bencoding::getPrettyRepr(std::move(data));
  LOG(INFO) << repr;
}
