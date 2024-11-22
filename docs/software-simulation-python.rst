====================================
Run a simulation using Python
====================================

If you used Python to create or modify JSON files as shown in :doc:`software-configuration-script`,
it may be convenient to invoke |exe_s| to run a simulation from a
Python script. One way of doing this is shown below using the subprocess_ package.

.. code-block:: python

    import subprocess

    # specify paths
    binary_path = "binDirectory\Eradication.exe"
    input_path = "inputDirectory\Namawala\

    # commission job
    subprocess.call( [binary_path, "-C", "config.json", "--input", input_path] )


.. _subprocess: https://docs.python.org/3/library/subprocess.html

See :doc:`software-simulation-cli` for more information about the command options available for use
with |exe_s|.
