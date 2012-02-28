// LXLocalWebSite.h : Declaration of the CLXLocalWebSite

#ifndef __LXLOCALWEBSITE_H_
#define __LXLOCALWEBSITE_H_

#include "resource.h"       // main symbols
#include "LXWebSiteCP.h"

/////////////////////////////////////////////////////////////////////////////
// CLXLocalWebSite
class ATL_NO_VTABLE CLXLocalWebSite : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLXLocalWebSite, &CLSID_LXLocalWebSite>,
	public IConnectionPointContainerImpl<CLXLocalWebSite>,
	public IDispatchImpl<ILXLocalWebSite, &IID_ILXLocalWebSite, &LIBID_LXWEBSITELib>,
	public CMessageMap,
	public CProxy_ILXLocalWebSiteEvents< CLXLocalWebSite >
{
public:

	class CSiteDir;

	class CSiteItem
	{
	public:
		int m_type;

		CSiteItem()
		{
			m_parent = NULL;
			m_pWebSite = NULL;
			m_dbid = 0;
			memset(&m_wfd, 0, sizeof(m_wfd));
		}
		
		CSiteDir* m_parent;

		BOOL	m_bFoundOnDisk;
		DWORD	m_dbid;

		CLXLocalWebSite* m_pWebSite;
		WIN32_FIND_DATA m_wfd;
		CString	m_fullPathName;

		//void DeselectAll();

		HRESULT AddSiteItemToDatabase();
		HRESULT RemoveSiteItemFromDatabase();
		HRESULT UpdateDatabase();

		CString GetFullPathName()
		{
			return m_fullPathName;
		}

		CString GetFullDisplayName();
	};

	class CSiteDir : public CSiteItem
	{
	public:
		CSiteDir()
		{
			m_type = 1;
		}

		CUPtrList m_childList;
		UPOSITION AddChildTail(CSiteItem* pChild)
		{
			pChild->m_parent = this;
			UPOSITION pos = m_childList.AddTail(pChild);
			return pos;
		}

		void ScanFiles(BOOL bCheckExisting = FALSE, BOOL bUpdateDB = FALSE, CSiteItem** pActive = NULL);
		//void ScanFTPFiles(BOOL bCheckExisting = FALSE);

		CSiteItem* FileExists(LPCTSTR pathname, int type);

		CSiteItem* FindFilePathName(LPCTSTR pathName, int type);

		CSiteItem* FindItemByID(DWORD id, int type);

		HRESULT UpdateOutLinks();

		void BuildFromDatabase();

		/*
		virtual CUString GetFullPathName()
		{
			return m_fullpathName;
		}
		*/
	};

	class CSiteFile : public CSiteItem
	{
	public:
		CSiteFile()
		{
			m_type = 2;

			m_ownerDocument_id = 0;
		}

		DWORD m_ownerDocument_id;

		HRESULT UpdateOutLinks();
		void SaveDOMElementToDatabase(ILDOMElement* parent, int parentNode_id, int index);

	//	void UpdateDatabase();
		void RemoveFromDatabase();

		/*
		virtual CUString GetFullPathName()
		{
			CSiteDir* pDir = (CSiteDir*)m_parent;

			TCHAR pathName[_MAX_PATH];
			_makepath(pathName, NULL, pDir->m_pathName, m_wfd.cFileName, NULL);

			return pathName;
		}
		*/
	};

	CLXLocalWebSite() : m_cwnd("static", this, 1)
	{
		m_pRootDir = NULL;
		m_dwChangeThreadID = 0L;
		m_hChangeThread = NULL;

		m_homepageFileID = 0L;
	}

	int FinalConstruct();
	void FinalRelease();

	CContainedWindow m_cwnd;

	CSiteDir* m_pRootDir;
	DWORD m_dwChangeThreadID;
	HANDLE m_hChangeThread;
	_bstr_t	m_rootPath;

	ADODB::_ConnectionPtr m_siteDataConnection;
	WCHAR m_connstr[512];


//	ADODB::_RecordsetPtr m_rsFiles;

	DWORD m_homepageFileID;

	void ScanFiles();
	HRESULT GetHomepageFileIdFromDB();
	_bstr_t GetFolderDisplayPathRecursive(DWORD folder_id, WCHAR* pathDelimiter);

	void SetElementFromRecordset(ADODB::_RecordsetPtr recordset, ILDOMElement* element);

	DWORD AddNewFile(DWORD parent_id, long type, BSTR filename, DWORD file_size, double file_date);

	HRESULT RemoveFileFromDatabase(DWORD parent_id, DWORD id);

	void CreateTable_websettings(ADOX::_CatalogPtr catalog);
	void CreateTable_files(ADOX::_CatalogPtr catalog);
	void CreateTable_files_data(ADOX::_CatalogPtr catalog);
	void CreateTable_links(ADOX::_CatalogPtr catalog);
	void CreateTable_tasks(ADOX::_CatalogPtr catalog);

	long GetFileNameID(long parent_id, BSTR filename);
	long GetPathFileNameID(BSTR filePath);

DECLARE_REGISTRY_RESOURCEID(IDR_LXLOCALWEBSITE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLXLocalWebSite)
	COM_INTERFACE_ENTRY(ILXLocalWebSite)
//	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CLXLocalWebSite)
	CONNECTION_POINT_ENTRY(DIID__ILXLocalWebSiteEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CLXLocalWebSite)
	ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_USER+100, OnFileChange)
END_MSG_MAP()

	LRESULT OnFileChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// ILXLocalWebSite
public:
	STDMETHOD(UpdateItem)(/*[in]*/ BSTR pathName, /*[in]*/ BSTR xmltext);
	STDMETHOD(GetItemAttributes)(/*[in]*/ BSTR pathName, /*[out,retval]*/ BSTR* pVal);
	STDMETHOD(GetFolderList)(/*[in]*/ BSTR pathName, /*[out,retval]*/ BSTR* pVal);
	STDMETHOD(RenameFile)(/*[in]*/ DWORD file_id, /*[in]*/ BSTR name, /*[out,retval]*/ VARIANT_BOOL* pSuccess);
	STDMETHOD(CreateFolder)(/*[in]*/ DWORD parent_id, /*[in]*/ BSTR name, /*[in]*/ BSTR user, /*[out,retval]*/ DWORD* pfolder_id);
	STDMETHOD(DeleteFilePermanently)(/*[in]*/ unsigned long file_id, /*[out,retval]*/ VARIANT_BOOL* pSuccess);
	STDMETHOD(FindOrCreateFolder)(/*[in]*/ DWORD parent_id, /*[in]*/ BSTR name, /*[in]*/ BSTR user, /*[out,retval]*/ DWORD* folder_id);
	STDMETHOD(GetParentFolderId)(/*[in]*/ DWORD folder_id, /*[out,retval]*/ DWORD* pVal);
	STDMETHOD(GetFolderDisplayPath)(/*[in]*/ DWORD folder_id, /*[out,retval]*/ BSTR* displayName);
	STDMETHOD(OpenFileAsArray)(/*[in]*/ unsigned long file_id, /*[out,retval]*/ SAFEARRAY** dataArray);
	STDMETHOD(SaveArrayAsFile)(/*[in]*/ unsigned long folder_id, /*[in]*/ BSTR fileName, /*[in]*/ SAFEARRAY* dataArray, /*[in]*/ BSTR user, /*[in,out]*/ DWORD* file_id, /*[out,retval]*/ VARIANT_BOOL* pSuccess);
	STDMETHOD(get_rootPath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(GetDBConnection)(/*[out,retval]*/ IDispatch** pVal);
	STDMETHOD(NewDocument)(/*[in]*/ BSTR pathName, /*[in]*/ BSTR siteName);
	STDMETHOD(GetHomepageFileId)(/*[out,retval]*/ unsigned long* file_id);
	STDMETHOD(SetHomepageFileId)(/*[in]*/ unsigned long file_id);
	STDMETHOD(LoadDocument)(/*[in]*/ BSTR pathName, /*[out,retval]*/ VARIANT_BOOL* success);
	STDMETHOD(GetFileMoniker)(/*[in]*/ BSTR pathName, /*[out,retval]*/ IMoniker* *pVal);
};

#endif //__LXLOCALWEBSITE_H_
