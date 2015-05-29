//
//本文档用于定义通信对象的唯一ID，及事件内容
//

//通信对象唯一ID
#define COMID_MainDlg					1				//主对话框对象
#define COMID_OrderManageDlg			2				//单子管理窗口对象
#define COMID_EatCodeDlg				3				//吃码上报窗口对象
#define COMID_RewardDlg					4				//兑奖窗口对象
#define COMID_SettingDlg				5				//设置窗口对象
#define COMID_CustomerDlg				6				//下庄用户管理窗口对象


//通信事件
#define COMEVENT_ShowOrderPage			1		//显示接单页面，wParam, lParam无意义
#define COMEVENT_CustomerChange			2		//下庄用户变更，wParam为CUSTOMER_CHANGE_*值表示具体变更的地方，lParam为TCHAR指针，表示变更下庄用户名
#define COMEVENT_ShowCustomerPage		4		//显示下庄页面，wParam, lParam无意义
#define COMEVENT_ShowEatCodePage		5		//显示吃码上报页面，wParam, lParam无意义
#define COMEVENT_OrderChange			6		//单子发生变更，wParam为ORDER_CHANGE_MASK_*标志，lParam无意义
#define COMEVENT_ShowSettingPage			7		//显示设置页面，wParam, lParam无意义
#define COMEVENT_DeleteAllOrder				8		//删除所有单子

//单子变化标志
#define ORDER_CHANGE_MASK_ORDER			0X00000001				//单子数据发生变化
#define ORDER_CHANGE_MASK_EAT_TABLE		0X00000002				//吃码表数据发生变化

//用户变化标志
#define CUSTOMER_CHANGE_NEW_CUSTOMER				1		//新增一个用户
#define CUSTOMER_CHANGE_DELETE_CUSTOMER			2		//删除一个用户
#define CUSTOMER_CHANGE_UPDATE_CUSOMTER		3		//更新一个用户