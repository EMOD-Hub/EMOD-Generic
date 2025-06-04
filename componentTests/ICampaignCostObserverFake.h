
#pragma once

#include "Interventions.h"

using namespace Kernel;

struct ICampaignCostObserverFake : public ICampaignCostObserver
{
    virtual ~ICampaignCostObserverFake() {};

    virtual void notifyCampaignExpenseIncurred( float expenseIncurred, const IIndividualHumanEventContext * pIndiv ) {};

    virtual int32_t AddRef()  { return -1 ; };
    virtual int32_t Release() { return -1 ; };
};
