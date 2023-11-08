#ifndef __EPOLL_SERVER_H_
#define __EPOLL_SERVER_H_

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <yaml-cpp/yaml.h>

#include "server/msg_processor.h"
#include "util/util.h"

class EpollServer {
public:
  explicit EpollServer(MPMCQueue<std::string>*, YAML::Node);
  ~EpollServer();
  void Init();
  void InitRecvSocket();
  void InitSendSocket();
  void MainProcess();
  void Stop();
private:
  MPMCQueue<std::string>* msg_queue_;
  int epoll_fd_;
  int listen_fd_;
  int send_fd_;
  MsgProcessor* msg_processor_;
  std::atomic<bool> stop_{false};
  YAML::Node server_config_;
};

#endif
