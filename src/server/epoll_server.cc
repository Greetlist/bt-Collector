#include "server/epoll_server.h"

EpollServer::EpollServer(MPMCQueue<std::string>* queue, YAML::Node config) : msg_queue_(queue), server_config_(config) {
}

EpollServer::~EpollServer() {
  delete msg_processor_;
}

void EpollServer::Init() {
  epoll_fd_ = epoll_create1(0);
  if (epoll_fd_ < 0) {
    LOG(INFO) << "Epoll Create Error: " << strerror(errno);
    exit(1);
  }
  InitRecvSocket();
  InitSendSocket();
  msg_processor_ = new MsgProcessor(msg_queue_, server_config_);
  msg_processor_->InitThreads();
}

void EpollServer::InitRecvSocket() {
  listen_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (listen_fd_ < 0) {
    LOG(ERROR) << "Create Listen Socket Error: " << strerror(errno);
    exit(1);
  }

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(server_config_["bind_port"].as<int>());
  server.sin_addr.s_addr = inet_addr(server_config_["bind_address"].as<std::string>().c_str());

  bool flag = true;
  setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  int ss = bind(listen_fd_, (struct sockaddr*)&server, sizeof(server));
  if (ss < 0) {
    LOG(ERROR) << "Bind Error" << strerror(errno);
    exit(1);
  }
  ss = set_nonblock(listen_fd_);
  if (ss < 0) {
    LOG(ERROR) << "Set Nonblock Error: " << strerror(errno);
    exit(1);
  }

  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.fd = listen_fd_;
  ev.events = EPOLLIN | EPOLLET;
  ss = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, listen_fd_, &ev);
  if (ss < 0) {
    LOG(ERROR) << "Epoll Add Error: " << strerror(errno);
    exit(1);
  }
  LOG(INFO) << "Create listen socket success";
}

void EpollServer::InitSendSocket() {
  send_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (send_fd_ < 0) {
    LOG(ERROR) << "Create Send Socket Error: " << strerror(errno);
    exit(1);
  }
  int ss = set_nonblock(send_fd_);
  if (ss < 0) {
    LOG(ERROR) << "Set Nonblock Error: " << strerror(errno);
    exit(1);
  }

  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.data.fd = send_fd_;
  ev.events = EPOLLIN | EPOLLET;
  ss = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, send_fd_, &ev);
  if (ss < 0) {
    LOG(ERROR) << "Epoll Add Error: " << strerror(errno);
    exit(1);
  }
  LOG(INFO) << "Create send socket success";
}

void EpollServer::MainProcess() {
  socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
  while (!stop_) {
    struct epoll_event events[1024];
    int num = epoll_wait(epoll_fd_, events, 1024, 1000);
    for (int i = 0; i < num; ++i) {
      if (events[i].data.fd == listen_fd_) {
        struct sockaddr_storage peer_addr;
        char read_buf[65536];
        int read_bytes_num = recvfrom(
            listen_fd_, read_buf, 65536, 0,
            (struct sockaddr*)&peer_addr, &peer_addr_len);
        if (read_bytes_num < 0 && errno != EAGAIN) {
          LOG(INFO) << "Read UDP Error: " << strerror(errno);
        } else {
          char host[NI_MAXHOST], port[NI_MAXSERV];
          getnameinfo(
            (struct sockaddr*)&peer_addr, peer_addr_len,
            host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);
          LOG(INFO) << "Peer addr is: " << host << ", port is: " << std::stoi(std::string{port});
          std::string* data = new std::string{read_buf};
          msg_queue_->Push(data);
        }
      } else if (events[i].data.fd == listen_fd_) {

      }
    }
  }
}

void EpollServer::Stop() {
  msg_processor_->Stop();
  stop_ = true;
}





