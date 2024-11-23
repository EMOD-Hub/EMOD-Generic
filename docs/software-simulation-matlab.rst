====================================
Run a simulation using MATLAB
====================================


If you used MATLAB to create or modify JSON files as shown in :doc:`software-configuration-script`,
it may be convenient to invoke |exe_s| to run a simulation from a
MATLAB script. One way of doing this is shown below using the dos_ command.

.. code-block:: matlab

    exe_name = fullfile( binDirectory, 'Eradication.exe' );
    exe_args = [ '-C config.json -I ', fullfile( inputDirectory, 'Namawala' ) ];
    [status,result] = dos( ['cd ', WorkDirectory, ' && ', exe_name, ' ', exe_args ], '-echo' );

.. _dos: http://www.mathworks.com/help/matlab/ref/dos.html

See :doc:`software-simulation-cli` for more information about the command options available for use
with |exe_s|.

