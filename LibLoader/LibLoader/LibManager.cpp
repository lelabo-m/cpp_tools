#include <iostream>
#include <vector>
#include <algorithm>
#include "LibManager.hh"

LibManager::LibManager()
{
#ifdef WIN32 // WINDOWS
#define LIBEXT ".dll"
	this->_loader = new WinLibLoader();
#else // LINUX
#define LIBEXT ".so"
	this->_loader = new LinuxLibLoader();
#endif
	this->_libs.clear();
	this->_funcs.clear();
}

LibManager::~LibManager()
{
	for (auto node : this->_libs)
	{
		this->_loader->CloseLib(node.second);
		node.second = nullptr;
	}
	this->_libs.clear();
	this->_funcs.clear();
	delete this->_loader;
}

bool	LibManager::LoadLibrary(const std::string& pathstring)
{
	boost::filesystem::path p = pathstring;

	if (boost::filesystem::exists(p))
	{
		if (boost::filesystem::is_directory(p))
		{

			std::vector<boost::filesystem::path> v;
			std::copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(v));
			std::sort(v.begin(), v.end());
			for (auto subpath : v)
			{
				if (subpath.extension().string() == LIBEXT)
					LoadLibrary(subpath.string());
			}
		}
		else if (boost::filesystem::is_regular(p) && p.extension().string() == LIBEXT)
		{
			LIB l = _loader->OpenLib(p.filename().string());
			_libs[p.stem().string()] = l;
		}
		else
			throw new LoadingException(std::string("Is not a valid lib : " + pathstring).c_str());
	}
	else
		throw new LoadingException(std::string("Path does not exist : " + pathstring).c_str());
	return true;
}

bool	LibManager::LoadLibraryAs(const std::string& name, const std::string& pathstring)
{
	boost::filesystem::path p = pathstring;

	if (boost::filesystem::exists(p))
	{
		if (boost::filesystem::is_regular(p) && p.extension().string() == LIBEXT)
		{
			LIB l = _loader->OpenLib(p.filename().string());
			_libs[name] = l;
		}
		else
			throw new LoadingException(std::string("Is not a valid lib : " + pathstring).c_str());
	}
	else
		throw new LoadingException(std::string("Path does not exist : " + pathstring).c_str());
	return true;
}

bool	LibManager::LoadFunction(const std::string& lib, const std::string& func)
{
	LIB	l = _libs[lib];
	if (!l)
		return false;
	FUNC f = _loader->OpenFunc(l, func);
	_funcs[lib][func] = f;
	return true;
}