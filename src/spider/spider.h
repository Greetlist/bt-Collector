#ifndef __SPIDER_H_
#define __SPIDER_H_

#include <yaml-cpp/yaml.h>

#include "server/epoll_server.h"
#include "msg_queue/mpmc_queue.h"

class Spider {
public:
  explicit Spider(YAML::Node);
  ~Spider();
  void Init();
  void Start();
private:
  EpollServer* server_;
  MPMCQueue<std::string>* msg_queue_;
  YAML::Node server_config_;
};

#endif
