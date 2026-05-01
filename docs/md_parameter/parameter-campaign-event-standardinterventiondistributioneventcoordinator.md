# StandardInterventionDistributionEventCoordinator

The **StandardInterventionDistributionEventCoordinator** coordinator class distributes an individual-level or
node-level intervention to a specified fraction of individuals or nodes within a node set. Recurring
campaigns can be created by specifying the number of times distributions should occur and the time
between repetitions.

Demographic restrictions such as **Demographic_Coverage** and **Target_Gender** do not apply when
distributing node-level interventions. The node-level intervention must handle the demographic
restrictions.

See the following JSON example and table, which shows all available parameters
for this event coordinator.

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

{{ read_csv("csv/campaign-standardinterventiondistributioneventcoordinator.csv") }}

```json
{% include "json/campaign-standardinterventiondistributioneventcoordinator.json" %}
```
