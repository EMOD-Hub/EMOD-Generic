============================
SurveillanceEventCoordinator
============================

.. note:: 

    This campaign class and associated parameters are currently in beta release and have not yet been fully tested.

The **SurveillanceEventCoordinator** coordinator class listens for and detects events happening and then responds with broadcasted events when a threshold has been met. This campaign
event is typically used with other classes, such as :doc:`parameter-campaign-event-broadcastcoordinatorevent`, :doc:`parameter-campaign-event-triggeredeventcoordinator`, and :doc:`parameter-campaign-event-delayeventcoordinator`.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-surveillanceeventcoordinator.csv

.. literalinclude:: json/campaign-surveillanceeventcoordinator.json
   :language: json