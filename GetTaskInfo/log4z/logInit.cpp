//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ��Ȩ���У��Ϻ��º���������޹�˾
// �� �� ����logInit.cpp 
// �������ڣ�2015-07-10
// ��    �ߣ����
// �޸����ڣ�
// �� �� �ߣ�
// �޸�˵����
// ����˵����ʹ��boost.smart_ptr�����CLog4zInit�ķ���ʼ��
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