=======================
Scalars and multipliers
=======================

The following parameters scale or multiply values set in other areas of the configuration file or other
:term:`input files`. This can be especially useful for understanding the sensitivities of
disease dynamics to input data without requiring modifications to those base values. For example,
one might set **x_Birth** to a value less than 1 to simulate a lower future birth rate due to increased
economic prosperity and available medical technology.


.. include:: reuse/warning-case.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/config-scalars-generic.csv