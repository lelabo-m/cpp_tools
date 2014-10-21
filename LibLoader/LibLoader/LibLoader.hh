#ifndef LIBLOADER_HH_
#define LIBLOADER_HH_

#include <string>
#include <stdexcept>

#ifdef WIN32 // WINDOWS

#include <windows.h>
#include <boost\filesystem.hpp>
typedef HMODULE LIB;
typedef void* FUNC;

#else // LINUX

#include <dlfcn.h>
#include <boost/filesystem.hpp>
typedef void* LIB;
typedef void* FUNC;

#endif

// ########### Exception #########

class LoadingException : public std::runtime_error
{
public:
	LoadingException(const char *msg = "") : std::runtime_error(msg) {};
	LoadingException(const LoadingException& other) : std::runtime_error(other.what()) {}
	LoadingException(LoadingException&& other) : std::runtime_error(other.what()) {}
	LoadingException&	operator=(const LoadingException& other) { return *this; }
	LoadingException&	operator=(LoadingException&& other) { return *this; }
};

class LibLoader
{
public:
	LibLoader() {};
	virtual ~LibLoader() {};
	virtual LIB			OpenLib(const std::string& path) = 0;
	virtual FUNC		OpenFunc(LIB l, const std::string& fname) = 0;
	virtual bool		CloseLib(LIB l) = 0;
};

#ifdef WIN32 // WINDOWS

class WinLibLoader : public LibLoader
{
public:
	WinLibLoader();
	~WinLibLoader();
	LIB		OpenLib(const std::string& path);
	FUNC	OpenFunc(LIB l, const std::string& fname);
	bool	CloseLib(LIB l);
}; 

#else // LINUX

class LinuxLibLoader : public LibLoader
{
public:
	LinuxLibLoader();
	~LinuxLibLoader();
	LIB		OpenLib(const std::string& path);
	FUNC	OpenFunc(LIB l, const std::string& fname);
	bool	CloseLib(LIB l);
};

#endif

#endif /* !LIBLOADER_HH_ */