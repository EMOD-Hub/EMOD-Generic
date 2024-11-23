========
Outbreak
========

The **Outbreak** class allows the introduction of a disease outbreak event by the addition of new
infected individuals or infection of existing individuals in a node. **Outbreak** is a node-level
intervention; to distribute an outbreak to specific categories of existing individuals within a
node, use :doc:`parameter-campaign-individual-outbreakindividual`.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-outbreak.csv

.. literalinclude:: json/campaign-outbreak.json
   :language: json