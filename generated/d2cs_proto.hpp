#ifndef INCLUDED_D2CS_PROTO_HPP
#define INCLUDED_D2CS_PROTO_HPP

#include <string>

#include <sstream>

#include "net/bn_types.h"

using string = std::string;

enum PROTO_AUTHREPLY: bn_int {
  D2CS_D2GS_AUTHREPLY_SUCCEED = 0,
  D2CS_D2GS_AUTHREPLY_BAD_VERSION = 1,
  D2CS_D2GS_AUTHREPLY_BAD_CHECKSUM = 2,
};

enum PROTO_CONTROL_CMD: bn_int {
  D2CS_D2GS_CONTROL_CMD_RESTART = 1,
  D2CS_D2GS_CONTROL_CMD_SHUTDOWN = 2,
  D2CS_D2GS_CONTROL_VALUE_DEFAULT = 0,
};

enum PROTO_GAME_DIFFICULTY: bn_byte {
  D2GAME_DIFFICULTY_NORMAL = 0,
  D2GAME_DIFFICULTY_NIGHTMARE = 1,
  D2GAME_DIFFICULTY_HELL = 2,
};

enum PROTO_CREATEGAME_RESULT: bn_int {
  D2GS_D2CS_CREATEGAME_SUCCEED = 0,
  D2GS_D2CS_CREATEGAME_FAILED = 1,
};

enum PROTO_JOINGAME_RESULT: bn_int {
  D2GS_D2CS_JOINGAME_SUCCEED = 0,
  D2GS_D2CS_JOINGAME_FAILED = 1,
};

enum PROTO_UPDATEGAMEINFO_FLAG: bn_int {
  D2GS_D2CS_UPDATEGAMEINFO_FLAG_UPDATE = 0,
  D2GS_D2CS_UPDATEGAMEINFO_FLAG_ENTER = 1,
  D2GS_D2CS_UPDATEGAMEINFO_FLAG_LEAVE = 2,
};

#define t_d2cs_d2gs_authreq_typecode 16
struct t_d2cs_d2gs_authreq
{
  t_d2cs_d2gs_header h;
  bn_int sessionnum;
  bn_int signlen;
  string realm_name;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&sessionnum, sizeof(bn_int));
    iss.read((char*)&signlen, sizeof(bn_int));
    std::getline(iss, realm_name, '\0');
  }
  std::string WriteAsString() {
    h.type = 16;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&sessionnum, sizeof(bn_int));
    oss.write((char*)&signlen, sizeof(bn_int));
    oss << realm_name;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_authreply_typecode 17
struct t_d2gs_d2cs_authreply
{
  t_d2cs_d2gs_header h;
  bn_int version;
  bn_int checksum;
  bn_int randnum;
  bn_int signlen;
  bn_basic sign[128];

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&version, sizeof(bn_int));
    iss.read((char*)&checksum, sizeof(bn_int));
    iss.read((char*)&randnum, sizeof(bn_int));
    iss.read((char*)&signlen, sizeof(bn_int));
    iss.read((char*)&sign[0], sizeof(bn_basic) * 128);
  }
  std::string WriteAsString() {
    h.type = 17;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&version, sizeof(bn_int));
    oss.write((char*)&checksum, sizeof(bn_int));
    oss.write((char*)&randnum, sizeof(bn_int));
    oss.write((char*)&signlen, sizeof(bn_int));
    oss.write((char*)&sign[0], sizeof(bn_basic) * 128);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_authreply_typecode 17
struct t_d2cs_d2gs_authreply
{
  t_d2cs_d2gs_header h;
  PROTO_AUTHREPLY reply;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&reply, sizeof(PROTO_AUTHREPLY));
  }
  std::string WriteAsString() {
    h.type = 17;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&reply, sizeof(PROTO_AUTHREPLY));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_setgsinfo_typecode 18
