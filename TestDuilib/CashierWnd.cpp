#include "CashierWnd.h"


CCashierWnd::CCashierWnd(MySQL* pMySQL)
: m_pMySQL(pMySQL)
{
}


CCashierWnd::~CCashierWnd()
{
}


// xml文件对应的目录
CDuiString CCashierWnd::GetSkinFolder()
{
	return _T("");
}

// xml文件的名字
CDuiString CCashierWnd::GetSkinFile()
{
	return _T("CashierManage.xml");
}

// 窗口类的名字：在注册窗口时必须提供
LPCTSTR CCashierWnd::GetWindowClassName(void) const
{
	return _T("CashierWnd");
}

void CCashierWnd::Notify(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName(); 
	if (msg.sType == _T("click"))
	{
		if (strName == _T("BTN_CLOSE"))
			Close();
		else if (strName == _T("BTN_MIN"))
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if (strName == _T("BTN_ADD"))
			AddGoodsCount();
		else if (strName == _T("BTN_SUB"))
			SubGoodsCount();
		else if (strName == _T("BTN_SELECT"))
			SelectGoods();
		else if (strName == _T("BTN_COMMIT"))
			CommitOrder();
		else if (strName == _T("BTN_CANCEL"))
			CancelOrder();
		else if (strName == _T("BTN_OK"))
			InsertGoodsList();
	}
}

void CCashierWnd::SelectGoods()
{
	// 1. 获取商品名称
	CDuiString strGoodsName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_NAME")))->GetText();

	// 2. 构造SQL语句，到数据库中查此商品
	string strSQL("select * from goodstable where GoodsName = '");
	strSQL += UnicodeToANSI(strGoodsName);
	strSQL += "';";
	vector<vector<string>> vRet = m_pMySQL->Select(strSQL);
	if (vRet.empty())
	{
		MessageBox(m_hWnd, _T("暂无此商品"), _T("Cashier"), IDOK);
		return;
	}

	// 3. 将商品剩余个数显示到界面编辑框中
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_LEFT")))->SetText(ANSIToUnicode(vRet[0][7]));
}

void CCashierWnd::AddGoodsCount()
{
	// 库存减1
	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_LEFT"));
	CDuiString strLeft = pGoodsLeft->GetText();
	if (strLeft == _T("0"))
	{
		MessageBox(m_hWnd, _T("库存量不足"), _T("Cashier"), IDOK);
		return;
	}

	int count = 0;
	count = atoi(UnicodeToANSI(strLeft).c_str());
	--count;
	strLeft.Format(_T("%d"), count);
	pGoodsLeft->SetText(strLeft);

	// 商品个数加1
	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_COUNT"));
	CDuiString strCount = pGoodsCount->GetText();
	count = atoi(UnicodeToANSI(strCount).c_str());
	++count;
	strCount.Format(_T("%d"), count);
	pGoodsCount->SetText(strCount);
}

void CCashierWnd::SubGoodsCount()
{
	// 商品个数减去1
	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_COUNT"));
	CDuiString strCount = pGoodsCount->GetText();
	if (strCount == _T("0"))
	{
		MessageBox(m_hWnd, _T("商品个数已经为0!!!"), _T("Cashier"), IDOK);
		return;
	}

	int count = atoi(UnicodeToANSI(strCount).c_str());
	--count;
	strCount.Format(_T("%d"), count);
	pGoodsCount->SetText(strCount);

	// 库存减1
	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_LEFT"));
	CDuiString strLeft = pGoodsLeft->GetText();	
	count = atoi(UnicodeToANSI(strLeft).c_str());
	++count;
	strLeft.Format(_T("%d"), count);
	pGoodsLeft->SetText(strLeft);
}

void CCashierWnd::InsertGoodsList()
{
	// 1. 从界面获取商品名称以及购买的数量
	CDuiString strGoodsName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_NAME")))->GetText();
	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_COUNT"));
	CDuiString strCount = pGoodsCount->GetText();
	
	// 2. 从数据库中获取商品的价格以及计量单位
	string strSQL("select * from goodstable where GoodsName = '");
	strSQL += UnicodeToANSI(strGoodsName);
	strSQL += "';";
	vector<vector<string>> vRet = m_pMySQL->Select(strSQL);

	// 3. 合计价格
	int count = atoi(UnicodeToANSI(strCount).c_str());
	double price = atof(vRet[0][5].c_str());
	price = count*price;
	CDuiString strPrice;
	strPrice.Format(_T("%lf"), price);

	// 4. 将信息更新到list中
	CListTextElementUI* pItem = new CListTextElementUI;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("OrderList"));
	pList->Add(pItem);
	pItem->SetText(0, strGoodsName);
	pItem->SetText(1, ANSIToUnicode(vRet[0][5]));
	pItem->SetText(2, strCount);
	pItem->SetText(3, ANSIToUnicode(vRet[0][6]));
	pItem->SetText(4, strPrice);

	// 5. 将商品数量以及名称的编辑框清0
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_NAME")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_LEFT")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODS_COUNT")))->SetText(_T("0"));
}

void CCashierWnd::CancelOrder()
{
	// 清空所有商品
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("OrderList"));
	pList->RemoveAll();
}

void CCashierWnd::CommitOrder()
{
	// 1. 合计总价格
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("OrderList"));
	int count = pList->GetCount();

	double totalPrice = 0;
	for (int i = 0; i < count; ++i)
	{
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
		CDuiString strPrice = pItem->GetText(4);
		totalPrice += atof(UnicodeToANSI(strPrice).c_str());
	}

	CDuiString strTotalPrice;
	strTotalPrice.Format(_T("%.02lf"), totalPrice);
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_TOTAL")))->SetText(strTotalPrice);
	// 2. 更新商品的数据库
	for (int i = 0; i < count; ++i)
	{
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
		CDuiString strCount = pItem->GetText(2);

		string strSQL("update goodstable set Inventory='");
		strSQL += UnicodeToANSI(strCount);
		strSQL += "';";
		m_pMySQL->UpDate(strSQL);
	}
	// 3. 插入本次销售记录
}