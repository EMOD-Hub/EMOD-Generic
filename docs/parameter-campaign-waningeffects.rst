=====================
Waning effect classes
=====================

The following classes are nested within interventions (both individual-level and node-level) to
indicate how their efficacy wanes over time. They can be used with several parameters including
**Blocking_Config**, **Killing_Config**, and **Waning_Config**. Note that waning effect parameters
do not control the overall duration of an intervention and are not assigned probabilistically. 

.. include:: reuse/warning-case.txt

See the example below that uses a mix of different waning effect classes and the tables below that
describe all parameters that can be used with each waning effect class.

.. literalinclude:: json/campaign-waningeffects.json
   :language: json

.. contents:: Contents
   :local:

WaningEffectBox
===============

The efficacy is held at a constant rate until it drops to zero after the user-defined duration.

.. literalinclude:: json/campaign-waningeffectbox.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectbox.csv



WaningEffectBoxExponential
==========================

The initial efficacy is held for a specified duration, then the efficacy decays at an exponential rate where the current effect is equal to **Initial_Effect** - dt/**Decay_Time_Constant**.

.. literalinclude:: json/campaign-waningeffectboxexponential.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectboxexponential.csv



WaningEffectCombo
=================

The **WaningEffectCombo** class is used within individual-level interventions and allows for specifiying a list of effects when the intervention only has one **WaningEffect** defined. These effects can be added or multiplied.

.. literalinclude:: json/campaign-waningeffectcombo.json
   :language: json


.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectconstant.csv



WaningEffectConstant
====================

The efficacy is held at a constant rate.

.. literalinclude:: json/campaign-waningeffectconstant.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectconstant.csv



WaningEffectExponential
=======================

The efficacy decays at an exponential rate where the current effect is equal to **Initial_Effect** - dt/**Decay_Time_Constant**.

.. literalinclude:: json/campaign-waningeffectexponential.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectexponential.csv


WaningEffectMapLinear
=====================

The efficacy decays based on the time since the start of the intervention. This change is defined
by a map of time to efficacy values in which the time between time/value points is linearly
interpolated. When the time since start reaches the end of the times in the map, the last value will
be used unless the intervention expires. If the time since start is less than the first value in the
map, the efficacy will be zero. This can be used to define the shape of a curve whose magnitude is
defined by the **Initial_Effect** multiplier.

.. literalinclude:: json/campaign-waningeffectmaplinear.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectmaplinear.csv


WaningEffectMapLinearAge
========================

Similar to **WaningEffectMapLinear**, except that the efficacy decays based on the age of the
individual who owns the intervention instead of the time since the start of the intervention.

.. literalinclude:: json/campaign-waningeffectmaplinearage.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectmaplinearage.csv


WaningEffectMapLinearSeasonal
=============================

Similar to **WaningEffectMapLinear**, except that the map will repeat itself every 365 days. That
is, the time since start will reset to zero once it reaches 365.  This allows you to simulate
seasonal effects.

.. literalinclude:: json/campaign-waningeffectmaplinearseasonal.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectmaplinearseasonal.csv


WaningEffectMapPiecewise
========================

Similar to **WaningEffectMapLinear**, except that the data is assumed to be constant between
time/value points (no interpolation). If the time since start falls between two points, the efficacy
of the earlier time point is used.

.. literalinclude:: json/campaign-waningeffectmappiecewise.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectmappiecewise.csv


WaningEffectRandomBox
=====================

The efficacy is held at a constant rate until it drops to zero after a user-defined duration. This
duration is randomly selected from an exponential distribution where **Expected_Discard_Time** is
the mean.

.. literalinclude:: json/campaign-waningeffectrandombox.json
   :language: json

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/campaign-waningeffectrandombox.csv

