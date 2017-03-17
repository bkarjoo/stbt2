#pragma once

#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include "message.h"

enum class read_mode {
    PACKET, MESSAGE, IGNORE_PACKET, IGNORE_MESSAGE
};

class message_retriever {

public:
  message_retriever() {}
  ~message_retriever() { }

  std::string header_str = "";
  std::set<std::string> symbols;
  std::map<std::string,int> field_sequences;

  char previous_char;
  void run();

  void set_file_path(std::string path) { file_path = path; }

  const char& get_c() const { return c; }
  const std::string& get_packet_header() const { return packet_header; }
  const std::shared_ptr<message> get_message_ptr() const { return mp; }
  const std::string& get_file_path() const { return file_path; }

  char msg_delim = 0; // indicates msg source and starts each new msg
  char msg_exch = 0; // lowercase letter in begining of msg designating exchange
  std::string symbol = ""; // the subject of the msg
  bool msg_header_complete = false;
  std::set<std::string>   watch_list;
private:


  std::string             file_path;


  std::shared_ptr<message>  mp  = nullptr;

  char c = 0;
  std::string packet_header = "";

  read_mode mode = read_mode::PACKET;
  void process_packet_delimiter(const char&);
  void process_message_delimiter(const char&);
  void process_field_delimiter(const char&);
  void add_packet_header_char(const char&);
  void add_message_header_char(const char&);

  bool is_packet_delimiter(const char& c)
  {
    return (c == 31);
  }

  bool is_message_delimiter(const char& c)
  {
    return (c == 11 || c == 13 || c == 14 || c == 15);
  }

  bool is_field_delimiter(const char& c)
  {
    return (c >= 97 && c <= 122) || c == 127;
  }

  bool is_lower_case(const char & c)
  {
    return (c >= 97 && c <= 122);
  }

  bool non_delimited_message(const char & c)
  {
    return (msg_header_complete || mode == read_mode::PACKET) && ((c >= 65 && c <= 90) || c == '~');
  }

};
