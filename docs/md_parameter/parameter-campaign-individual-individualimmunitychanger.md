# IndividualImmunityChanger

The **IndividualImmunityChanger** intervention class acts essentially as a
[parameter-campaign-individual-multieffectvaccine](parameter-campaign-individual-multieffectvaccine.md),
with the exception of how the behavior is implemented. Rather than
attaching a persistent vaccine intervention object to an individual’s intervention list (as a
campaign-individual-multieffectboostervaccine does), the **IndividualImmunityChanger** directly
alters the immune modifiers of the individual’s susceptibility object and is then immediately disposed
of. Any immune waning is not governed by [parameter-campaign-waningeffects](parameter-campaign-waningeffects.md), as [parameter-campaign-individual-multieffectvaccine](parameter-campaign-individual-multieffectvaccine.md) is, but rather
by the immunity waning parameters in the configuration file.

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

{{ read_csv("csv/campaign-individualimmunitychanger.csv") }}

```json
{% include "json/campaign-individualimmunitychanger.json" %}
```
