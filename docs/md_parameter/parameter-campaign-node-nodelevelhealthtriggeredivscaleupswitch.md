# NodeLevelHealthTriggeredIVScaleUpSwitch

The **NodeLevelHealthTriggeredIVScaleUpSwitch** intervention class transitions from one intervention
to another over time. Generally this is used if one type of diagnostic tool is being phased out but
the transition to replace it with the new diagnostic takes place over a few years. The individuals
who are included by **Demographic_Coverage** will receive the new intervention and those that
aren't will receive the older "not covered" intervention.

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

{{ read_csv("csv/campaign-nodelevelhealthtriggeredivscaleupswitch.csv") }}

[link](../json/campaign-nodelevelhealthtriggeredivscaleupswitch.json)