struct t_d2gs_d2cs_setgsinfo
{
  t_d2cs_d2gs_header h;
  bn_int maxgame;
  bn_int gameflag;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&maxgame, sizeof(bn_int));
    iss.read((char*)&gameflag, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 18;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&maxgame, sizeof(bn_int));
    oss.write((char*)&gameflag, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_echoreq_typecode 19
struct t_d2cs_d2gs_echoreq
{
  t_d2cs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 19;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_getconf_typecode 21
struct t_d2gs_d2cs_getconf
{
  t_d2cs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 21;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_echoreply_typecode 19
struct t_d2gs_d2cs_echoreply
{
  t_d2cs_d2gs_header h;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
  }
  std::string WriteAsString() {
    h.type = 19;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_control_typecode 20
struct t_d2cs_d2gs_control
{
  t_d2cs_d2gs_header h;
  PROTO_CONTROL_CMD cmd;
  bn_int value;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&cmd, sizeof(PROTO_CONTROL_CMD));
    iss.read((char*)&value, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 20;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&cmd, sizeof(PROTO_CONTROL_CMD));
    oss.write((char*)&value, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_setinitinfo_typecode 21
struct t_d2cs_d2gs_setinitinfo
{
  t_d2cs_d2gs_header h;
  bn_int time;
  bn_int gs_id;
  bn_int ac_version;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&time, sizeof(bn_int));
    iss.read((char*)&gs_id, sizeof(bn_int));
    iss.read((char*)&ac_version, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 21;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&time, sizeof(bn_int));
    oss.write((char*)&gs_id, sizeof(bn_int));
    oss.write((char*)&ac_version, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_setconffile_typecode 22
struct t_d2cs_d2gs_setconffile
{
  t_d2cs_d2gs_header h;
  bn_int size;
  bn_int reserved1;
  string conf;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&size, sizeof(bn_int));
    iss.read((char*)&reserved1, sizeof(bn_int));
    std::getline(iss, conf, '\0');
  }
  std::string WriteAsString() {
    h.type = 22;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&size, sizeof(bn_int));
    oss.write((char*)&reserved1, sizeof(bn_int));
    oss << conf;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_creategamereq_typecode 32
struct t_d2cs_d2gs_creategamereq
{
  t_d2cs_d2gs_header h;
  bn_byte ladder;
  bn_byte expansion;
  PROTO_GAME_DIFFICULTY difficulty;
  bn_byte hardcore;
  string gamename;
  string gamepass;
  string gamedesc;
  string creator_acctname;
  string creator_charname;
  string creator_ipaddr;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&ladder, sizeof(bn_byte));
    iss.read((char*)&expansion, sizeof(bn_byte));
    iss.read((char*)&difficulty, sizeof(PROTO_GAME_DIFFICULTY));
    iss.read((char*)&hardcore, sizeof(bn_byte));
    std::getline(iss, gamename, '\0');
    std::getline(iss, gamepass, '\0');
    std::getline(iss, gamedesc, '\0');
    std::getline(iss, creator_acctname, '\0');
    std::getline(iss, creator_charname, '\0');
    std::getline(iss, creator_ipaddr, '\0');
  }
  std::string WriteAsString() {
    h.type = 32;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&ladder, sizeof(bn_byte));
    oss.write((char*)&expansion, sizeof(bn_byte));
    oss.write((char*)&difficulty, sizeof(PROTO_GAME_DIFFICULTY));
    oss.write((char*)&hardcore, sizeof(bn_byte));
    oss << gamename;
    oss.put((char)0);
    oss << gamepass;
    oss.put((char)0);
    oss << gamedesc;
    oss.put((char)0);
    oss << creator_acctname;
    oss.put((char)0);
    oss << creator_charname;
    oss.put((char)0);
    oss << creator_ipaddr;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_creategamereply_typecode 32
struct t_d2gs_d2cs_creategamereply
{
  t_d2cs_d2gs_header h;
  PROTO_CREATEGAME_RESULT result;
  bn_int gameid;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&result, sizeof(PROTO_CREATEGAME_RESULT));
    iss.read((char*)&gameid, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 32;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&result, sizeof(PROTO_CREATEGAME_RESULT));
    oss.write((char*)&gameid, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2cs_d2gs_joingamereq_typecode 33
struct t_d2cs_d2gs_joingamereq
{
  t_d2cs_d2gs_header h;
  bn_int gameid;
  bn_int token;
  bn_int vip_expire;
  string charname;
  string acctname;
  string client_ipaddr;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&gameid, sizeof(bn_int));
    iss.read((char*)&token, sizeof(bn_int));
    iss.read((char*)&vip_expire, sizeof(bn_int));
    std::getline(iss, charname, '\0');
    std::getline(iss, acctname, '\0');
    std::getline(iss, client_ipaddr, '\0');
  }
  std::string WriteAsString() {
    h.type = 33;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&gameid, sizeof(bn_int));
    oss.write((char*)&token, sizeof(bn_int));
    oss.write((char*)&vip_expire, sizeof(bn_int));
    oss << charname;
    oss.put((char)0);
    oss << acctname;
    oss.put((char)0);
    oss << client_ipaddr;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_joingamereply_typecode 33
struct t_d2gs_d2cs_joingamereply
{
  t_d2cs_d2gs_header h;
  PROTO_JOINGAME_RESULT result;
  bn_int gameid;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&result, sizeof(PROTO_JOINGAME_RESULT));
    iss.read((char*)&gameid, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 33;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&result, sizeof(PROTO_JOINGAME_RESULT));
    oss.write((char*)&gameid, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_updategameinfo_typecode 34
struct t_d2gs_d2cs_updategameinfo
{
  t_d2cs_d2gs_header h;
  PROTO_UPDATEGAMEINFO_FLAG flag;
  bn_int gameid;
  bn_int charlevel;
  bn_int charclass;
  bn_int addr;
  string charname;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&flag, sizeof(PROTO_UPDATEGAMEINFO_FLAG));
    iss.read((char*)&gameid, sizeof(bn_int));
    iss.read((char*)&charlevel, sizeof(bn_int));
    iss.read((char*)&charclass, sizeof(bn_int));
    iss.read((char*)&addr, sizeof(bn_int));
    std::getline(iss, charname, '\0');
  }
  std::string WriteAsString() {
    h.type = 34;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&flag, sizeof(PROTO_UPDATEGAMEINFO_FLAG));
    oss.write((char*)&gameid, sizeof(bn_int));
    oss.write((char*)&charlevel, sizeof(bn_int));
    oss.write((char*)&charclass, sizeof(bn_int));
    oss.write((char*)&addr, sizeof(bn_int));
    oss << charname;
    oss.put((char)0);
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#define t_d2gs_d2cs_closegame_typecode 35
struct t_d2gs_d2cs_closegame
{
  t_d2cs_d2gs_header h;
  bn_int gameid;

  void ReadFromString(std::string& s) {
    std::istringstream iss(s);
    iss.read((char*)&h, sizeof(t_d2cs_d2gs_header));
    iss.read((char*)&gameid, sizeof(bn_int));
  }
  std::string WriteAsString() {
    h.type = 35;
    std::ostringstream oss;
    oss.write((char*)&h, sizeof(t_d2cs_d2gs_header));
    oss.write((char*)&gameid, sizeof(bn_int));
    h.size = (bn_short)oss.tellp();
    oss.seekp(0);
    oss.write((char*)&h.size, sizeof(bn_int));
    return oss.str();
  }
};

#endif
