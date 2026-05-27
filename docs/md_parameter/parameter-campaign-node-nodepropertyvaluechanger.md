# NodePropertyValueChanger

The **NodePropertyValueChanger** intervention class sets a given node property to a new value. You can
also define a duration in days before the node property reverts back to its original value, the
probability that a node will change its node property to the target value, and the number of days
over which nodes will attempt to change their individual properties to the target value. This
node-level intervention functions in a similar manner as the individual-level intervention,
[parameter-campaign-individual-propertyvaluechanger](parameter-campaign-individual-propertyvaluechanger.md).

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

{{ read_csv("csv/campaign-nodepropertyvaluechanger.csv") }}

[link](../json/campaign-nodepropertyvaluechanger.json)
