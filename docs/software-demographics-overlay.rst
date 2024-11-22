==========================
Demographics overlay files
==========================

You can specify multiple demographics files, which function as a "base layer" file and one
or more "overlay" files that override the base layer configuration. Overlay files can change the
value of parameters already specified in the base layer or add new parameters. Support for multiple
demographics layers allows for the following scenarios:

* Separating different sets of parameters and values into individual layers (for example, to separate
  those that are useful for specific diseases into different layers)
* Adding new parameters for a simulation into a new layer for easier prototyping
* Overriding certain parameters of interest in a new layer
* Overriding certain parameters for a particular sub-region
* Simulating subsets of a larger region for which input files have been constructed


To use an overlay file:

#.  Select the demographics file to use as the base layer file. All nodes to be included in the simulation
    must be listed in this file.
#.  In the metadata, make note of the **IdReference** value.

    You may change this value if you desire, but all input files for a simulation must have the
    same **IdReference** value. For more information about this parameter and the structure of
    demographics files in general, see :doc:`parameter-demographics`.

#.  Create one or more overlay files. Keep the following things in mind:

    * In the metadata, the value for **IdReference** must match the value in the base layer file and
      all other input files except configuration and campaign.
    * Any nodes listed in an overlay but not in the base layer will not be simulated.
    * If the demographics files include any JSON array elements, the entire array is overridden.
      You cannot add or remove individual elements in an array using an overlay file.
    * Overriding a parameter value on a node will not affect the other parameter values applied to
      that node.
    * Values set in the **Defaults** section of an overlay will be applied to all nodes listed in
      that file, not all nodes in the entire simulation. Therefore, an overlay file that includes a
      **Defaults** section but no **Nodes** section will not have any effect.

#.  Place all demographics files in the directory where the other input files are stored.

#.  In the :term:`configuration file`, set **Demographics_Filenames** to an array that contains a
    comma-delimited list of demographics files, listing the base layer file first.

An example base layer demographics file and an overlay file is below. You can see that the overlay
adds the **TransmissionMatrix** for |HINT_l| to only three of the five nodes (which correspond to
Washington state counties).

.. literalinclude:: json/howto-demographics-base-layer.json
   :language: json

.. literalinclude:: json/howto-demographics-overlay.json
   :language: json