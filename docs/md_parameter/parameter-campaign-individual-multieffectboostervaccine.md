# MultiEffectBoosterVaccine

The **MultiEffectBoosterVaccine** intervention class is derived from **MultiEffectVaccine** and
preserves many of the same parameters. Upon distribution and successful take, the vaccine’s effect
in each immunity compartment (acquisition, transmission,  and mortality) is determined by the
recipient’s immune state. If the recipient’s immunity modifier in the corresponding compartment is
above a user-specified threshold, then the vaccine’s initial effect will be equal to the
corresponding priming parameter. If the recipient’s immune modifier is below this threshold, then
the vaccine’s initial effect will be equal to the corresponding boost parameter. After distribution,
the effect wanes, just like a [parameter-campaign-individual-multieffectvaccine](parameter-campaign-individual-multieffectvaccine.md). The behavior
is intended to mimic biological priming and boosting.

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

{{ read_csv("csv/campaign-multieffectboostervaccine.csv") }}

```json
{% include "json/campaign-multieffectboostervaccine.json" %}
```
