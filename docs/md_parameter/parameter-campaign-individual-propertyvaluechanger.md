# PropertyValueChanger

The **PropertyValueChanger** intervention class assigns new individual property values to
individuals. You must update one property value and have the option to update another using
**New_Property_Value**. This parameter is generally used to move patients from one intervention
state in the health care cascade (InterventionStatus) to another, though it can be used for any
individual property. Individual property values are user-defined in the demographics file (see
[NodeProperties and IndividualProperties](parameter-demographics.md#nodeproperties-and-individualproperties) for more information). Note that the HINT feature
does not need to be enabled to use this intervention. To instead change node properties, use
[parameter-campaign-node-nodepropertyvaluechanger](parameter-campaign-node-nodepropertyvaluechanger.md).

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

{{ read_csv("csv/campaign-propertyvaluechanger.csv") }}

[link](../json/campaign-propertyvaluechanger.json)
