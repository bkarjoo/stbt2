#include "message_retriever.h"
#include <iostream>

using namespace std;

void message_retriever::run()
{
  // TODO : implement ignoring packets based on time
  // TODO : implement ignoring messages based on symbol
  // TODO : implement getting message header here and only creating message if symbol needed
  std::ifstream fs(file_path);
  if (!fs.is_open()) return;
  while (fs >> c) {
    header_str += c;
    if (is_packet_delimiter(c)) {

      process_packet_delimiter(c);

    } else if (mode != read_mode::IGNORE_PACKET && is_message_delimiter(c)) {

      process_message_delimiter(c);

    } else if (mode != read_mode::IGNORE_PACKET && non_delimited_message(c)) {

      char x = 15;
      process_message_delimiter(x);
      if (is_lower_case(previous_char)) add_message_header_char(previous_char);
      add_message_header_char(c);

    } else if (mode == read_mode::PACKET) {

      add_packet_header_char(c);

    } else if (mode == read_mode::MESSAGE) {

      if (msg_header_complete) {
          //if (mp == nullptr) throw 20;
          mp->add_message_char(c);
          //if (mp->encountered_error) mode = read_mode::IGNORE_MESSAGE;
      }
      else
      {
        add_message_header_char(c);
      }
    }
    previous_char = c;
  }
  cout << "DONE" << endl;
  std::ofstream out("out.txt");
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
  std::map<std::string,int>::iterator it;
  for (it=field_sequences.begin(); it!=field_sequences.end(); ++it)
    std::cout << it->second << it->first << std::endl;
  std::cout.rdbuf(coutbuf);
  fs.close();
}

void message_retriever::process_packet_delimiter(const char&)
{
  //std::cout << "packet delimiter: " << packet_header << std::endl;
  if (mp != nullptr) {
    mp->message_complete();
    //cout << mp->field_sequence << endl;
    field_sequences[mp->field_sequence]++;

    if (mp->encountered_error) {
      std::ofstream out("out.txt");
      std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
      std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

      cout << endl << header_str << mp->msg_str << endl;
      throw 20;
    }
  }
  packet_header = "";
  msg_exch = 0;
  symbol = "";
  msg_header_complete = false;
  header_str = "";
  mode = read_mode::PACKET;
}

void message_retriever::process_message_delimiter(const char&)
{
  // TODO: pass old message to observers
  if (mp != nullptr) {
    mp->message_complete();
    //cout << mp->field_sequence << endl;
    field_sequences[mp->field_sequence]++;

    if (mp->encountered_error) {
      std::ofstream out("out.txt");
      std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
      std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

      cout << endl << header_str << mp->msg_str << endl;
      throw 20;
    }
  }
  msg_exch = 0;
  symbol = "";
  msg_header_complete = false;
  msg_delim = c;
  header_str = "";
  // if (mode == read_mode::PACKET) cout << packet_header << endl;
  if (mode != read_mode::IGNORE_PACKET) mode = read_mode::MESSAGE;
}

void message_retriever::add_packet_header_char(const char& c)
{

    packet_header += c;

}

void message_retriever::add_message_header_char(const char &c)
{
  if (msg_exch == 0 && symbol == "")
  {
    if (is_lower_case(c))
      msg_exch = c;
    else if (c == '.')
      mode = read_mode::IGNORE_MESSAGE; // ignore .TRACER meesages
    else
      symbol += c;
  } else {
    // TODO check if a field delim comes up
    if (is_field_delimiter(c)) {
      symbols.insert(symbol);
      if (watch_list.find(symbol) == watch_list.end()) {
        mode = read_mode::IGNORE_MESSAGE;
      } else {
        msg_header_complete = true;
        mp = std::make_shared<message>();
        mp->exchange = msg_exch;
        mp->delim = msg_delim;
        swap(mp->symbol,symbol);
        mp->add_message_char(c);
      }
    } else {
      symbol += c;
    }
  }
}
