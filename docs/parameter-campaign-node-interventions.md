# Node-level interventions

Node-level interventions determine *what* will be distributed to each **node** to reduce the spread of a
disease. For example, spraying larvicide in a village to kill mosquito larvae is a node-level malaria
intervention. Sometimes this can be an intermediate intervention that schedules another
intervention. Node-level disease outbreaks are also configured as "interventions". In the schema,
these are labeled as **NodeTargeted**.

It is also possible (but not required) to configure *why* a particular intervention is distributed
by adding trigger conditions to the intervention. For example, interventions can be triggered by
notifications broadcast after some an event, such as Births, NewInfectionEvent, and more. It's also
possible to have one intervention trigger another intervention by asking the first intervention to
broadcast a unique string, and having the second intervention be triggered upon receipt of that
string. See [parameter-campaign-event-list](parameter-campaign-event-list.md).

- [parameter-campaign-node-birthtriggerediv](parameter-campaign-node-birthtriggerediv.md)
- [parameter-campaign-node-broadcastnodeevent](parameter-campaign-node-broadcastnodeevent.md)
- [parameter-campaign-node-importpressure](parameter-campaign-node-importpressure.md)
- [parameter-campaign-node-migratefamily](parameter-campaign-node-migratefamily.md)
- [parameter-campaign-node-multinodeinterventiondistributor](parameter-campaign-node-multinodeinterventiondistributor.md)
- [parameter-campaign-node-nodelevelhealthtriggerediv](parameter-campaign-node-nodelevelhealthtriggerediv.md)
- [parameter-campaign-node-nodelevelhealthtriggeredivscaleupswitch](parameter-campaign-node-nodelevelhealthtriggeredivscaleupswitch.md)
- [parameter-campaign-node-nodepropertyvaluechanger](parameter-campaign-node-nodepropertyvaluechanger.md)
- [parameter-campaign-node-outbreak](parameter-campaign-node-outbreak.md)
