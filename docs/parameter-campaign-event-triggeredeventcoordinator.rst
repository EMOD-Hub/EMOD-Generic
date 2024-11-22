=========================
TriggeredEventCoordinator
=========================

.. note::

    This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **TriggeredEventCoordinator** coordinator class listens for trigger events, begins a series of repetitions of intervention distributions, and then broadcasts an event upon completion. This campaign
event is typically used with other classes that broadcast and distribute events, such as
:doc:`parameter-campaign-event-broadcastcoordinatorevent`, :doc:`parameter-campaign-event-delayeventcoordinator`, and :doc:`parameter-campaign-event-surveillanceeventcoordinator`.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-triggeredeventcoordinator.csv

.. literalinclude:: json/campaign-triggeredeventcoordinator.json
   :language: json