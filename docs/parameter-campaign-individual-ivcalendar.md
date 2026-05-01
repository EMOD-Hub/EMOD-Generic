# IVCalendar

The **IVCalendar** intervention class contains a list of ages when an individual will receive the
actual intervention. In **IVCalendar**, there is a list of actual interventions where the
distribution is dependent on whether the individual's age matches the next date in the calendar.
This implies that at a certain age, the list of actual interventions will be distributed according
to a given probability. While a typical use case might involve the distribution of calendars by a
[parameter-campaign-node-birthtriggerediv](parameter-campaign-node-birthtriggerediv.md) in the context of a routine vaccination schedule, calendars
may also be distributed directly to individuals.

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

{{ read_csv("csv/campaign-ivcalendar.csv") }}

```json
{% include "json/campaign-ivcalendar.json" %}
```
