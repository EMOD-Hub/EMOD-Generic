
#pragma once

#include "NodeVectorEventContext.h"
#include "VectorDefs.h"

namespace Kernel
{
    class INodeMalariaInterventionEffects : public ISupports
    {
        public:
            virtual void ChallengeWithSporozoites(int n_sporozoites, float coverage=1.0f, tAgeBitingFunction=nullptr ) = 0;
            virtual void ChallengeWithInfectiousBites(int n_bites, float coverage=1.0f, tAgeBitingFunction=nullptr) = 0;
    };

    class NodeMalariaEventContextHost : public NodeVectorEventContextHost,
                                        public INodeMalariaInterventionEffects
    {
        IMPLEMENT_NO_REFERENCE_COUNTING()

    public:
        NodeMalariaEventContextHost(Node* _node);
        virtual ~NodeMalariaEventContextHost();

        virtual INodeMalariaInterventionEffects* GetNodeMalariaInterventionEffects() override;

        virtual void ChallengeWithSporozoites( int n_sporozoites, float coverage, tAgeBitingFunction );
        virtual void ChallengeWithInfectiousBites( int n_bites, float coverage, tAgeBitingFunction );
    };
}