#ifndef INCLUDED_D2CS_PROTO_HPP
#define INCLUDED_D2CS_PROTO_HPP

#include <string>
#include <sstream>
#include <WinSock2.h>

#include "net/bn_types.h"

using string = std::string;

enum PROTO_DATATYPE: bn_short {
  D2GS_DATA_CHARSAVE = 1,
  D2GS_DATA_PORTRAIT = 2,
};

enum PROTO_SAVE_RESULT: bn_int {
  D2DBS_SAVE_DATA_SUCCESS = 0,
  D2DBS_SAVE_DATA_FAILED = 1,
};

enum PROTO_GET_DATA_RESULT: bn_int {
  D2DBS_GET_DATA_SUCCESS = 0,
  D2DBS_GET_DATA_FAILED = 1,
  D2DBS_GET_DATA_CHARLOCKED = 2,
};

#define t_d2gs_d2dbs_generic_typecode 0
struct t_d2gs_d2dbs_generic
{
  t_d2dbs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 0;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2dbs_save_data_request_typecode 48
struct t_d2gs_d2dbs_save_data_request
{
  t_d2dbs_d2gs_header h;
  PROTO_DATATYPE datatype;
  bn_short datalen;
  string acctname;
  string charname;
  string realmname;
  std::string data;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&datatype, sizeof(PROTO_DATATYPE));
    iss.read((char*)&datalen, sizeof(bn_short));
    std::getline(iss, acctname, '\0');
    std::getline(iss, charname, '\0');
    std::getline(iss, realmname, '\0');
    data.resize(datalen);
    if (datalen > 0) iss.read((char*)&data.front(), sizeof(char) * datalen);
  }
  std::string WriteAsString() {
    h.type = 48;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&datatype, sizeof(PROTO_DATATYPE));
    oss.write((char*)&datalen, sizeof(bn_short));
    oss << acctname;
    oss.put((char)0);
    oss << charname;
    oss.put((char)0);
    oss << realmname;
    oss.put((char)0);
    if (datalen > 0) oss.write((char*)&data.front(), sizeof(char) * datalen);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2dbs_d2gs_save_data_reply_typecode 48
struct t_d2dbs_d2gs_save_data_reply
{
  t_d2dbs_d2gs_header h;
  PROTO_SAVE_RESULT result;
  PROTO_DATATYPE datatype;
  string charname;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&result, sizeof(PROTO_SAVE_RESULT));
    iss.read((char*)&datatype, sizeof(PROTO_DATATYPE));
    std::getline(iss, charname, '\0');
  }
  std::string WriteAsString() {
    h.type = 48;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&result, sizeof(PROTO_SAVE_RESULT));
    oss.write((char*)&datatype, sizeof(PROTO_DATATYPE));
    oss << charname;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2dbs_get_data_request_typecode 49
struct t_d2gs_d2dbs_get_data_request
{
  t_d2dbs_d2gs_header h;
  PROTO_DATATYPE datatype;
  string acctname;
  string charname;
  string realmname;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&datatype, sizeof(PROTO_DATATYPE));
    std::getline(iss, acctname, '\0');
    std::getline(iss, charname, '\0');
    std::getline(iss, realmname, '\0');
  }
  std::string WriteAsString() {
    h.type = 49;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&datatype, sizeof(PROTO_DATATYPE));
    oss << acctname;
    oss.put((char)0);
    oss << charname;
    oss.put((char)0);
    oss << realmname;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2dbs_d2gs_get_data_reply_typecode 49
struct t_d2dbs_d2gs_get_data_reply
{
  t_d2dbs_d2gs_header h;
  PROTO_GET_DATA_RESULT result;
  bn_int charcreatetime;
  bn_int allowladder;
  PROTO_DATATYPE datatype;
  bn_short datalen;
  string charname;
  std::string data;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&result, sizeof(PROTO_GET_DATA_RESULT));
    iss.read((char*)&charcreatetime, sizeof(bn_int));
    iss.read((char*)&allowladder, sizeof(bn_int));
    iss.read((char*)&datatype, sizeof(PROTO_DATATYPE));
    iss.read((char*)&datalen, sizeof(bn_short));
    std::getline(iss, charname, '\0');
    data.resize(datalen);
    if (datalen > 0) iss.read((char*)&data.front(), sizeof(char) * datalen);
  }
  std::string WriteAsString() {
    h.type = 49;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&result, sizeof(PROTO_GET_DATA_RESULT));
    oss.write((char*)&charcreatetime, sizeof(bn_int));
    oss.write((char*)&allowladder, sizeof(bn_int));
    oss.write((char*)&datatype, sizeof(PROTO_DATATYPE));
    oss.write((char*)&datalen, sizeof(bn_short));
    oss << charname;
    oss.put((char)0);
    if (datalen > 0) oss.write((char*)&data.front(), sizeof(char) * datalen);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2dbs_update_ladder_typecode 50
struct t_d2gs_d2dbs_update_ladder
{
  t_d2dbs_d2gs_header h;
  bn_int charlevel;
  bn_int charexplow;
  bn_int charexphigh;
  bn_short charclass;
  bn_short charstatus;
  string charname;
  string realmname;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&charlevel, sizeof(bn_int));
    iss.read((char*)&charexplow, sizeof(bn_int));
    iss.read((char*)&charexphigh, sizeof(bn_int));
    iss.read((char*)&charclass, sizeof(bn_short));
    iss.read((char*)&charstatus, sizeof(bn_short));
    std::getline(iss, charname, '\0');
    std::getline(iss, realmname, '\0');
  }
  std::string WriteAsString() {
    h.type = 50;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&charlevel, sizeof(bn_int));
    oss.write((char*)&charexplow, sizeof(bn_int));
    oss.write((char*)&charexphigh, sizeof(bn_int));
    oss.write((char*)&charclass, sizeof(bn_short));
    oss.write((char*)&charstatus, sizeof(bn_short));
    oss << charname;
    oss.put((char)0);
    oss << realmname;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2dbs_char_lock_typecode 51
struct t_d2gs_d2dbs_char_lock
{
  t_d2dbs_d2gs_header h;
  bn_int lockstatus;
  string acctname;
  string charname;
  string realmname;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
    iss.read((char*)&lockstatus, sizeof(bn_int));
    std::getline(iss, acctname, '\0');
    std::getline(iss, charname, '\0');
    std::getline(iss, realmname, '\0');
  }
  std::string WriteAsString() {
    h.type = 51;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    oss.write((char*)&lockstatus, sizeof(bn_int));
    oss << acctname;
    oss.put((char)0);
    oss << charname;
    oss.put((char)0);
    oss << realmname;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2dbs_d2gs_echoreq_typecode 52
struct t_d2dbs_d2gs_echoreq
{
  t_d2dbs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 52;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2dbs_echoreply_typecode 52
struct t_d2gs_d2dbs_echoreply
{
  t_d2dbs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2dbs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 52;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2dbs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#endif
