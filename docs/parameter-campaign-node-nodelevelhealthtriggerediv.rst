==========================
NodeLevelHealthTriggeredIV
==========================

The **NodeLevelHealthTriggeredIV** intervention class distributes an intervention to an individual when a
specific event occurs. **NodeLevelHealthTriggeredIV** monitors for event triggers from individuals,
and when found, will distribute the intervention. For example, **NodeLevelHealthTriggeredIV** can be
configured such that all individuals will be given a diagnostic intervention when they transition
from susceptible to infectious. During the simulation, when individuals become infected, they
broadcast the **NewInfectionEvent** trigger and **NodeLevelHealthTriggeredIV** distributes the
diagnostic intervention to them.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-nodelevelhealthtriggerediv.csv

.. literalinclude:: json/campaign-nodelevelhealthtriggerediv.json
   :language: json