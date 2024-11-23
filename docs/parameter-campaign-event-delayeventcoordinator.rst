=====================
DelayEventCoordinator
=====================

.. note::

    This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **DelayEventCoordinator** coordinator class insert delays into coordinator event chains. This campaign
event is typically used with :doc:`parameter-campaign-event-broadcastcoordinatorevent` to broadcast events after the delays.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-delayeventcoordinator.csv

.. literalinclude:: json/campaign-delayeventcoordinator.json
   :language: json