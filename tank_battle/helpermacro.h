#ifndef _HELPERMACRO_H
#define _HELPERMACRO_H

#include <cstdio>
#include <functional>
#include "gameplay.h"
#include "entitymanager.h"

//FUNC_CALLBACK_X表示有X个参数
#define FUNC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define FUNC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define FUNC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define FUNC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

#define SAFE_DELETE(p)\
{\
	delete (p);\
	(p) = nullptr;\
}

#define SAFE_DELETE_ARRAY(p)\
{\
	delete [](p);\
	(p) = nullptr;\
}

#define SAFE_CLOSE(fp)\
{\
	if(fp)\
	{\
		fclose(fp);\
		fp = nullptr;\
	}\
}

#define BREAKIF(c)\
{\
	if(c)\
	{\
		break;\
	}\
}

#define FLOATEQUALACC 1e-5
#define FLOATEQUAL(a, b) (fabs((a) - (b)) < FLOATEQUALACC)

#define KEYSTATE(k) (GamePlay::GetInstance()->GetKeyState(k))
#define KEYUP(k) (GamePlay::GetInstance()->IsKeyUp(k))
#define KEYDOWN(k) (GamePlay::GetInstance()->IsKeyDown(k))

#define SAFE_DESTROYENT(e)\
{\
	EntityManager::GetInstance()->DestroyEntity(e);\
	e = nullptr;\
}

#endif