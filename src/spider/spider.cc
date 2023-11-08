#include "spider/spider.h"

Spider::Spider(YAML::Node config) : server_config_(config) {
}

Spider::~Spider() {
  server_->Stop();
  delete server_;
  delete msg_queue_;
}

void Spider::Init() {
  msg_queue_ = new MPMCQueue<std::string>(server_config_["mpmc_size"].as<int>());
  server_ = new EpollServer(msg_queue_, server_config_);
  server_->Init();
}

void Spider::Start() {
  server_->MainProcess();
}
