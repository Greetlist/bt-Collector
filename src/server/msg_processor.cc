#include "server/msg_processor.h"

MsgProcessor::MsgProcessor(MPMCQueue<std::string>* queue, int processor_num) : msg_queue_(queue), processor_num_(processor_num) {
}

MsgProcessor::~MsgProcessor() {
  while (msg_queue_->Size() > 0) {
    std::string* msg_string = msg_queue_->Pop();
    if (msg_string != nullptr) {
      LOG(INFO) << *msg_string;
    }
  }
  for (auto& t : processor_vec_) {
    t.join();
  }
}

void MsgProcessor::InitThreads() {
  for (int i = 0; i < processor_num_; ++i) {
    std::thread t = std::thread(&MsgProcessor::ProcessorMain, this);
    processor_vec_.emplace_back(std::move(t));
  }
}

void MsgProcessor::ProcessorMain() {
  while (!stop_) {
    std::string* msg_string = msg_queue_->Pop();
    if (msg_string != nullptr) {
      LOG(INFO) << *msg_string;
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void MsgProcessor::Stop() {
  stop_ = true;
}
