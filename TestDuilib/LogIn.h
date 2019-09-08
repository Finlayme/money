#include "Common.h"


class LogInWnd : public WindowImplBase
{
public:
	LogInWnd(MySQL* pMySQL);

	virtual void Notify(TNotifyUI& msg);

	void LogIn();
protected:
	// WindowImplBase：提供的三个纯虚函数
	// xml文件对应的目录
	virtual CDuiString GetSkinFolder();

	// xml文件的名字
	virtual CDuiString GetSkinFile();

	// 窗口类的名字：在注册窗口时必须提供
	virtual LPCTSTR GetWindowClassName(void) const;

	MySQL* m_pMySQL;
};