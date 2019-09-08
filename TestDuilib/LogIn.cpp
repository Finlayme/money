#include "LogIn.h"
#include "MainWnd.h"
#include "CashierWnd.h"


LogInWnd::LogInWnd(MySQL* pMySQL)
: m_pMySQL(pMySQL)
{}

// xml文件对应的目录
CDuiString LogInWnd::GetSkinFolder()
{
	return _T("");
}

// xml文件的名字
CDuiString LogInWnd::GetSkinFile()
{
	return _T("loginWnd.xml");
}

// 窗口类的名字：在注册窗口时必须提供
LPCTSTR LogInWnd::GetWindowClassName(void) const
{
	return _T("LogInWnd");
}

void LogInWnd::Notify(TNotifyUI& msg)
{
	CDuiString strName =  msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{
		if (strName == _T("BTN_MIN"))
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			//SendMessage(WM_SYSCOMMAND,SW_MINIMIZE);
		else if (strName == _T("BTN_CLOSE"))
			Close();
		else if (strName == _T("BTN_LOGIN"))
			LogIn();
	}
}

void LogInWnd::LogIn()
{
	// 从编辑框中获取用户名以及密码
	CEditUI* pEditUserName = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_USER_NAME"));
	CDuiString strUserName = pEditUserName->GetText();
	
	CEditUI* pEditPassword = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_USER_PASSWORD"));
	CDuiString strUserPassWord = pEditPassword->GetText();

	if (strUserName.IsEmpty())
	{
		MessageBox(m_hWnd, _T("请输入用户名"), _T("Cashier"), IDOK);
		return;
	}

	if (strUserPassWord.IsEmpty())
	{
		MessageBox(m_hWnd, _T("请输入用户密码"), _T("Cashier"), IDOK);
		return;
	}

	// 查询数据库，检测该用户是否存在
	string strSQL("select * from  employee where Name = '");
	// ascII   UNICODE
	strSQL += UnicodeToANSI(strUserName);
	strSQL += "';";

	vector<vector<string>> vRet = m_pMySQL->Select(strSQL);
	if (vRet.empty())
	{
		MessageBox(m_hWnd, _T("用户名错误"), _T("Caisher"), IDOK);
		return;
	}

	string userpassward = UnicodeToANSI(strUserPassWord);
	if (userpassward != vRet[0][4])
	{
		MessageBox(m_hWnd, _T("用户密码错误"), _T("Caisher"), IDOK);
		return;
	}

	// 隐藏登录窗口
	ShowWindow(false);

	if (vRet[0][5] == "管理员")
	{
		// 创建主窗口
		MainWnd mianWnd(m_pMySQL);
		mianWnd.Create(NULL, _T("MainWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		mianWnd.CenterWindow();
		mianWnd.ShowModal();
	}
	else
	{
		// 创建主窗口
		CCashierWnd mianWnd(m_pMySQL);
		mianWnd.Create(NULL, _T("CashierWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		mianWnd.CenterWindow();
		mianWnd.ShowModal();
	}
}