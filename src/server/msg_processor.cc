#include "server/msg_processor.h"

MsgProcessor::MsgProcessor(MPMCQueue<std::string>* queue, YAML::Node config) : msg_queue_(queue), server_config_(config) {
}

MsgProcessor::~MsgProcessor() {
  while (msg_queue_->Size() > 0) {
    std::string* msg_string = msg_queue_->Pop();
    if (msg_string != nullptr) {
      dht_->OnPacketRecv(msg_string);
    }
  }
  for (auto& t : processor_vec_) {
    t.join();
  }
  delete dht_;
}

void MsgProcessor::InitThreads() {
  dht_ = new DHT();
  int processor_num = server_config_["msg_processor_num"].as<int>();
  for (int i = 0; i < processor_num; ++i) {
    std::thread t = std::thread(&MsgProcessor::ProcessorMain, this);
    processor_vec_.emplace_back(std::move(t));
  }
}

void MsgProcessor::ProcessorMain() {
  while (!stop_) {
    std::string* msg_string = msg_queue_->Pop();
    if (msg_string != nullptr) {
      dht_->OnPacketRecv(msg_string);
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void MsgProcessor::Stop() {
  stop_ = true;
}
