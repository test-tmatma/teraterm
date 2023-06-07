/*
 * (C) 2005- TeraTerm Project
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

/* Routines for dialog boxes */

#include <windows.h>
#if !defined(_CRTDBG_MAP_ALLOC)
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#include <crtdbg.h>
#include <wchar.h>
#include <assert.h>

#include "dlglib.h"
#include "ttlib.h"
#include "codeconv.h"
#include "asprintf.h"
#include "compat_win.h"
#include "win32helper.h"

/**
 *	EndDialog() 互換関数
 */
BOOL TTEndDialog(HWND hDlgWnd, INT_PTR nResult)
{
	return EndDialog(hDlgWnd, nResult);
}

/**
 *	CreateDialogIndirectParam() 互換関数
 */
HWND TTCreateDialogIndirectParam(
	HINSTANCE hInstance,
	LPCTSTR lpTemplateName,
	HWND hWndParent,			// オーナーウィンドウのハンドル
	DLGPROC lpDialogFunc,		// ダイアログボックスプロシージャへのポインタ
	LPARAM lParamInit)			// 初期化値
{
	DLGTEMPLATE *lpTemplate = TTGetDlgTemplate(hInstance, lpTemplateName);
	HWND hDlgWnd = CreateDialogIndirectParamW(hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
	free(lpTemplate);
	return hDlgWnd;
}

/**
 *	CreateDialogParam() 互換関数
 */
HWND TTCreateDialogParam(
	HINSTANCE hInstance,
	LPCTSTR lpTemplateName,
	HWND hWndParent,
	DLGPROC lpDialogFunc,
	LPARAM lParamInit)
{
	return TTCreateDialogIndirectParam(hInstance, lpTemplateName,
									   hWndParent, lpDialogFunc, lParamInit);
}

/**
 *	CreateDialog() 互換関数
 */
HWND TTCreateDialog(
	HINSTANCE hInstance,
	LPCTSTR lpTemplateName,
	HWND hWndParent,
	DLGPROC lpDialogFunc)
{
	return TTCreateDialogParam(hInstance, lpTemplateName,
							   hWndParent, lpDialogFunc, (LPARAM)NULL);
}

/**
 *	DialogBoxParam() 互換関数
 *		EndDialog()ではなく、TTEndDialog()を使用すること
 */
INT_PTR TTDialogBoxParam(HINSTANCE hInstance, LPCTSTR lpTemplateName,
						 HWND hWndParent,		// オーナーウィンドウのハンドル
						 DLGPROC lpDialogFunc,  // ダイアログボックスプロシージャへのポインタ
						 LPARAM lParamInit)		// 初期化値
{
	DLGTEMPLATE *lpTemplate = TTGetDlgTemplate(hInstance, lpTemplateName);
	INT_PTR DlgResult = DialogBoxIndirectParamW(hInstance, lpTemplate, hWndParent, lpDialogFunc, lParamInit);
	free(lpTemplate);
	return DlgResult;
}

/**
 *	DialogBox() 互換関数
 *		EndDialog()ではなく、TTEndDialog()を使用すること
 */
INT_PTR TTDialogBox(HINSTANCE hInstance, LPCTSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc)
{
	return TTDialogBoxParam(hInstance, lpTemplateName, hWndParent, lpDialogFunc, (LPARAM)NULL);
}

/**
 *	ダイアログフォントを設定する
 *		1. 指定フォントを設定する(存在しない場合は2へ)
 *		2. lngファイル内のフォントを設定する(設定がない,存在しない場合は3へ)
 *		3. MessageBox()のフォントを設定する
 *
 * @param FontName			フォント 名前 (NULLのとき指定なし)
 * @param FontPoint			フォント ポイント
 * @param FontCharSet		フォント CharSet(不要?)
 * @param UILanguageFile	lng ファイル (NULLのとき指定なし)
 * @param Section			lng セクション
 * @param Key				lng キー
*/
void SetDialogFont(const wchar_t *FontName, int FontPoint, int FontCharSet,
				   const wchar_t *UILanguageFile, const char *Section, const char *Key)
{
	LOGFONTW logfont;
	BOOL result;

	// 指定フォントをセット
	if (FontName != NULL && FontName[0] != 0) {
		// 存在チェック
		result = IsExistFontW(FontName, FontCharSet, TRUE);
#if 0
		// 存在をチェックしない
		//   存在しなくてもフォントリンクで多分うまく表示される
		//result = TRUE;
#endif
		if (result == TRUE) {
			TTSetDlgFontW(FontName, FontPoint, FontCharSet);
			return;
		}
	}

	// .lngの指定
	if (UILanguageFile != NULL && Section != NULL && Key != NULL) {
		wchar_t *sectionW = ToWcharA(Section);
		wchar_t *keyW = ToWcharA(Key);
		result = GetI18nLogfontW(sectionW, keyW, &logfont, 0, UILanguageFile);
		free(keyW);
		free(sectionW);
		if (result == TRUE) {
			if (IsExistFontW(logfont.lfFaceName, logfont.lfCharSet, TRUE)) {
				TTSetDlgFontW(logfont.lfFaceName, logfont.lfHeight, logfont.lfCharSet);
				return;
			}
		}
	}

	// ini,lngで指定されたフォントが見つからなかったとき、
	// messagebox()のフォントをとりあえず選択しておく
	GetMessageboxFontW(&logfont);
	if (logfont.lfHeight < 0) {
		logfont.lfHeight = GetFontPointFromPixel(NULL, -logfont.lfHeight);
	}
	TTSetDlgFontW(logfont.lfFaceName, logfont.lfHeight, logfont.lfCharSet);
}


/**
 *	pixel数をpoint数に変換する(フォント用)
 *		注 1point = 1/72 inch, フォントの単位
 *		注 ウィンドウの表示具合で倍率が変化するので hWnd が必要
 */
int GetFontPixelFromPoint(HWND hWnd, int pixel)
{
	if (hWnd == NULL) {
		hWnd = GetDesktopWindow();
	}
	HDC DC = GetDC(hWnd);
	int dpi = GetDeviceCaps(DC, LOGPIXELSY);	// dpi = dot per inch (96DPI)
	int point = MulDiv(pixel, dpi, 72);			// pixel = point / 72 * dpi
	ReleaseDC(hWnd, DC);
	return point;
}

/**
 *	point数をpixel数に変換する(フォント用)
 *		注 1point = 1/72 inch, フォントの単位
 */
int GetFontPointFromPixel(HWND hWnd, int point)
{
	HDC DC = GetDC(hWnd);
	int dpi = GetDeviceCaps(DC, LOGPIXELSY);	// dpi = dot per inch (96DPI)
	int pixel = MulDiv(point, 72, dpi);			// point = pixel / dpi * 72
	ReleaseDC(hWnd, DC);
	return pixel;
}

/**
 *	コンボボックス、ドロップダウンリストの
 *	リストの横幅を拡張する(元の幅より狭くなることはない)
 *	セットされている文字がすべて見えるようにする
 *
 *	@param[in]	dlg		ダイアログのハンドル
 *	@param[in]	ID		コンボボックスのID
 *
 *	コンボボックスにテキストをセット(SetDlgItemTextW(dlg, ID, L"text");)
 *	する前にコールするとフォーカスがおかしくなることがない
 */
void ExpandCBWidth(HWND dlg, int ID)
{
	HWND hCtrlWnd = GetDlgItem(dlg, ID);
	int count = (int)SendMessageW(hCtrlWnd, CB_GETCOUNT, 0, 0);
	HFONT hFont = (HFONT)SendMessageW(hCtrlWnd, WM_GETFONT, 0, 0);
	int i, max_width = 0;
	HDC TmpDC = GetDC(hCtrlWnd);
	hFont = (HFONT)SelectObject(TmpDC, hFont);
	for (i = 0; i < count; i++) {
		SIZE s;
		int len = (int)SendMessageW(hCtrlWnd, CB_GETLBTEXTLEN, i, 0);
		wchar_t *text = (wchar_t *)calloc(len + 1, sizeof(wchar_t));
		SendMessageW(hCtrlWnd, CB_GETLBTEXT, i, (LPARAM)text);
		GetTextExtentPoint32W(TmpDC, text, len, &s);
		if (s.cx > max_width)
			max_width = s.cx;
		free(text);
	}
	max_width += GetSystemMetrics(SM_CXVSCROLL);  // スクロールバーの幅も足し込んでおく
	SendMessageW(hCtrlWnd, CB_SETDROPPEDWIDTH, max_width, 0);
	SelectObject(TmpDC, hFont);
	ReleaseDC(hCtrlWnd, TmpDC);
}

/**
 *	GetOpenFileName(), GetSaveFileName() 用フィルタ文字列取得
 *
 *	@param[in]	user_filter_mask	ユーザーフィルタ文字列
 *									"*.txt", "*.txt;*.log" など
 *									NULLのとき使用しない
 *	@param[in]	UILanguageFile
 *	@param[out]	len					生成した文字列長(wchar_t単位)
 *									NULLのときは返さない
 *	@retval		"User define(*.txt)\0*.txt\0All(*.*)\0*.*\0" など
 *				終端は "\0\0" となる
 */
wchar_t *GetCommonDialogFilterWW(const wchar_t *user_filter_mask, const wchar_t *UILanguageFile, size_t *len)
{
	// "ユーザ定義(*.txt)\0*.txt"
	wchar_t *user_filter_str = NULL;
	size_t user_filter_len = 0;
	if (user_filter_mask != NULL && user_filter_mask[0] != 0) {
		wchar_t *user_filter_name;
		GetI18nStrWW("Tera Term", "FILEDLG_USER_FILTER_NAME", L"User define", UILanguageFile, &user_filter_name);
		size_t user_filter_name_len = wcslen(user_filter_name);
		size_t user_filter_mask_len = wcslen(user_filter_mask);
		user_filter_len = user_filter_name_len + 1 + user_filter_mask_len + 1 + 1 + user_filter_mask_len + 1;
		user_filter_str = (wchar_t *)malloc(user_filter_len * sizeof(wchar_t));
		wchar_t *p = user_filter_str;
		wmemcpy(p, user_filter_name, user_filter_name_len);
		p += user_filter_name_len;
		*p++ = '(';
		wmemcpy(p, user_filter_mask, user_filter_mask_len);
		p += user_filter_mask_len;
		*p++ = ')';
		*p++ = '\0';
		wmemcpy(p, user_filter_mask, user_filter_mask_len);
		p += user_filter_mask_len;
		*p++ = '\0';
		free(user_filter_name);
	}

	// "すべてのファイル(*.*)\0*.*"
	wchar_t *all_filter_str;
	size_t all_filter_len;
	GetI18nStrWW("Tera Term", "FILEDLG_ALL_FILTER", L"All(*.*)\\0*.*", UILanguageFile, &all_filter_str);
	{
		// check all_filter_str
		size_t all_filter_title_len = wcslen(all_filter_str);
		if (all_filter_title_len == 0) {
			free(all_filter_str);
			all_filter_str = NULL;
			all_filter_len = 0;
		} else {
			size_t all_filter_mask_len = wcslen(all_filter_str + all_filter_title_len + 1);
			if (all_filter_mask_len == 0) {
				free(all_filter_str);
				all_filter_str = NULL;
				all_filter_len = 0;
			} else {
				// ok
				all_filter_len = all_filter_title_len + 1 + all_filter_mask_len + 1;
			}
		}
	}

	// フィルタ文字列を作る
	size_t filter_len = user_filter_len + all_filter_len;
	wchar_t* filter_str;
	if (filter_len != 0) {
		filter_len++;
		filter_str = (wchar_t*)malloc(filter_len * sizeof(wchar_t));
		wchar_t *p = filter_str;
		if (user_filter_len != 0) {
			wmemcpy(p, user_filter_str, user_filter_len);
			p += user_filter_len;
		}
		wmemcpy(p, all_filter_str, all_filter_len);
		p += all_filter_len;
		*p = '\0';
	} else {
		filter_len = 2;
		filter_str = (wchar_t*)malloc(filter_len * sizeof(wchar_t));
		filter_str[0] = 0;
		filter_str[1] = 0;
	}

	if (user_filter_len != 0) {
		free(user_filter_str);
	}
	if (all_filter_len != 0) {
		free(all_filter_str);
	}

	if (len != NULL) {
		*len = filter_len;
	}

	return filter_str;
}

wchar_t *GetCommonDialogFilterWW(const wchar_t *user_filter_mask, const wchar_t *UILanguageFile)
{
	return GetCommonDialogFilterWW(user_filter_mask, UILanguageFile, NULL);
}

/**
 *	アイコンをロードする
 *	@param[in]	hinst
 *	@param[in]	name
 *	@param[in]	cx	アイコンサイズ(96dpi時)
 *	@param[in]	cy	アイコンサイズ
 *	@param[in]	dpi	アイコンサイズ(cx,cy)はdpi/96倍のサイズで読み込まれる
 *	@param[in]	notify	カスタム通知アイコンの場合は TRUE, ウィンドウアイコンの場合は FALSE
 *	@return		HICON
 *
 *		cx == 0 && cy == 0 のときデフォルトのアイコンサイズで読み込む
 *		DestroyIcon()すること
 */
HICON TTLoadIcon(HINSTANCE hinst, const wchar_t *name, int cx, int cy, UINT dpi, BOOL notify)
{
	if (cx == 0 && cy == 0) {
		// 100%(96dpi?)のとき、GetSystemMetrics(SM_CXICON)=32
		if (pGetSystemMetricsForDpi != NULL) {
			cx = pGetSystemMetricsForDpi(SM_CXICON, dpi);
			cy = pGetSystemMetricsForDpi(SM_CYICON, dpi);
		}
		else {
			cx = GetSystemMetrics(SM_CXICON);
			cy = GetSystemMetrics(SM_CYICON);
		}
	}
	else {
		cx = cx * dpi / 96;
		cy = cy * dpi / 96;
	}
	HICON hIcon;
	if (IsWindowsNT4() || (notify && IsWindows2000())) {
		// 4bit アイコン
		// 		1. NT4 のとき
		//				Windows NT 4.0 は 4bit アイコンしかサポートしていない
		// 		2. Windows 2000 のタスクトレイアイコンのとき
		//				Windows 2000 のタスクトレイは 4bit アイコンしかサポートしていない
		// LR_VGACOLOR = 16(4bit) color = VGA color
		hIcon = (HICON)LoadImageW(hinst, name, IMAGE_ICON, cx, cy, LR_VGACOLOR);
	}
	else {
		HRESULT hr = _LoadIconWithScaleDown(hinst, name, cx, cy, &hIcon);
		if(FAILED(hr)) {
			hIcon = NULL;
		}
	}
	return hIcon;
}

typedef struct {
	wchar_t *icon_name;
	HICON icon;
	WNDPROC prev_proc;
	int cx;
	int cy;
} IconSubclassData;

static void SetIcon(HWND hwnd, HICON icon)
{
	char class_name[32];
	int r = GetClassNameA(hwnd, class_name, _countof(class_name));
	if (r == 0) {
		return;
	}
	if (strcmp(class_name, "Button") == 0) {
		SendMessage(hwnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon);
	}
	else if (strcmp(class_name, "Static") == 0) {
		SendMessage(hwnd, STM_SETICON, (WPARAM)icon, 0);
	}
	else {
		// not support
		assert(FALSE);
	}
}

static LRESULT IconProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	IconSubclassData *data = (IconSubclassData *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (msg) {
	case WM_DPICHANGED: {
		const HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
		const UINT new_dpi = LOWORD(wp);
		HICON icon = TTLoadIcon(hinst, data->icon_name, data->cx, data->cy, new_dpi, FALSE);
		if (icon != NULL) {
			DestroyIcon(data->icon);
			data->icon = icon;
			SetIcon(hwnd, icon);
		}
		break;
	}
	case WM_NCDESTROY: {
		LRESULT result = CallWindowProc(data->prev_proc, hwnd, msg, wp, lp);
		DestroyIcon(data->icon);
		if (HIWORD(data->icon_name) != 0) {
			free(data->icon_name);
		}
		free(data);
		return result;
	}
	default:
		break;
	}
	return CallWindowProc(data->prev_proc, hwnd, msg, wp, lp);
}

/**
 *	ダイアログのコントロールにアイコンをセットする
 *
 *	@param	dlg		ダイアログ
 *	@param	nID		コントロールID
 *	@param	name	アイコン
 *	@param	cx		アイコンサイズ
 *	@param	cy		アイコンサイズ
 *
 *	cx == 0 && cy == 0 のときデフォルトのアイコンサイズで読み込む
 *
 *	セットする例
 *		SetDlgItemIcon(Dialog, IDC_TT_ICON, MAKEINTRESOURCEW(IDI_TTERM), 0, 0);
 *	DPIが変化したときにアイコンのサイズを変更する例
 *		case WM_DPICHANGED:
 *			SendDlgItemMessage(Dialog, IDC_TT_ICON, WM_DPICHANGED, wParam, lParam);
 */
void SetDlgItemIcon(HWND dlg, int nID, const wchar_t *name, int cx, int cy)
{
	IconSubclassData *data = (IconSubclassData *)malloc(sizeof(IconSubclassData));
	data->icon_name = (HIWORD(name) == 0) ? (wchar_t *)name : _wcsdup(name);
	data->cx = cx;
	data->cy = cy;

	const HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(dlg, GWLP_HINSTANCE);
	const UINT dpi = GetMonitorDpiFromWindow(dlg);
	data->icon = TTLoadIcon(hinst, name, cx, cy, dpi, FALSE);

	const HWND hwnd = GetDlgItem(dlg, nID);
	SetIcon(hwnd, data->icon);

	data->prev_proc = (WNDPROC)SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)IconProc);
	SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

