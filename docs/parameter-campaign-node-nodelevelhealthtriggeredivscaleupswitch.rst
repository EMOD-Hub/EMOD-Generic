=======================================
NodeLevelHealthTriggeredIVScaleUpSwitch
=======================================

The **NodeLevelHealthTriggeredIVScaleUpSwitch** intervention class transitions from one intervention
to another over time. Generally this is used if one type of diagnostic tool is being phased out but
the transition to replace it with the new diagnostic takes place over a few years. The individuals
who are included by **Demographic_Coverage** will receive the new intervention and those that
aren't will receive the older "not covered" intervention.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-nodelevelhealthtriggeredivscaleupswitch.csv

.. literalinclude:: json/campaign-nodelevelhealthtriggeredivscaleupswitch.json
   :language: json