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

#include "server/msg_processor.h"

class EpollServer {
public:
  explicit EpollServer(MPMCQueue<std::string>*, int);
  ~EpollServer();
  void Init();

private:
  MPMCQueue<std::string>* msg_queue_;
  MsgProcessor* msg_processor_;
  std::atomic<bool> stop_;  
};

#endif
