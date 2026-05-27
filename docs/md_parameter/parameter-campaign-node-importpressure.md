# ImportPressure

The **ImportPressure** intervention class extends the **ImportCases** outbreak event. Rather than importing a
deterministic number of cases on a scheduled day, **ImportPressure** applies a set of per-day rates
of importation of infected individuals, over a corresponding set of durations. **ImportPressure**
inherits from **Outbreak**; the **Antigen** and **Genome** parameters are defined as they are for all
**Outbreak** events.

!!! warning

        Be careful when configuring import pressure in multi-node simulations.
        **Daily_Import_Pressures**  defines a rate of per-day importation for *each* node that the
        intervention is distributed to. In a 10 node simulation with  **Daily_Import_Pressures** = [0.1,
        5.0], the total importation rate summed over all nodes will be 1/day and 50/day during the two
        time periods. You must divide the per-day importation rates by the number of
        nodes.

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

{{ read_csv("csv/campaign-importpressure.csv") }}

[link](../json/campaign-importpressure.json)
