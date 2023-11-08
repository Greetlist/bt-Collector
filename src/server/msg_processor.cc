#include "server/msg_processor.h"

MsgProcessor::MsgProcessor(MPMCQueue<std::string>* queue, int thread_num) : msg_queue_(queue), thread_num_(thread_num) {
}

MsgProcessor::~MsgProcessor() {
  for (auto t : processor_vec_) {
    t.join();
  }
}

MsgProcessor::InitThread() {
  for (int i = 0; i < thread_num_; ++i) {
    std::thread t = std::thread(MsgProcessor::ProcessorMain, this);
    processor_vec_.emplace_back(std::move(t));
  }
}

MsgProcessor::ProcessorMain() {
  while (!stop_) {
    std::string* msg_string = msg_queue->Pop();
    LOG(INFO) << *msg_string;
  }
}
