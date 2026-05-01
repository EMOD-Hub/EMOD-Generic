# NodeLevelHealthTriggeredIV

The **NodeLevelHealthTriggeredIV** intervention class distributes an intervention to an individual when a
specific event occurs. **NodeLevelHealthTriggeredIV** monitors for event triggers from individuals,
and when found, will distribute the intervention. For example, **NodeLevelHealthTriggeredIV** can be
configured such that all individuals will be given a diagnostic intervention when they transition
from susceptible to infectious. During the simulation, when individuals become infected, they
broadcast the **NewInfectionEvent** trigger and **NodeLevelHealthTriggeredIV** distributes the
diagnostic intervention to them.

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

{{ read_csv("csv/campaign-nodelevelhealthtriggerediv.csv") }}

```json
{% include "json/campaign-nodelevelhealthtriggerediv.json" %}
```
