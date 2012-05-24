
#include "stdafx.h"
using namespace std;

// Recursive directory traversal using the Win32 API
bool ListFiles(string path, string mask, vector<string>& files)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	string fullpath;
	stack<string> folders;
	//folders.push(
	folders.push(path);
	files.clear();
	
	while (!folders.empty())
	{
		path = folders.top();
		fullpath = path + "\\" + mask;
		folders.pop();

		hFind = FindFirstFile(fullpath.c_str(), &fdata);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(fdata.cFileName, ".") != 0 &&
                    strcmp(fdata.cFileName, "..") != 0)
				{
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						folders.push(path + "\\" + fdata.cFileName);
					}
					else
					{
						files.push_back(path + "\\" + fdata.cFileName);
					}
				}
			}
			while (FindNextFile(hFind, &fdata) != 0);
		}

		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);

			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}

int main(int argc, char* argv[])
{
	vector<string> files;

	if (!ListFiles("D:\\Music", "*", files))
		exit(0);
		

	for (vector<string>::iterator iter = files.begin(); iter != files.end(); ++iter)
		{
			//iter->replace(
			wcout << iter->c_str() << endl;
		}
		//wstring a=L" hi\\there";
		//wstring b=L"\\";
		//wstring c=L"\\\\";
		///size_t pos;
		//pos=a.find(b);
		//a.replace(pos,b.length(),c);
		//wcout<<L"\n"<<a;
	_getch();
	return false;
}
