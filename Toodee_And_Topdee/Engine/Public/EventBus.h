#pragma once
#include "Base.h"
#include "Event.h"

BEGIN(Engine)

class CEventBus final : public CBase
{
public:
    typedef function<void(const CEvent&)> Callback;

private:
    CEventBus();
    virtual ~CEventBus() = default;

public:
    HRESULT Initialize(_uint iNumLevels);

public:
    template <typename EventType>
    void Subscribe(_uint iSubscribeLevel,EVENT_KEY eEventKey, function<void(const EventType&)> Callback) {
        auto Wrapper = [Callback](const CEvent& Event) {
            Callback(static_cast<const EventType&>(Event));
            };
        m_Subscribers[iSubscribeLevel][eEventKey].push_back(Wrapper);
    }

    void Publish(_uint iSubscribeLevel, EVENT_KEY eEventKey, const CEvent& Event) {
        for (auto CallBack : m_Subscribers[iSubscribeLevel][eEventKey]) {
            CallBack(Event);
        }
    }

    void Clear(_uint iClearLevel);

private:
    _uint									    m_iNumLevels = {};
    unordered_map<EVENT_KEY, vector<Callback>>* m_Subscribers = { nullptr };

public:
    static CEventBus* Create(_uint iNumLevels);
    virtual void Free() override;
};

END