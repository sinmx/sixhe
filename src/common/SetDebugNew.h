//
//�����ڴ�й¶��⣬����ͷ�ļ����뵽stdafx.h�ļ���
//

#pragma once

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new(__FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif