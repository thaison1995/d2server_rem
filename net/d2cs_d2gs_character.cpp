#include "d2cs_d2gs_character.h"

#include <string>
#include <ctime>
#include <cassert>

std::string assemble_charinfo(std::string& acctname, std::string charname, int char_class, int char_level,
	int exp_low, int char_status, std::string& portrait, int create_time, std::string realmname) {

	t_d2charinfo_file		d2charinfo;
	memset(&d2charinfo, 0, sizeof(t_d2charinfo_file));
	
	t_d2charinfo_header& ci_header = d2charinfo.header;
	t_d2charinfo_portrait& ci_portrait = d2charinfo.portrait;
	t_d2charinfo_summary& ci_summary = d2charinfo.summary;

	ci_header.magicword = D2CHARINFO_MAGICWORD;
	ci_header.version = D2CHARINFO_VERSION;
	ci_header.create_time = create_time;
	ci_header.last_time = time(NULL);
	strcpy_s(ci_header.account, MAX_ACCTNAME_LEN, acctname.c_str());
	strcpy_s(ci_header.charname, MAX_CHARNAME_LEN, charname.c_str());
	strcpy_s(ci_header.realmname, MAX_REALMNAME_LEN, realmname.c_str());

	ci_summary.experience = exp_low;
	ci_summary.charlevel = char_level;
	ci_summary.charclass = char_class;
	ci_summary.charstatus = char_status;

	assert(sizeof(t_d2charinfo_portrait) == portrait.length()+1);
	memcpy(&ci_portrait, portrait.c_str(), portrait.length());

	return std::string((const char*)&d2charinfo, sizeof(t_d2charinfo_file));
}