
#pragma once

#include "BaseChannelReport.h"

class CustomReport : public BaseChannelReport
{
public:
    CustomReport();
    virtual ~CustomReport() { }

    virtual void LogNodeData( Kernel::INodeContext * pNC ) override;
    virtual bool IsCollectingIndividualData( float currentTime, float dt ) const override { return true ; } ;
    virtual void LogIndividualData( Kernel::IIndividualHuman* individual ) override;
    virtual void Finalize() override;

protected:
    virtual void populateSummaryDataUnitsMap( std::map<std::string, std::string> &units_map ) override;
    virtual void postProcessAccumulatedData() override;

private:
};

