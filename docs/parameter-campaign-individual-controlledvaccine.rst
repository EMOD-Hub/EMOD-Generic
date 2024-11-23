=================
ControlledVaccine
=================

The **ControlledVaccine** intervention class is a subclass of :doc:`parameter-campaign-individual-simplevaccine`
so it contains all functionality of **SimpleVaccine**, but provides more control over
additional events and event triggers. This intervention can be configured so that specific events
are broadcast when individuals receive an intervention or when the intervention expires. Further,
individuals can be re-vaccinated, using a configurable wait time between vaccinations.

.. include:: reuse/warning-case.txt

.. include:: reuse/campaign-example-intro.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-controlledvaccine.csv

.. literalinclude:: json/campaign-controlledvaccine.json
   :language: json
