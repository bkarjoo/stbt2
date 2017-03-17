#include <cmath>
#include "message.h"

#include <iostream>
using std::cout; using std::endl;



void message::add_message_char(const char& c)
{

  if (is_field_delimiter(c)) {
    process_field_delimiter(c);
  } else {
    add_field_char(c);
  }
}

void message::message_complete()
{
  if (field_value  == "") return; // nothing to register
  if (field_code_value == "") register_simple(); else register_complex();
}

void message::process_field_delimiter(const char& c) {
  if (field_value == "" && field_code_value == "" && field_code != 0) {
    // the field delimiter is an exchange code
  } else if (c != 'f' && field_code == 'f' && (field_code_value == "60" || field_code_value == "52") && field_value == "") {
    // value is an exchange code
    field_value += c;
  } else {
    // at this point the field is complete
    if (field_code_value == "") register_simple(); else register_complex();
    field_code = c;
    field_code_value = "";
    field_value = "";
  }
}


void message::add_field_char(const char& c)
{
  if (c == ',')
    swap(field_code_value,field_value);
  else
    field_value += c;
}

void message::register_simple() {
  if (field_sequence.length() != 0) field_sequence += ',';
  field_sequence += field_code;

  switch (field_code) {
    case 'a':
      ask_price = set_price(field_value);
      break;
    case 'b':
      bid_price = set_price(field_value);
      break;
    case 'c':
      current_price = set_price(field_value);
      break;
    case 'd':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'e':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'f':
      // this is a precision setting
      set_precision();
      break;
    case 'g':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'h':
      day_high = set_price(field_value);
      break;
    case 'i':
      trade_size = set_volume(field_value);
      break;
    case 'j':
      bid_size = set_volume(field_value);
      break;
    case 'k':
      ask_size = set_volume(field_value);
      break;
    case 'l':
      day_low = set_price(field_value);
      break;
    case 'm':
      if (field_value != "252304")
      cout << "NONE USD CURRENCY DETECTED: field m" << field_value << endl;
      break;
    case 'n':
      // net change
      break;
    case 'o':
      open_price = set_price(field_value);
      break;
    case 'p':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'q':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'r':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 's':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 't':
      last_price = set_price(field_value);
      break;
    case 'u':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'v':
      volume = set_volume(field_value);
      break;
    case 'w':
      prev_volume = set_volume(field_value);
      break;
    case 'x':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;
      break;
    case 'y':
      close_price = set_price(field_value);
      break;
    case 'z':
      cout << "NOT IMPLEMENTED FIELD CODE: " << field_code <<  " value: " << field_value << endl;

      break;
  }
}

void message::register_complex() {
  if (field_sequence.length() != 0) field_sequence += ',';
  field_sequence += field_code;
  field_sequence += field_code_value;
  switch (field_code) {
    case 'f':
      register_complex_f();
      break;
    case 'g':
      register_complex_g();
      break;
    case 'z':
      register_complex_z();
      break;
  }
}

void message::register_complex_f() {
  unsigned short int fcv = 0;
  try {
    fcv = stoi(field_code_value);
    switch (fcv) {
      case 10:
        // percentage change
        break;
      case 11:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 12:
        // session volume
        session_volume = set_volume(field_value);
        break;
      case 13:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 14:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 15:
        swap(activity_date,field_value);
        break;
      case 16:
        swap(activity_time, field_value);
        break;
      case 17:
        // yesterday's trade date or timestamp
        break;
      case 18:
        if (field_value == "1") market_crossed = true;
        break;
      case 19:
        // number of trade blocks
        break;
      case 20:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 21:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 22:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 23:
        swap(trade_date,field_value);
        break;
      case 24:
        swap(trade_time,field_value);
        break;
      case 25:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 26:
        swap(quote_date,field_value);
        break;
      case 27:
        swap(quote_time,field_value);
        break;
      case 28:
        // VWAP
        VWAP = set_price(field_value);
        break;
      case 29:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 30:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 31:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 32:
        // unique exchange id
        break;
      case 33:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 34:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 35:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 36:
        // Identifies the current trading state of the IPO
        // TODO: for IPO strategies
        if (field_value == "1" || field_value == "21" || field_value == "22")
          {}
        else {
          if (field_value != "" && field_value != "0") {
            cout << "TODO: Implement f36 IPO state: " << field_value << endl;
            encountered_error = true;
          }
        }
        break;
      case 37:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 38:
        if (field_value != "" && field_value != "0") cout << "TODO: Implement f38 Issuer Financial Status: " << field_value << endl;
        break;
      case 39:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 40:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 41:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 42:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 43:
        // weighted average life (in days) of a money market as calculated in
        // accordance with the Money Market Reform Act
        break;
      case 44:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 45:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 46:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 47:
        // trade block volume
        break;
      case 48:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 49:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 50:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 51:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 52:
        // participant providing best ask at close
        break;
      case 53:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 54:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 55:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 56:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 57:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 58:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 59:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 60:
        // participant providing best bid at close
        break;
      case 61:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 62:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 63:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 64:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 65:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 66:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 67:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 68:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 69:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 70:
        // Composite quote condition
        break;
      case 71:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 72:
        if (field_value == "1") is_cycle_message = true;
        break;
      case 73:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 74:
        if (field_value != "0" && field_value != "15") cout << "TODO: Implement f74 OFFICIAL CLOSE SET: " << field_value << endl;
        if (field_value == "15") official_close_set = true;
        break;
      case 75:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 76:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 77:
        if (field_value != "") trade_size = set_volume(field_value);
        break;
      case 78:
        if (field_value != "") last_price = set_price(field_value);
        break;
      case 79:
        if (field_value != "")
        {
          auto trade_condition_code = stoi(field_value);
          tc = static_cast<trade_condition>(trade_condition_code);
        }
        break;
      case 80:
        // correction prev trade price, cancel trade, price to be corrected
        break;
      case 81:
        // correction new trade price
        break;
      case 82:
        // correction previous trade size
        break;
      case 83:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 84:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 85:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 86:
        // trade through expemt: 0 is not 1 is
        // TODO: check with primus, this might be FINRA ADF Prints
        break;
      case 87:
        // original trade through indicator
        break;
      case 88:
        // TODO: for hard to borrow strategies
        if (field_value != "" && field_value != "0") cout << "TODO: Implement f88 Shortsale restriction: " << field_value << endl;
        break;
      case 89:
        limit_high = set_price("field_value");
        break;
      case 90:
        limit_low = set_price("field_value");
        break;
      case 91:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 92:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 93:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 94:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 95:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 96:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 97:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 98:
        cout << "F Field f" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 99:
        // record reset
        break;
    }
  } catch (...) {
    encountered_error = true;
    cout << "ERROR ON FIELD CONVERSION: f" << field_code_value << " field value: " << field_value << endl;
  }
}

