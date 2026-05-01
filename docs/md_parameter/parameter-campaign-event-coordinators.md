# Events and event coordinators

Campaign events determine *when* and *where* an intervention is distributed during a campaign. "When"
can be the number of days after the beginning of the simulation or at a point during a particular
calendar year. "Where" is the geographic **node** or nodes in which the event takes place.

Event coordinators are nested within the campaign event JSON object and determine *who* receives the
intervention. "Who" is determined by filtering on age, gender, or on the individual properties
configured in the demographics files, such as risk group or sociodemographic category. See [model-properties](../md_model/model-properties.md).

