# Run a simulation using Python

If you used Python to create or modify JSON files as shown in [software-configuration-script](software-configuration-script.md),
it may be convenient to invoke Eradication.exe to run a simulation from a
Python script. One way of doing this is shown below using the subprocess_ package.

```python

    import subprocess

    # specify paths
    binary_path = "binDirectory\Eradication.exe"
    input_path = "inputDirectory\Namawala\

    # commission job
    subprocess.call( [binary_path, "-C", "config.json", "--input", input_path] )
```

See [software-simulation-cli](software-simulation-cli.md) for more information about the command options available for use
with Eradication.exe.
