# ControlledVaccine

The **ControlledVaccine** intervention class is a subclass of [parameter-campaign-individual-simplevaccine](parameter-campaign-individual-simplevaccine.md)
so it contains all functionality of **SimpleVaccine**, but provides more control over
additional events and event triggers. This intervention can be configured so that specific events
are broadcast when individuals receive an intervention or when the intervention expires. Further,
individuals can be re-vaccinated, using a configurable wait time between vaccinations.

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

{{ read_csv("csv/campaign-controlledvaccine.csv") }}

```json
{% include "json/campaign-controlledvaccine.json" %}
```
