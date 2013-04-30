

class DirUtils
{
public:
	static BOOL FolderExist(CString strPath)
	{
		WIN32_FIND_DATA wfd;
		BOOL rValue = FALSE;
		HANDLE hFind = FindFirstFile(strPath, &wfd);
		if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			rValue = TRUE;
		}
		FindClose(hFind);
		return rValue;
	}
	
	static BOOL FileExist(CString strFileName)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(strFileName, &FindFileData);
		
		if (hFind == INVALID_HANDLE_VALUE)
			return false;
		else
		{
			FindClose(hFind);
			return true;
		}
		return false;
	}

	static CString getExeDirectory()
	{
		CString cstrDir;
		TCHAR cBuffer[260];
		GetCurrentDirectory(MAX_PATH, cBuffer);
		CString cstrPath(cBuffer);
		if (FolderExist(cstrPath + _T("\\symbols")))
		{
			cstrDir = cstrPath;
		}
		else
		{
			int nCount = cstrPath.ReverseFind(_T('\\'));
			cstrPath.Delete(nCount, cstrPath.GetLength() - nCount);
			
			if (FolderExist(cstrPath + _T("\\symbols")))
			{
				cstrDir = cstrPath;
			}
			else
			{
				MyMessageBox_Error(_T("getExeDirectory Error."), _T("Error"));
				return _T("");
			}
		}
		cstrDir += _T("\\");
		return cstrDir;
	}
};