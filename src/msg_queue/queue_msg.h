#ifndef __QUEUE_MSG_H_
#define __QUEUE_MSG_H_

#include <string>

class MessageInfo {
public:
  MessageInfo(void* c, const int mt, const std::string&& mj) : conn(c), message_type(mt), message_json(mj) {}
  ~MessageInfo() = default;
  void* conn;
  int message_type;
  std::string message_json;
};

#endif
