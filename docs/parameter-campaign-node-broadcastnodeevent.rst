==================
BroadcastNodeEvent
==================

.. note::

	This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **BroadcastNodeEvent** node intervention class broadcasts node events. This can be used with the campaign class, :doc:`parameter-campaign-event-surveillanceeventcoordinator`, that can monitor and listen for events received from **BroadcastNodeEvent** and then perform an action based on the broadcasted event. You can also use this for the reporting of the broadcasted events by setting the configuraton parameters, **Report_Node_Event_Recorder** and **Report_Surveillance_Event_Recorder**, which listen to events to be recorded. You must use this coordinator class with listeners that are operating on the same core. For more information, see :doc:`dev-architecture-core`.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-broadcastnodeevent.csv

.. literalinclude:: json/campaign-broadcastnodeevent.json
   :language: json