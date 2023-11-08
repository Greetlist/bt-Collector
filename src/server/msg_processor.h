#ifndef __MSG_PROCESSOR_H_
#define __MSG_PROCESSOR_H_

#include <thread>
#include <vector>
#include <atomic>
#include <string>
#include <glog/logging.h>
#include <chrono>

#include "msg_queue/mpmc_queue.h"

class MsgProcessor {
public:
  explicit MsgProcessor(MPMCQueue<std::string>*, int);
  ~MsgProcessor();
  void InitThreads();
  void ProcessorMain();
  void Stop();
private:
  std::vector<std::thread> processor_vec_;
  MPMCQueue<std::string>* msg_queue_;
  int processor_num_;
  std::atomic<bool> stop_{false};
};

#endif
