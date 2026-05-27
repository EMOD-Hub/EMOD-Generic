# SimpleBoosterVaccine

The **SimpleBoosterVaccine** intervention class is derived from [parameter-campaign-individual-simplevaccine](parameter-campaign-individual-simplevaccine.md)
and preserves many of the same parameters. The behavior is much like **SimpleVaccine**, except that upon distribution
and successful take, the vaccine's effect is determined by the recipient's immune state. If the
recipient’s immunity modifier in the corresponding channel (acquisition, transmission, or mortality) is
above a user-specified threshold, then the vaccine’s initial effect will be equal to the
corresponding priming parameter. If the recipient’s immune modifier is below this threshold, then
the vaccine's initial effect will be equal to the corresponding boosting parameter. After
distribution, the effect wanes, just like **SimpleVaccine**. In essence, this intervention
provides a **SimpleVaccine** intervention with one effect to all naive (below- threshold)
individuals, and another effect to all primed (above-threshold) individuals; this behavior is
intended to mimic biological priming and boosting.

!!! note

        Parameters are case-sensitive. For Boolean parameters, set to 1 for true or 0 for false.
        Minimum, maximum, or default values of "NA" indicate that those values are not applicable for
        that parameter.

        EMOD does not use true defaults; that is, if the dependency relationships indicate that a 
        parameter is required, you must supply a value for it. However, many of the tools used to work 
        with EMOD will use the default values provided below.

        JSON format does not permit comments, but you can add "dummy" parameters to add contextual
        information to your files. Any keys that are not EMOD parameter names will be ignored by the
        model.

The table below describes all possible parameters with which this class can be configured. The JSON
example that follows shows one potential configuration.

{{ read_csv("csv/campaign-simpleboostervaccine.csv") }}

[link](../json/campaign-simpleboostervaccine.json)
