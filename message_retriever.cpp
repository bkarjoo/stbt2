#include "message_retriever.h"

#include <iostream>
using namespace std;



void message_retriever::run()
{
    // TODO : implement ignoring packets based on time
    // TODO : implement ignoring messages based on symbol
    // TODO : implement getting message header here and only creating message if symbol needed
    _setmaxstdio(2048);
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
    dump_messages();
}

void message_retriever::process_packet_delimiter(const char&)
{
    //std::cout << "packet delimiter: " << packet_header << std::endl;
    if (mp != nullptr) {
        mp->message_complete();
        insert_message(mp->symbol,mp->msg_str);
        if (mp->encountered_error) {
            std::ofstream out("out.txt");
            std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
            std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

            cout << endl << header_str << mp->msg_str << endl;
            //throw 20;
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
        insert_message(mp->symbol,mp->msg_str);
        if (mp->encountered_error) {
            std::ofstream out("out.txt");
            std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
            std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

            cout << endl << header_str << mp->msg_str << endl;
            //throw 20;
        }
    }
    msg_exch = 0;
    symbol = "";
    msg_header_complete = false;
    msg_delim = c;
    header_str = "";
    if (mode == read_mode::PACKET) {
        // add packet header to all the strs
        char x = 31;
        std::string pheader = "";
        pheader += 31;
        pheader += packet_header;
        for (auto& a : message_strs) a.second += pheader;
        cout << packet_header << endl;
    }
    if (mode != read_mode::IGNORE_PACKET) mode = read_mode::MESSAGE;
}

void message_retriever::add_packet_header_char(const char& c)
{

    // if (c == ':' && packet_header.length() < 3) {
    //   auto hour = stoi(packet_header);
    //   if (hour > 0 || hour < 9) mode = read_mode::IGNORE_PACKET;
    // }
    if (c >= 65 && c <= 90) {
        cout << "ERROR: " << packet_header << c << endl;
        // throw 20;
    }
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
            msg_header_complete = true;
            mp = std::make_shared<message>();

            mp->delim = msg_delim;
            mp->msg_str += msg_delim;

            mp->exchange = msg_exch;
            mp->msg_str += msg_exch;

            swap(mp->symbol,symbol);
            mp->msg_str += mp->symbol;

            mp->add_message_char(c);


        } else {
            symbol += c;
        }
    }
}

void message_retriever::insert_message(const std::string& symbol, const std::string& msg) {
    cout << "atempting to add to map" << msg << endl;
    message_strs[symbol] += msg;
    cout << "added to str on map" << msg << endl;
    if (message_strs[symbol].length() > 350000) {

        if (outs.size() < 2045) {
            cout << "trying to find ofstream" << endl;
            auto it = outs.find("symbol");
            if (it == outs.end()) {
                cout << "ofstream not found" << endl;
                auto name = output_path + symbol + ".CAP";
                outs[symbol] = std::make_shared<std::ofstream>(name,ios::app);
                cout << "ofstream created in map" << endl;
            }
            *outs[symbol] << message_strs[symbol];
            cout << "wrote to ofstream on map" << endl;
        } else {
            cout << "no more room for ofstreams" << endl;
            auto name = output_path + symbol + ".CAP";
            ofstream of(name,ios::app);
            of << message_strs[symbol];
            of.close();
            cout << "open and closed ofstream" << endl;
        }
        message_strs[symbol] = "";
        cout << "dumped and cleared 350000 char str" << endl;
    }
}

void message_retriever::dump_messages() {
    for (auto const& a : message_strs) {
        auto it = outs.find(a.first);
        if (it == outs.end()) {
            auto name = output_path + a.first + ".CAP";
            ofstream of(name,ios::app);
            of << a.second;
            of.close();
        } else {
            *outs[a.first] << a.second;
            outs[a.first]->close();
        }
    }
}
