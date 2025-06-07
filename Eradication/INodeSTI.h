
#pragma once
#include <vector>
#include "ISupports.h"
#include "IRelationshipManager.h"
#include "ISociety.h"
#include "TransmissionGroupMembership.h"

using namespace std;

namespace Kernel
{
    class INodeSTI : public ISupports
    {
    public:
        virtual /*const?*/ IRelationshipManager* GetRelationshipManager() /*const?*/ = 0;
        virtual void GetGroupMembershipForIndividual_STI( const std::map<std::string, uint32_t>& properties, std::map< int, TransmissionGroupMembership_t>& membershipOut ) = 0;
        virtual ISociety* GetSociety() = 0;
        virtual std::string GetRelationshipName(int) = 0;
        virtual std::string GetRelationshipStateName(int) = 0;
    };
}
