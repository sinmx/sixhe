//
//���ĵ����ڶ���ͨ�Ŷ����ΨһID�����¼�����
//

//ͨ�Ŷ���ΨһID
#define COMID_MainDlg					1				//���Ի������
#define COMID_OrderManageDlg			2				//���ӹ����ڶ���
#define COMID_EatCodeDlg				3				//�����ϱ����ڶ���
#define COMID_RewardDlg					4				//�ҽ����ڶ���
#define COMID_SettingDlg				5				//���ô��ڶ���
#define COMID_CustomerDlg				6				//��ׯ�û������ڶ���


//ͨ���¼�
#define COMEVENT_ShowOrderPage			1		//��ʾ�ӵ�ҳ�棬wParam, lParam������
#define COMEVENT_CustomerChange			2		//��ׯ�û������wParamΪCUSTOMER_CHANGE_*ֵ��ʾ�������ĵط���lParamΪTCHARָ�룬��ʾ�����ׯ�û���
#define COMEVENT_ShowCustomerPage		4		//��ʾ��ׯҳ�棬wParam, lParam������
#define COMEVENT_ShowEatCodePage		5		//��ʾ�����ϱ�ҳ�棬wParam, lParam������
#define COMEVENT_OrderChange			6		//���ӷ��������wParamΪORDER_CHANGE_MASK_*��־��lParam������
#define COMEVENT_ShowSettingPage			7		//��ʾ����ҳ�棬wParam, lParam������
#define COMEVENT_DeleteAllOrder				8		//ɾ�����е���

//���ӱ仯��־
#define ORDER_CHANGE_MASK_ORDER			0X00000001				//�������ݷ����仯
#define ORDER_CHANGE_MASK_EAT_TABLE		0X00000002				//��������ݷ����仯

//�û��仯��־
#define CUSTOMER_CHANGE_NEW_CUSTOMER				1		//����һ���û�
#define CUSTOMER_CHANGE_DELETE_CUSTOMER			2		//ɾ��һ���û�
#define CUSTOMER_CHANGE_UPDATE_CUSOMTER		3		//����һ���û