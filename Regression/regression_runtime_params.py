import os

LOCAL_SIM_ROOT = 'outputs'
LOCAL_BIN_ROOT = 'bin'


class RuntimeParameters:
    def __init__(self, args):
        print("os = " + os.name)
        self.args = args

        if os.name == "posix":
            self.os_type = "POSIX"
        else:
            self.os_type = "WINDOWS"

        # Jenkins stuff
        self.mod_path = None
        if os.path.exists("regression_test.cfg"):
            with open("regression_test.cfg") as fid01:
                flines = [val.strip() for val in fid01.readlines()]
            linux_block = True
            for lval in flines:
                if (not linux_block and 'POSIX' not in lval):
                    continue
                elif (not linux_block and 'POSIX' in lval):
                    linux_block = True
                elif ('local_sim_root' in lval):
                    self.mod_path = (lval.split('=')[1]).strip()
                    break

        self.display()

    def display(self):
        print("[arg] Suite:                      ", self.suite)
        print("[arg] Executable path:            ", self.executable_path)
        print("[arg] Python VE path:             ", self.py_path)
        print("[arg] Run in perf mode:           ", self.measure_perf)
        print("[arg] Use DLLs:                   ", self.use_dlls)
        print("[arg] SCons:                      ", self.scons)
        print("[arg] Timings:                    ", self.timing)
        print("[arg] Disable schema test:        ", self.disable_schema_test)
        print("[arg] Component tests:            ", self.component_tests)
        print("[arg] Config constraints:         ", self.constraints_dict)
        print("[cfg] DLL root:                   ", self.dll_root)
        print("[cfg] Local bin root:             ", self.local_bin_root)
        print("[cfg] Local sim root:             ", self.local_sim_root)
        print("[cfg] Source root:                ", self.src_root)
        return

    @property
    def suite(self):
        return self.args.suite

    @property
    def executable_path(self):
        path = self.args.exe_path
        if not path:
            if self.scons:
                path = "../build/x64/Release/Eradication/Eradication"
                if os.name == "nt":
                    path += ".exe"
            else:
                path = "../Eradication/x64/Release/Eradication.exe"

        return path

    @property
    def py_path(self):
        return self.args.py_path

    @property
    def timing(self):
        return self.args.timing

    @property
    def measure_perf(self):
        return self.args.perf

    @property
    def use_dlls(self):
        return self.args.use_dlls

    @property
    def scons(self):
        return self.args.scons

    @property
    def local_sim_root(self):
        if (self.mod_path):
            out_path = os.path.join(self.mod_path, LOCAL_SIM_ROOT)
        else:
            out_path = LOCAL_SIM_ROOT
        return out_path

    @property
    def local_bin_root(self):
        return LOCAL_BIN_ROOT

    @property
    def dll_root(self):
        return LOCAL_BIN_ROOT

    @property
    def src_root(self):
        return ".."

    @property
    def disable_schema_test(self):
        return self.args.disable_schema_test

    @property
    def component_tests(self):
        return self.args.component_tests

    @property
    def constraints_dict(self):
        constraints_dict = {}
        if self.args.config_constraints and len(self.args.config_constraints) > 0:
            constraints_list = self.args.config_constraints.split(",")
            for raw_nvp in constraints_list:
                nvp = raw_nvp.split(":")
                constraints_dict[nvp[0]] = nvp[1]
        return constraints_dict
