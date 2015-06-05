#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <cstdlib>
#include <vector>
#include <stack>
#include <string>

#ifdef _WIN32
#include <WinSock2.h>
#define SLASHSEP "\\"
#define OTHERSEP "/"
#else
#define SLASHSEP "/"
#define OTHERSEP "\\"
#endif

#define ALLSEP (SLASHSEP "" OTHERSEP)

namespace FileSystem
{
	enum class FileType { INVALID, REGULAR, DIRECTORY };

}

#ifdef _WIN32

	#include <windows.h>
	
	inline std::string		to_string(const std::wstring& s)
	{
		return std::string(s.begin(), s.end());
	}
	inline std::wstring		to_string(const std::string& s)
	{
		return std::wstring(s.begin(), s.end());
	}

	inline FileSystem::FileType	GetFileType(const std::string& path)
	{
		std::wstring	wstr(path.begin(), path.end());
		unsigned long attribute = GetFileAttributesW(wstr.c_str());
		FileSystem::FileType type = FileSystem::FileType::INVALID;
		if (attribute == INVALID_FILE_ATTRIBUTES)
			return type;
		type = (!(attribute & FILE_ATTRIBUTE_DIRECTORY)) ? (FileSystem::FileType::REGULAR) : (FileSystem::FileType::DIRECTORY);
		return (type);
	}

	inline void		ListDir(const std::string& spath, const std::string& smask, std::vector<std::string>& files)
	{
		std::wstring	path = to_string(spath);
		std::wstring	mask = to_string(smask);

		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAW ffd;
		std::wstring spec;
		std::stack<std::wstring> directories;

		directories.push(path);
		files.clear();

		while (!directories.empty())
		{
			path = directories.top();
			spec = path + L"\\" + mask;
			directories.pop();

			hFind = FindFirstFileW(spec.c_str(), &ffd);
			if (hFind == INVALID_HANDLE_VALUE)
				return;
			while (FindNextFileW(hFind, &ffd) != 0)
			{
				if (std::wcscmp(ffd.cFileName, L".") != 0 && std::wcscmp(ffd.cFileName, L"..") != 0)
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						directories.push(path + L"\\" + ffd.cFileName);
					else
						files.push_back(to_string(path + L"\\" + ffd.cFileName));
				}
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				FindClose(hFind);
				return;
			}
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
		return;
	}
	inline void		ListDir(const std::string& spath, std::vector<std::string>& dir)
	{
		std::wstring	path = to_string(spath);

		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATAW ffd;
		std::wstring spec;
		std::stack<std::wstring> directories;

		directories.push(path);

		while (!directories.empty())
		{
			path = directories.top();
			spec = path + L"\\*";
			directories.pop();

			hFind = FindFirstFileW(spec.c_str(), &ffd);
			if (hFind == INVALID_HANDLE_VALUE)
				return;
			while (FindNextFileW(hFind, &ffd) != 0)
			{
				if (std::wcscmp(ffd.cFileName, L".") != 0 && std::wcscmp(ffd.cFileName, L"..") != 0)
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						dir.push_back(to_string(path + L"\\" + ffd.cFileName));
						directories.push(path + L"\\" + ffd.cFileName);
					}
				}
			}
			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				FindClose(hFind);
				return;
			}
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
		return;
	}
	inline bool	make_directory(const std::string& path)
	{
		if (CreateDirectoryW(to_string(path).c_str(), NULL) == 0)
			return false;
		return true;
	}

#else // Unix

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>

inline FileSystem::FileType	GetFileType(const std::string& path)
{
	struct stat		buf;
	if (stat(path.c_str(), &buf) != 0)
		return FileSystem::FileType::INVALID;
	return (S_ISREG(buf.st_mode) ? (FileSystem::FileType::REGULAR) : S_ISDIR(buf.st_mode)
			? (FileSystem::FileType::DIRECTORY) : (FileSystem::FileType::INVALID));
}

