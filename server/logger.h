#ifndef INCLUDED_SERVER_LOGGER_H
#define INCLUDED_SERVER_LOGGER_H

#define GOOGLE_GLOG_DLL_DECL
#undef ERROR
#include <glog/logging.h>

#ifdef _DEBUG
#pragma comment(lib, "glogd.lib")
#else
#pragma comment(lib, "glog.lib")
#endif

#endif