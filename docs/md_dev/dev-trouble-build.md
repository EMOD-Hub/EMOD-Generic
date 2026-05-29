# Troubleshooting EMOD builds

If you encounter any of the following warnings or errors when attempting to build the EMOD executable (Eradication.exe) or
Eradication binary for Linux, see the information below to resolve the issue.

If you encounter an issue, use the GitHub link at the top of the page to submit a new issue in the repo. Be sure to include any error information. For general discussion and questions, please visit https://github.com/orgs/EMOD-Hub/discussions.

## Unknown compiler version

If you encounter the warning "Unknown compiler version - please run the configure tests and report
the results" when attempting to build the Eradication.exe or Eradication binary for Linux, this indicates you are using a
version of Boost that is no longer supported. Install Boost 1.77.0.

## Inconsistent DLL linkage

If you see the following warning on some files, "c:python27includepymath.h(22): warning C4273:
‘round’: inconsistent dll linkage", this indicates that you are using a version of Python that is
no longer supported. Install Python 3.9 64-bit.

## Error 255

Check to see if you have any white spaces in the path to your local EMOD source code. If you do,
remove the white spaces in all of the directory names in the path.

## Error LNK4272

If you see the error "LNK4272 library machine type 'X86' conflicts with target machine type 'x64'",
this indicates that you need to uninstall 32-bit Python and reinstall 64-bit Python. Install
Python 3.9 64-bit.
