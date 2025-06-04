
#pragma once
#include "IdmApi.h"
#include <map>
#include <list>
#include <string>
#include <functional>

#include "IIndividualHuman.h"

using namespace std;

namespace Kernel
{
    struct ITransmissionGroups;
    struct IRelationship;
    typedef map< unsigned int, IRelationship* > tNodeRelationshipType;

    struct IDMAPI IRelationshipManager : ISerializable
    {
        virtual void Update( list<IIndividualHuman*>& humans, ITransmissionGroups* groups, float dt ) = 0;
        virtual IRelationship* GetRelationshipById( unsigned int id ) = 0;
        virtual const tNodeRelationshipType& GetNodeRelationships() const = 0;
        virtual INodeContext* GetNode() const = 0;
        virtual void AddRelationship( IRelationship*, bool isNewRelationship ) = 0;
        virtual void RemoveRelationship( IRelationship*, bool leavingNode ) = 0;
        virtual void ConsummateRelationship( IRelationship*, unsigned int acts ) = 0;

        virtual IRelationship* Emigrate( IRelationship* ) = 0;
        virtual IRelationship* Immigrate( IRelationship* ) = 0;

        typedef std::function<void(IRelationship*)> callback_t;
        virtual void RegisterNewRelationshipObserver(callback_t observer) = 0;
        virtual void RegisterRelationshipTerminationObserver(callback_t observer) = 0;
        virtual void RegisterRelationshipConsummationObserver(callback_t observer) = 0;
    };
}