inline void		ListDir(const std::string& path, const std::string mask, std::vector<std::string>& files)
{
	std::stack<std::string>	directories;
	glob_t		glob_result;

	directories.push(path);
	while (directories.size())
	{
		std::string	search = directories.top() + "/" + mask;
		directories.pop();
		glob(search.c_str(), GLOB_TILDE, NULL ,&glob_result);
		for(size_t i = 0; i < glob_result.gl_pathc; ++i)
		{
			std::string res(glob_result.gl_pathv[i]);
			if (res != "." && res != ".." && GetFileType(res) == FileSystem::FileType::DIRECTORY)
				directories.push(res);
			else
				files.push_back(res);
		}
		globfree(&glob_result);
	}
}

inline void		ListDir(const std::string& path, std::vector<std::string>& dir)
{
	std::stack<std::string>	directories;
	glob_t		glob_result;

	directories.push(path);
	while (directories.size())
	{
		std::string	search = directories.top() + "/*";
		directories.pop();
		glob(search.c_str(), GLOB_TILDE, NULL, &glob_result);
		for (size_t i = 0; i < glob_result.gl_pathc; ++i)
		{
			std::string res(glob_result.gl_pathv[i]);
			if (res != "." && res != ".." && GetFileType(res) == FileSystem::FileType::DIRECTORY)
			{
				directories.push(res);
				dir.push_back(res);
			}
		}
		globfree(&glob_result);
	}
}


inline bool	make_directory(const std::string& path)
{
	if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
		return false;
	return true;
}
#endif

namespace FileSystem
{
	inline FileType	TypeOfFile(const std::string& path)
	{
		return GetFileType(path);
	}
	inline void	ListDirectory(const std::string& path, const std::string& mask, std::vector<std::string>& list)
	{
		ListDir(path, mask, list);
	}
	inline void	ListDirectory(const std::string& path, std::vector<std::string>& list)
	{
		ListDir(path, list);
	}
	inline bool	Mkdir(const std::string& path)
	{
		return make_directory(path);
	}
	inline std::string	to_string(const FileType f)
	{
		switch (f)
		{
			case FileType::INVALID:
				return "INVALID";
			case FileType::REGULAR:
				return "REGULAR";
			case FileType::DIRECTORY:
				return "DIRECTORY";
		}
		return "INVALID";
	}
	inline	std::string	DirName(const std::string& path)
	{
		auto it = path.find_last_of(ALLSEP);
		if (it == std::string::npos || (it + 1) == path.size())
			return "";
		return path.substr(0, it);
	}
	inline	std::string	FileName(const std::string& path)
	{
		auto  it = path.find(SLASHSEP);
		if (it == std::string::npos)
			return path;

		it = path.find_last_of(ALLSEP);
		if (it == std::string::npos || (it + 1) == path.size())
		{
			return "";
		}
		return path.substr(it + 1, path.size());
	}
	inline std::string	FileExt(const std::string& path)
	{
		auto filename = FileName(path);
		auto it = filename.find_last_of(".");
		if (it == std::string::npos || (it + 1) == path.size())
			return filename;
		return filename.substr(it, filename.size());
	}
	inline	std::string	FileShortName(const std::string& path)
	{
		auto filename = FileName(path);
		auto it = filename.find_last_of(".");
		if (it == std::string::npos || (it + 1) == path.size() || it == 0)
			return filename;
		return filename.substr(0, it);
	}
	inline void	PathSegment(const std::string& path, std::vector<std::string>& dir)
	{
		size_t	it;
		std::string currentpath = path;
		if ((it = currentpath.find_last_of(ALLSEP)) != std::string::npos && it != 0)
		{
			PathSegment(currentpath.substr(0, it), dir);
			if (it + 1 != currentpath.size())
				dir.push_back(currentpath.substr(it + 1, currentpath.size()));
		}
		else
		{
			if (currentpath.find(OTHERSEP) != std::string::npos)
				currentpath.replace(currentpath.find(OTHERSEP), 1, SLASHSEP);
			dir.push_back(currentpath);
		}
	}
	inline std::string	BuildPath(const std::string& path)
	{
		std::vector<std::string>	dir;
		PathSegment(path, dir);
		std::string pathtest;
		for (auto directory : dir)
		{
			pathtest += directory;
			if (TypeOfFile(pathtest) == FileType::REGULAR)
				return "";
			if (TypeOfFile(pathtest) == FileType::INVALID)
			{
				if (Mkdir(pathtest) == false)
					return "";
			}
			pathtest += SLASHSEP;
		}
		return pathtest;
	}

};


#endif /* !FILESYSTEM_H_ */