import subprocess
import datetime
import json
import time
import os
import pdb

import regression_local_monitor
import regression_utils as ru
import regression_clg as clg

class HpcMonitor(regression_local_monitor.Monitor):
    def __init__(self, sim_id, scenario_path, report, params, suffix, config_json=None, scenario_type='tests',
                 priority='Normal'):
        #super(regression_local_monitor.Monitor,self).__init__( sim_id, scenario_path, report, params, config_json, scenario_type )
        regression_local_monitor.Monitor.__init__( self, sim_id, scenario_path, report, params, config_json, scenario_type )
        #print "Running DTK execution and monitor thread for HPC commissioning."
        self.sim_root = self.params.sim_root # override base Monitor which uses local sim directory
        self.config_json = config_json
        self.scenario_path = scenario_path
        self.suffix = suffix
        self.options = {'/priority:': priority}

        # Prepare job command line options as they're available at this point
        self.prepare_options()

    def prepare_options(self):
        self.options['/scheduler:'] = self.params.hpc_head_node
        self.options['/nodegroup:'] = self.params.hpc_node_group
        if self.params.hpc_user != '':
            self.options['/user:'] = self.params.hpc_user
        if self.params.measure_perf:
            self.options['/exclusive'] = ' '
        if self.scenario_type == 'tests':
            self.options['/stdout:'] = 'StdOut.txt'
        self.options['/stderr:'] = 'StdErr.txt'

    def test_submission(self):

        jobsubmit_bin = 'job submit'
        jobsubmit_command = clg.CommandlineGenerator(jobsubmit_bin, self.options, ['dir'])
        hpc_command_line = jobsubmit_command.Commandline

        p = subprocess.Popen(hpc_command_line, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p.wait(timeout=30)
        assert p.returncode == 0, hpc_command_line

    def run(self):
        self.__class__.sems.acquire()
        self.sim_dir = self.sim_root + "\\" + self.sim_timestamp   # can't use os.path.join() here because on linux it'll give us the wrong dir-separator...
        if self.suffix is not None:
            job_name = self.config_json["parameters"]["Config_Name"].replace( ' ', '_' ) + "_" + self.suffix + "_(" + self.sim_timestamp + ")"
        else:
            job_name = self.config_json["parameters"]["Config_Name"].replace( ' ', '_' ) + "_(" + self.sim_timestamp + ")"
        job_name = job_name[:79]

        numcores = self.get_num_cores()

        hpc_resource_option = '/numcores:'
        hpc_resource_count  = str(numcores)
        mpi_core_option = None
        mpi_core_count  = ''

        #eradication.exe commandline
        eradication_bin = self.config_json['bin_path']
        eradication_options = {}
        if "Eradication" in eradication_bin:
            eradication_options = { '--config':'config.json', '--progress':' ' }

        # python-script-path is optional parameter.
        if "PSP" in self.config_json:
            eradication_options[ "--python-script-path" ] = self.config_json["PSP"]
        eradication_params = []
        eradication_command = clg.CommandlineGenerator(eradication_bin, eradication_options, eradication_params)

        #mpiexec commandline
        mpi_bin = 'mpiexec'
        mpi_options = {}
        if mpi_core_option is not None:
            mpi_options[mpi_core_option] = mpi_core_count
        mpi_params = [eradication_command.Commandline]
        mpi_command = clg.CommandlineGenerator(mpi_bin, mpi_options, mpi_params)
        
        #job submit commandline
        jobsubmit_bin = 'job submit'
        self.options['/workdir:'] = self.sim_dir
        self.options['/jobname:'] = job_name
        self.options[hpc_resource_option] = hpc_resource_count
        jobsubmit_params = [mpi_command.Commandline]
        jobsubmit_command = clg.CommandlineGenerator(jobsubmit_bin, self.options, jobsubmit_params)

        hpc_command_line = jobsubmit_command.Commandline
        if self.scenario_type != 'tests':
            hpc_command_line = hpc_command_line  + " ^> Test.txt"

        job_id = -1
        num_retries = -1

        while job_id == -1:
            num_retries += 1
            p = subprocess.Popen( hpc_command_line, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE )
            [hpc_pipe_stdout, hpc_pipe_stderr] = p.communicate()
            line = hpc_pipe_stdout

            if p.returncode == 0:
                job_id = int(line.decode().split( ' ' )[-1].strip().rstrip('.'))
                if job_id > 0:
                    print( self.scenario_path + " submitted (as job_id " + str(job_id) + ")\n" )
                else:
                    print( "ERROR: What happened here?  Please send this to Jeff:\n" )
                    print( hpc_pipe_stdout )
                    print( hpc_pipe_stderr )
                    job_id = -1
            else:
                print( "ERROR: job submit of " + self.scenario_path + " failed!" )
                print( hpc_pipe_stdout )
                print( hpc_pipe_stderr )

            if job_id == -1 and num_retries >= 5 and self.params.hide_graphs:
                print( "Job submission failed multiple times for " + self.scenario_path + ".  Aborting this test and logging error." )
                self.report.addErroringTest( self.scenario_path, "", self.sim_dir, self.scenario_type )
                return

        monitor_cmd_line = "job view /scheduler:" + self.params.hpc_head_node + " " + str(job_id)

        check_status = True
        while check_status:
            hpc_pipe = subprocess.Popen( monitor_cmd_line.split(), shell=False, stdout=subprocess.PIPE )
            [hpc_pipe_stdout, hpc_pipe_stderr] = hpc_pipe.communicate()
            lines = hpc_pipe_stdout
            for line in lines.decode().split('\n'):
                res = line.split( ':' )
                if res[0].strip() == "State":
                    state = res[1].strip()
                    if state == "Failed":
                        self.__class__.completed = self.__class__.completed + 1
                        print( self.scenario_path + " FAILED!" )
                        check_status = False
                        self.report.addErroringTest( self.scenario_path, "", self.sim_dir, self.scenario_type )
                    if state == "Canceled":
                        self.__class__.completed = self.__class__.completed + 1
                        print( "Canceled!" )
                        check_status = False
                    elif state == "Completed" or state == "Finished":
                        self.__class__.completed = self.__class__.completed + 1
                        print( str(self.__class__.completed) + " out of " + str(len(ru.reg_threads)) + " completed." )
                        check_status = False

                        if self.scenario_type != 'pymod':
                            with open( os.path.join(self.sim_dir, "status.txt"), "r" ) as status_file:
                                for status_line in status_file.readlines():
                                    if status_line.startswith("Done"):
                                        time_split = status_line.split('-')[1].strip().split(':')
                                        self.duration = datetime.timedelta(hours=int(time_split[0]), minutes=int(time_split[1]), seconds=int(time_split[2]))
                                        break

                        if self.scenario_type == 'tests':
                            if self.params.all_outputs == False:
                            # Following line is for InsetChart.json only
                                self.verify(self.sim_dir)
                            else:
                                # Every .json file in output (not hidden with . prefix) will be used for validation
                                for file in os.listdir( os.path.join( self.scenario_path, "output" ) ):
                                    if ( file.endswith( ".json" ) or file.endswith( ".csv" ) or file.endswith( ".kml" ) or file.endswith( ".bin" ) or file.endswith( ".h5" ) or file.endswith( ".db" ) ) and file[0] != "." and file != "transitions.json" and "linux" not in file:
                                        self.verify( self.sim_dir, file, "Channels" )
                        elif self.scenario_type == 'science':   # self.report <> None:
                            self.science_verify( self.sim_dir )

                        elif self.scenario_type == 'pymod':   # self.report <> None:
                            self.pymod_verify( self.sim_dir )

                    break
            time.sleep(5)
        self.__class__.sems.release()