void message::register_complex_g() {
  unsigned short int fcv = 0;
  try {
    fcv = stoi(field_code_value);
    switch (fcv) {
      case 10:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 11:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 12:
        day_highest_bid = set_price(field_value);
        break;
      case 13:
        day_lowest_bid = set_price(field_value);
        break;
      case 14:
        // bid net change
        break;
      case 15:
        // closing bid price
        break;
      case 16:
        // yesterday's closing bid
        break;
      case 17:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 18:
        // historical trade price
        break;
      case 19:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 20:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 21:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 22:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 23:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 24:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 25:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 26:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 27:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 28:
        // ask closing price
        break;
      case 29:
        // yesterday ask closing price
        break;
      case 30:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 31:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 32:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 33:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 34:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 35:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 36:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 37:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 38:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 39:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 40:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 41:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 42:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 43:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 44:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 45:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 46:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 47:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 48:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 49:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 50:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 51:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 52:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 53:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 54:
        // TODO : 4 level sale condition is on the primus trade tape
        // TODO : odd lots come as this
        break;
      case 55:
        if (field_value != "")
        cout << "IMPLEMENT " << field_code << field_code_value << " : value = " << field_value << endl;
        break;
      case 56:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 57:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 58:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 59:
        // original trade condition
        break;
      case 60:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 61:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 62:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 63:
        // corection new trade size
        break;
      case 64:
        if (field_value != "")
        cout << "IMPLEMENT " << field_code << field_code_value << " : value = " << field_value << endl;
        break;
      case 65:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 66:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 67:
        // historicla trade size
        break;
      case 68:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 69:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 70:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 71:
        //   historical
                 break;
      case 72:
      // historical
        break;
      case 73:
        // historical quote condition
        break;
      case 74:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 75:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 76:
        // regional exchange where canceled trade was executed
        break;
      case 77:
        if (field_value != "")
        swap(exchange_timestamp,field_value);
        break;
      case 78:
        if (field_value != "")
        cout << "IMPLEMENT " << field_code << field_code_value << " : value = " << field_value << endl;
        break;
      case 79:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 80:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 81:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 82:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 83:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 84:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 85:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 86:
        cout << "NOT IMPLEMENTED " << field_code << field_code_value << endl;
        break;
      case 87:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 88:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 89:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 90:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 91:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 92:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 93:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 94:
        // correction of previous sequence
        break;
      case 95:
        // day of the week and reset message
        break;
      case 96:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 97:
        if (field_value != "")
        exchange_sequence = stoi(field_value);
        break;
      case 98:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
      case 99:
        cout << "G Field g" << fcv << " Not Implemented. Value: " << field_value << endl;
        break;
    }
  } catch (...) {
    encountered_error = true;
    cout << "ERROR ON FIELD CONVERSION: g" << field_code_value << endl;
  }
}


void message::register_complex_z() {
  cout << "Z FIELD NOT IMPLEMENTED" << endl;
}

void message::set_precision() {
  try {
    precision = stoi(field_value);
  } catch (...) {

  }
}

unsigned int message::set_volume(const std::string& vol)
{
  try {

    int i = stoi(vol);
    if (vol[0] == '-')
      return i * -1;
    else if (field_code_value == "12")
      return i;
    else
      return i * 100;
  } catch (...) {
    if (vol == "" || vol == ".") return 0;
    encountered_error = true;
    cout << "ERROR Convertying volume: " << vol << endl;
  }
}

double message::set_price(const std::string& price)
{
  try {

    double d = stod(price);
    return d / pow(10,precision);
  } catch (...) {
    if (field_code == 'f' && (field_code_value == "89" || field_code_value =="90"))
    {}
    else {
        if (price == "" || price == ".") return 0;
        encountered_error = true;
        //cout << "ERROR Converting price: " << price << endl;
    }
  }
}
