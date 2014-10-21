#ifndef LIBMANAGER_HH_
#define LIBMANAGER_HH_


#include <map>
#include "LibLoader.hh"

class LibManager
{
public:
	LibManager();
	~LibManager();
	bool					LoadLibrary(const std::string& path);
	bool					LoadLibraryAs(const std::string& name, const std::string& path);
	bool					LoadFunction(const std::string& lib, const std::string& func);
	template<typename T> T	GetFunction(const std::string& lib, const std::string& func)
	{
		if (!_libs[lib])
			return nullptr;
		return reinterpret_cast<T>(_funcs[lib][func]);
	}
private:
	LibLoader*												_loader;
	std::map<std::string, LIB>								_libs;
	std::map< std::string, std::map<std::string, FUNC> >	_funcs;
};

#endif /* !LIBMANAGER_HH_ */