#include "SubjectObject.h"
#include "Observer.h"

CSubjectObject::CSubjectObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CSubjectObject::CSubjectObject(const CSubjectObject& Prototype)
	: CGameObject { Prototype }
{

}

void CSubjectObject::Add_Observer(CObserver* pObserver)
{
	if (nullptr == pObserver)
		return;

	m_Observers.push_back(pObserver);
}

void CSubjectObject::Notify(CObserver::EVENT eEvent)
{
	for (auto Observer : m_Observers)
		Observer->onNotify(eEvent, this);
}

void CSubjectObject::Free()
{
	__super::Free();

	auto iter = m_Observers.begin();

	for (iter; iter != m_Observers.end();)
	{
		Safe_Release(*iter);
		iter = m_Observers.erase(iter);
	}
}
