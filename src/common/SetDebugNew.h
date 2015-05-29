//
//开启内存泄露检测，将该头文件加入到stdafx.h文件中
//

#pragma once

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new(__FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#endif