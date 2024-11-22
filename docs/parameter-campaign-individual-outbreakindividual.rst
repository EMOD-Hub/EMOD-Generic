==================
OutbreakIndividual
==================

The **OutbreakIndividual** intervention class introduces contagious diseases that are compatible
with the simulation type to existing individuals using the individual targeted features configured
in the appropriate event coordinator. To instead add new infection individuals, use :doc:`parameter-campaign-node-outbreak`.

.. include:: reuse/warning-case.txt


.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-outbreakindividual.csv

.. literalinclude:: json/campaign-outbreakindividual.json
   :language: json