/**
 *	接続したホスト履歴をコンボボックスにセットする
 */
void SetComboBoxHostHistory(HWND dlg, int dlg_item, int maxhostlist, const wchar_t *SetupFNW)
{
	int i = 1;
	do {
		wchar_t EntNameW[128];
		wchar_t *TempHostW;
		_snwprintf_s(EntNameW, _countof(EntNameW), _TRUNCATE, L"host%d", i);
		hGetPrivateProfileStringW(L"Hosts", EntNameW, L"", SetupFNW, &TempHostW);
		if (TempHostW[0] != 0) {
			SendDlgItemMessageW(dlg, dlg_item, CB_ADDSTRING,
								0, (LPARAM) TempHostW);
		}
		free(TempHostW);
		i++;
	} while (i <= maxhostlist);
}

/**
 *	ウィンドウにアイコンをセットする
 *
 *	@param	hInst		アイコンを保持しているモジュールのinstance
 *						icon_name == NULL のとき NULL でもよい
 *	@param	hWnd		アイコンを設定するWindow Handle
 *	@param	icon_name	アイコン名
 *						NULLのとき アイコンを削除する
 *						idからの変換はMAKEINTRESOURCEW()を使う
 *	@param	dpi			dpi
 *						0 のとき hWnd が表示されているモニタのDPI
 */
void TTSetIcon(HINSTANCE hInst, HWND hWnd, const wchar_t *icon_name, UINT dpi)
{
	HICON icon;
	if (icon_name != NULL) {
		if (dpi == 0) {
			// hWnd が表示されているモニタのDPI
			dpi = GetMonitorDpiFromWindow(hWnd);
		}

		// 大きいアイコン(32x32,ディスプレイの拡大率が100%(dpi=96)のとき)
		icon = TTLoadIcon(hInst, icon_name, 0, 0, dpi, FALSE);
		icon = (HICON)::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
		if (icon != NULL) {
			DestroyIcon(icon);
		}

		// 小さいアイコン(16x16,ディスプレイの拡大率が100%(dpi=96)のとき)
		icon = TTLoadIcon(hInst, icon_name, 16, 16, dpi, FALSE);
		icon = (HICON)::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		if (icon != NULL) {
			DestroyIcon(icon);
		}
	}
	else {
		// アイコンを削除
		HICON null_icon = NULL;
		icon = (HICON)::SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)null_icon);
		if (icon != NULL) {
			DestroyIcon(icon);
		}
		icon = (HICON)::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)null_icon);
		if (icon != NULL) {
			DestroyIcon(icon);
		}
	}
}
