# Run a simulation using MATLAB

If you used MATLAB to create or modify JSON files as shown in [software-configuration-script](software-configuration-script.md),
it may be convenient to invoke Eradication.exe to run a simulation from a
MATLAB script. One way of doing this is shown below using the dos_ command.

```matlab

    exe_name = fullfile( binDirectory, 'Eradication.exe' );
    exe_args = [ '-C config.json -I ', fullfile( inputDirectory, 'Namawala' ) ];
    [status,result] = dos( ['cd ', WorkDirectory, ' && ', exe_name, ' ', exe_args ], '-echo' );
```

See [software-simulation-cli](software-simulation-cli.md) for more information about the command options available for use
with Eradication.exe.
