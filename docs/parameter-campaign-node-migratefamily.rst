=============
MigrateFamily
=============

The **MigrateFamily** intervention class tells residents of the targeted node to go on a family trip. The
duration of time residents wait before migration can be configured; the "timer" for this duration
does not start until all residents are "home".

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-migratefamily.csv

.. literalinclude:: json/campaign-migratefamily.json
   :language: json