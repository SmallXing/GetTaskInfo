//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 版权所有：上海新浩艺软件有限公司
// 文 件 名：logInit.cpp 
// 创建日期：2015-07-10
// 作    者：李建星
// 修改日期：
// 修 改 者：
// 修改说明：
// 附加说明：使用boost.smart_ptr来完成CLog4zInit的反初始化
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "logInit.h"

std::shared_ptr<CLog4zInit> CLog4zInit::mSPtr;

void CLog4zInit::Initialize()
{
	if (!mSPtr)
	{
		mSPtr.reset(new CLog4zInit);
	}
}