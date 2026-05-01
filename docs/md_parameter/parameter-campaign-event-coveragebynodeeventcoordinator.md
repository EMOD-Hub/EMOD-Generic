# CoverageByNodeEventCoordinator

The **CoverageByNodeEventCoordinator** coordinator class distributes individual-level interventions and is
similar to the **StandardInterventionDistributionEventCoordinator**, but adds the ability to specify
different demographic coverages by node. If no coverage has been specified for a particular node ID,
the coverage will be zero. See the following JSON example and table, which shows all available
parameters for this event coordinator.

!!! note

       This can only be used with individual-level interventions, but EMOD will not produce an error
       if you attempt to use it with an node-level intervention.

!!! note

        Parameters are case-sensitive. For Boolean parameters, set to 1 for true or 0 for false.
        Minimum, maximum, or default values of "NA" indicate that those values are not applicable for
        that parameter.

        EMOD does not use true defaults; that is, if the dependency relationships indicate that a parameter is required, you must supply a value for it. However, many of the tools used to work with EMOD will use the default values provided below.

        JSON format does not permit comments, but you can add "dummy" parameters to add contextual
        information to your files. Any keys that are not EMOD parameter names will be ignored by the
        model.

The table below describes all possible parameters with which this class can be configured. The JSON
example that follows shows one potential configuration.

{{ read_csv("csv/campaign-coveragebynodeeventcoordinator.csv") }}

```json
{% include "json/campaign-coveragebynodeeventcoordinator.json" %}
```
