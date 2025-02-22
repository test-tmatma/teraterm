﻿/*
 * Copyright (C) 2018- TeraTerm Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Tera term Micro Framework class
 */
#pragma once
#include <windows.h>

class TTCWnd
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	HACCEL m_hAccel;
	HWND m_hParentWnd;

	TTCWnd();
	void DestroyWindow();
	HWND GetSafeHwnd() const {return m_hWnd;}
	HWND GetSafeHwnd() { return m_hWnd; }
	HDC BeginPaint(LPPAINTSTRUCT lpPaint);
	BOOL EndPaint(LPPAINTSTRUCT lpPaint);
	LRESULT SendMessage(UINT msg, WPARAM wp, LPARAM lp);
	void ShowWindow(int nCmdShow);
	void SetWindowTextW(const wchar_t *str);
	void SetWindowTextA(const char *str);
	LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong);
	LONG_PTR GetWindowLongPtr(int nIndex);
	void ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	void ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	int MessageBoxA(const char * lpText, const char *lpCaption, UINT uType);
	int MessageBoxW(const wchar_t * lpText, const wchar_t *lpCaption, UINT uType);
	//virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL GetWindowRect(RECT *R);
	BOOL SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	BOOL GetClientRect(RECT *R);
	BOOL InvalidateRect(RECT *R, BOOL bErase = TRUE);
	BOOL EndDialog(int nResult);
	// for controls
	HWND GetDlgItem(int id);
	LRESULT SendDlgItemMessageW(int id, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT SendDlgItemMessageA(int id, UINT msg, WPARAM wp, LPARAM lp);
	void GetDlgItemTextW(int id, wchar_t *buf, size_t size);
	void GetDlgItemTextA(int id, char *buf, size_t size);
	void SetDlgItemTextW(int id, const wchar_t *str);
	void SetDlgItemTextA(int id, const char *str);
	void SetDlgItemNum(int id, LONG Num);
	void SetCheck(int id, int nCheck);
	UINT GetCheck(int id);
	void SetCurSel(int id, int no);
	int GetCurSel(int id);
	void SetDlgItemInt(int id, UINT val, BOOL bSigned = TRUE);
	UINT GetDlgItemInt(int id, BOOL* lpTrans = NULL, BOOL bSigned = TRUE) const;
	void EnableDlgItem(int id, BOOL enable);
private:
	void ModifyStyleCom(int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
};

class TTCFrameWnd : public TTCWnd
{
public:
	TTCFrameWnd();
	virtual ~TTCFrameWnd();
	static TTCFrameWnd *pseudoPtr;
	static LRESULT CALLBACK ProcStub(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	BOOL CreateW(HINSTANCE hInstance,
				 LPCWSTR lpszClassName,
				 LPCWSTR lpszWindowName,
				 DWORD dwStyle = WS_OVERLAPPEDWINDOW,
				 const RECT& rect = rectDefault,
				 HWND pParentWnd = NULL,
				 LPCTSTR lpszMenuName = NULL,
				 DWORD dwExStyle = 0);
	virtual LRESULT Proc(UINT msg, WPARAM wp, LPARAM lp) = 0;
	static const RECT rectDefault;
	///
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	///
	void OnKillFocus(HWND hNewWnd);
	void OnSetFocus(HWND hOldWnd);
	void OnSysCommand(WPARAM nID, LPARAM lParam);
	void OnClose();
	void OnSysKeyDown(WPARAM nChar, UINT nRepCnt, UINT nFlags);
	void OnSysKeyUp(WPARAM nChar, UINT nRepCnt, UINT nFlags);
};

class TTCDialog : public TTCWnd
{
public:
	TTCDialog();
	virtual ~TTCDialog();
	BOOL Create(HINSTANCE hInstance, HWND hParent, int idd);
	INT_PTR DoModal(HINSTANCE hInstance, HWND hParent, int idd);
	void DestroyWindow();
	virtual BOOL OnInitDialog();
	virtual	BOOL OnOK();
	virtual BOOL OnCancel();
	virtual BOOL OnCommand(WPARAM wp, LPARAM lp);
	virtual BOOL OnClose();
	virtual BOOL PostNcDestroy();
	virtual LRESULT DlgProc(UINT msg, WPARAM wp, LPARAM lp);

	static INT_PTR CALLBACK DlgProcStub(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK WndProcStub(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
private:

	LRESULT DlgProcBase(UINT msg, WPARAM wp, LPARAM lp);
	LRESULT WndProcBase(UINT msg, WPARAM wp, LPARAM lp);
	static TTCDialog *pseudoPtr;
};

class TTCPropertyPage : public TTCWnd
{
public:
	TTCPropertyPage(HINSTANCE inst, int id);
	virtual ~TTCPropertyPage();
	virtual void OnInitDialog();
	virtual void OnOK();
	virtual void OnHScroll(UINT nSBCode, UINT nPos, HWND pScrollBar);
	virtual BOOL OnCommand(WPARAM wp, LPARAM lp);
	virtual HBRUSH OnCtlColor(HDC hDC, HWND hWnd);
	virtual void OnHelp();
	HPROPSHEETPAGE CreatePropertySheetPage();
protected:
	PROPSHEETPAGEW_V1 m_psp;
private:
	static INT_PTR CALLBACK Proc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp);
};
