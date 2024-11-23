=================
ImmunityBloodTest
=================

The **ImmunityBloodTest** intervention class identifies whether an individual's immunity meets a specified threshold (as set with the **Positive_Threshold_AcquisitionImmunity** campaign parameter) and then broadcasts an event based on the results; positive has immunity while negative does not. 

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-immunitybloodtest.csv

.. literalinclude:: json/campaign-immunitybloodtest.json
   :language: json