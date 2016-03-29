//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 版权所有：上海新浩艺软件有限公司
// 文 件 名：logInit.h 
// 创建日期：2015-07-10
// 作    者：李建星
// 修改日期：
// 修 改 者：
// 修改说明：
// 附加说明：使用boost.smart_ptr来完成CLog4zInit的反初始化
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef LOGINIT_HPP_
#define LOGINIT_HPP_
#include "log4z.h"
#include <memory>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 创建日期：2015-07-10
// 作    者：李建星
// 修改日期：
// 修 改 者：
// 修改说明：
// 类 摘 要：日志库的配置初始化，运行封装
// 详细说明：单件 防止多次初始化
// 附加说明：只是日志库的简单使用封装
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CLog4zInit//:public boost::noncopyable
{
public:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 创建日期：2015-07-10
	// 作    者：李建星
	// 修改日期：
	// 修 改 者：
	// 修改说明：
	// 函 数 名：Initialize
	// 功能描述：运用boost.smart_ptr以及单件 进行初始化限定
	// 输入参数：null
	// 输出参数：null
	// 返 回 值：null
	// 异常说明: null
	// 附加说明：不用再写反初始化Release,以及delete 调用
	static void Initialize();
	static std::shared_ptr<CLog4zInit> mSPtr;

	~CLog4zInit()
	{

	}

private:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 创建日期：2015-07-10
	// 作    者：李建星
	// 修改日期：
	// 修 改 者：
	// 修改说明：
	// 函 数 名：CLog4zInit
	// 功能描述：构造函数完成日志格式的初始化配置
	// 输入参数：null
	// 输出参数：null
	// 返 回 值：null
	// 异常说明: null
	// 附加说明：
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CLog4zInit()
	{
		zsummer::log4z::ILog4zManager::getInstance()->config("log.cfg");// 配置文件 默认在当前目录下
		g_idDynamic = zsummer::log4z::ILog4zManager::getInstance()->createLogger("main");
		zsummer::log4z::ILog4zManager::getInstance()->setLoggerPath(g_idDynamic,".");

		zsummer::log4z::ILog4zManager::getInstance()->start();//start log4z
	}

private:
	LoggerId g_idDynamic;

};



#endif