# SurveillanceEventCoordinator

!!! note

        This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **SurveillanceEventCoordinator** coordinator class listens for and detects events happening and then responds with broadcasted events when a threshold has been met. This campaign
event is typically used with other classes, such as [parameter-campaign-event-broadcastcoordinatorevent](parameter-campaign-event-broadcastcoordinatorevent.md), [parameter-campaign-event-triggeredeventcoordinator](parameter-campaign-event-triggeredeventcoordinator.md), and [parameter-campaign-event-delayeventcoordinator](parameter-campaign-event-delayeventcoordinator.md).

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

{{ read_csv("csv/campaign-surveillanceeventcoordinator.csv") }}

[link](../json/campaign-surveillanceeventcoordinator.json)
