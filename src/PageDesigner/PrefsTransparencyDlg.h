// PrefsTransparencyDlg.h : Declaration of the CPrefsTransparencyDlg

#ifndef __PREFSTRANSPARENCYDLG_H_
#define __PREFSTRANSPARENCYDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CPrefsTransparencyDlg
class CPrefsTransparencyDlg : 
	public CAxPropertyPageImpl<CPrefsTransparencyDlg>
{
public:
	CPrefsTransparencyDlg()
	{
		m_gridSize = 0;
		m_gridColor0 = 0;
		m_gridColor1 = 0;
	}

	~CPrefsTransparencyDlg()
	{
	}

	int		m_gridSize;
	COLORREF	m_gridColor0;
	COLORREF	m_gridColor1;

	CComPtr<IPDDocumentSettings> m_targetObject;

	void SetColorIndex();

	void FillPreviewBitmap();

	enum { IDD = IDD_PREFSTRANSPARENCYDLG };

BEGIN_MSG_MAP(CPrefsTransparencyDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
	COMMAND_HANDLER(IDC_PREFSTRANSP_GRIDSIZE, CBN_SELCHANGE, OnChangeGridSize)
	COMMAND_HANDLER(IDC_PREFSTRANSP_GRIDCOLORS, CBN_SELCHANGE, OnChangeGridColors)
//	COMMAND_ID_HANDLER(IDOK, OnOK)
//	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	CHAIN_MSG_MAP(CAxPropertyPageImpl<CPrefsTransparencyDlg>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangeGridColors(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeGridSize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	BOOL OnApply();


/*	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	*/
};

#endif //__PREFSTRANSPARENCYDLG_H_
