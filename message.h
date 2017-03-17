#pragma once
#include <string>
#include "trade_condition.h"

class message {

public:
  bool encountered_error = false;
  std::string msg_str = "";
  std::string field_sequence = "";
  char delim = 0;
  std::string symbol = "";
  char exchange = 0;
  const std::string& get_symbol() { return symbol; }
  char field_code = 0;
  std::string field_code_value = "";
  std::string field_value = "";
  bool header_complete = false;

  void add_message_char(const char&);
  void message_complete();
  void add_field_char(const char&);
  void process_field_delimiter(const char&);

  void register_simple();
  void register_complex();
  void register_complex_f();
  void register_complex_g();
  void register_complex_z();

  void         set_precision();
  unsigned int set_volume(const std::string&);
  double       set_price(const std::string&);


  unsigned short int    precision = 0;
  bool                  is_cycle_message = false;
  bool                  market_crossed = false;
  bool                  official_close_set = false;
  trade_condition       tc = trade_condition::regular_sale;
  unsigned int          session_volume = 0;
  double                day_high = 0.0;
  double                day_low = 0.0;
  double                day_highest_bid = 0.0;
  double                day_lowest_bid = 0.0;
  double                day_highest_ask = 0.0;
  double                day_lowest_ask = 0.0;
  double                close_price = 0.0;
  double                current_price = 0.0;
  double                last_price = 0.0;
  double                open_price = 0.0;
  double                bid_price = 0.0;
  double                ask_price = 0.0;
  double                VWAP = 0.0;
  double                limit_high = 0.0;
  double                limit_low = 0.0;
  unsigned int          bid_size = 0;
  unsigned int          ask_size = 0;
  unsigned int          trade_size = 0;
  unsigned int          volume = 0;
  unsigned int          prev_volume = 0;
  std::string           exchange_timestamp = "";
  unsigned int          exchange_sequence = 0;
  std::string           trade_date = "";
  std::string           trade_time = "";
  std::string           quote_time = "";
  std::string           quote_date = "";
  std::string           activity_time = "";
  std::string           activity_date = "";
private:
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
};
