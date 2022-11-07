// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа PLUGINM2TWEOP_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции PLUGINM2TWEOP_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef PLUGINM2TWEOP_EXPORTS
#define PLUGINM2TWEOP_API __declspec(dllexport)
#else
#define PLUGINM2TWEOP_API __declspec(dllimport)
#endif

#include "plugData.h"
#include <string>
#include "framework.h"
#include "basicFuncs.h"



//init this plugin
extern "C" PLUGINM2TWEOP_API int initPlugin(std::string* modPath);

void initLua();