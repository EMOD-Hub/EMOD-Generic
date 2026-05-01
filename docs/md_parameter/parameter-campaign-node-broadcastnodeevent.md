# BroadcastNodeEvent

!!! note

    	This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **BroadcastNodeEvent** node intervention class broadcasts node events. This can be used with the campaign class, [parameter-campaign-event-surveillanceeventcoordinator](parameter-campaign-event-surveillanceeventcoordinator.md), that can monitor and listen for events received from **BroadcastNodeEvent** and then perform an action based on the broadcasted event. You can also use this for the reporting of the broadcasted events by setting the configuraton parameters, **Report_Node_Event_Recorder** and **Report_Surveillance_Event_Recorder**, which listen to events to be recorded. You must use this coordinator class with listeners that are operating on the same core. For more information, see [dev-architecture-core](../md_dev/dev-architecture-core.md).

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

{{ read_csv("csv/campaign-broadcastnodeevent.csv") }}

[link](../json/campaign-broadcastnodeevent.json)
