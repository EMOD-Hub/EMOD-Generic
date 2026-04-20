import os
import json
import threading
import subprocess
import glob
import regression_local_monitor
import regression_utils as ru
import sys
import shutil
import pdb

class MyRegressionRunner(object):

    # static variables
    emodules_map = {}

    # class variables
    campaign_filename = "campaign.json"

    def __init__(self, params):
        self.params = params
        try:
            self.dtk_hash = ru.md5_hash_of_file(self.params.executable_path)
        except Exception as ex:
            self.dtk_hash = None
            print( "Exception getting md5 of Eradication binary/exe; OK if doing pymod run." )
        self.sim_dir_sem = threading.Semaphore()
        self.emodules_map["interventions"] = []
        self.emodules_map["disease_plugins"] = []
        self.emodules_map["reporter_plugins"] = []
        self.src_dest_set = set()
        self.dll_name_to_path = {}
        if params.dll_root is not None and params.use_dlls is True:
            self.copyEModulesOver(params)
        else:
            print("Not using DLLs")

        return

    def update_file(self, source, destination):

        succeeded = False
        if os.path.exists(source):
            src_dest_pair = (source, destination)
            if src_dest_pair not in self.src_dest_set:
                ru.copy(source, destination)
                self.src_dest_set.add(src_dest_pair)
            succeeded = True
        else:
            print("Could not find source file '{0}' to copy to '{1}'".format(source, destination))

        return succeeded

    def copy_serialized_population_files(self, config_json, simulation_directory, scenario_directory):
        input_files = config_json['parameters'].get('Serialized_Population_Filenames', [])
        ref_files   = config_json['parameters'].get('Reference_SQL_Files', [])
        all_files   = input_files+ref_files

        serialized_pop_filenames = []

        for filename in all_files:
            if not filename or len(filename.strip(' ')) == 0:
                continue

            scenario_file = os.path.join(scenario_directory, filename)

            # Copy directly to remote simulation working directory
            if os.path.isfile(scenario_file):
                simulation_path = os.path.join(self.params.local_sim_root, simulation_directory)
                simulation_file = os.path.join(simulation_path, os.path.basename(filename))
                self.update_file(scenario_file, simulation_file)
                if(filename in input_files):
                    serialized_pop_filenames.append(os.path.basename(filename))

        if( len(serialized_pop_filenames) > 0 ):
            config_json["parameters"]["Serialized_Population_Filenames"] = serialized_pop_filenames

        return

    def copy_demographics_files_to_user_input(self, simulation_directory, config_json, scenario_directory, source_input_directory):

        input_files = config_json['parameters'].get('Demographics_Filenames', [])
        if not input_files:
            input_files = config_json['parameters'].get('Demographics_Filename', '').split(';')

        demographics_filenames = []
        config_json["parameters"]["Demographics_Filenames"] = []
        missing_files = []

        for filename in input_files:
            if not filename or len(filename.strip(' ')) == 0:
                continue

            scenario_file = os.path.join(scenario_directory, filename)
            source_path = os.path.join(source_input_directory, filename)

            # For any demographics overlays WITHIN regression folder:
            # Copy directly to remote simulation working directory
            if scenario_file and os.path.isfile(scenario_file):
                # print('Copying %s to remote working directory'%filename)
                simulation_path = os.path.join(self.params.local_sim_root, simulation_directory)
                simulation_file = os.path.join(simulation_path, os.path.basename(filename))
                self.update_file(scenario_file, simulation_file)
            else:
                print("Could not find source file '{0}' locally ({1}) or in inputs ({2}) [{3}]!".format(filename, scenario_file, source_path, scenario_directory))
                missing_files.append(os.path.basename(filename))

            demographics_filenames.append(os.path.basename(filename))

        config_json["parameters"]["Demographics_Filenames"] = demographics_filenames

        if "Demographics_Filename" in config_json["parameters"]:
            del(config_json["parameters"]["Demographics_Filename"])

        if len(missing_files) > 0:
            config_json["parameters"][".Missing_Demographics_Files"] = missing_files

        return

    def copy_climate_and_migration_files_to_user_input(self, simulation_directory, config_json, source_input_directory, scenario):

        filter_list = ['Demographics_Filename',
                       'Demographics_Filenames',
                       'Campaign_Filename',
                       'Custom_Reports_Filename',
                       'Serialized_Population_Filenames',
                       '.Serialized_Population_Filenames']

        # Copy climate and migration files also
        for key in config_json["parameters"]:
            if ("_Filename" in key) and (key not in filter_list):
                filename = config_json["parameters"][key]
                if len(filename) == 0:
                    continue

                scenario_file = os.path.join(scenario, filename)
                source = os.path.join(source_input_directory, filename)

                # For any demographics overlays WITHIN regression folder:
                # Copy directly to remote simulation working directory
                if os.path.isfile(scenario_file):
                    simulation_path = os.path.join(self.params.local_sim_root, simulation_directory)
                    simulation_file = os.path.join(simulation_path, os.path.basename(filename))
                    source = scenario_file
                    dest = simulation_file
                    if not self.update_file(source, dest):
                        print("Could not find input file '{0}' to copy to '{1}' for scenario '{2}'".format(source, dest, scenario))
                    config_json["parameters"][key] = os.path.basename(filename)

                    # Copy secondary file (Climate and Migration only)
                    if key != "Load_Balance_Filename":
                        source = source + ".json"
                        dest = dest + ".json"
                        if not self.update_file(source, dest):
                            print("Could not find input file '{0}' to copy to '{1}' for scenario '{2}'".format(source, dest, scenario))
        return

    def copy_pymod_files( self, config_json, simulation_directory, scenario_path ):
        if "emodularization" not in scenario_path:
            return

        # Copy *_template.json and *_test.py from scenario_path to simulation_directory.
        # And copy ../*.pyd files
        sim_dir = os.path.join(self.params.local_sim_root, simulation_directory)

        # just search for all pyd files by walking the tree and copy them to each sim folder for now
        pyds = []
        suffix = ".so" if os.name == "posix" else ".pyd"
        #pyds = glob.glob( ( "../emodularization/**/*." + suffix ), recursive=True)
        for root, dirs, files in os.walk("../emodularization/"):
            for file in files:
                if file.endswith(suffix):
                    pyds.append((os.path.join(root, file))) 
        print( "Found python shared objects to copy: " + str( pyds ) )
        for pyd in pyds:
            print( "Copying " + pyd + " to" + os.path.join( sim_dir, os.path.basename( pyd ) ) )
            ru.copy( pyd, os.path.join( sim_dir, os.path.basename( pyd ) ) )
       
        # Yes, I can combine the below 3 blocks by having list pairs of root-dir and regex but I 
        # want the last two to go away.
        regexes = [ "*_template.json", "demographics_*.json", "*.py" ]
        # copy certain files (nice if we can be more specific)
        for pattern in regexes:
            foundfiles = glob.glob(os.path.join( scenario_path, pattern ))
            for myfile in foundfiles:
                ru.copy( myfile, os.path.join( sim_dir, os.path.basename( myfile ) ) )
       

        # WANT TO GET RID OF THIS: Some multi-test situations have common python scripts in the parent folder
        # but regular tests (non-sub-foldered) could have who-knows-what in their parent dir!
        for py in glob.glob( os.path.join( os.path.join( scenario_path, ".." ), "*.py" )):
            ru.copy( py, os.path.join( sim_dir, os.path.basename( py ) ) )

        return

    def copy_input_files_to_user_input(self, simulation_directory, scenario_path, config_json):
        # Copy local demographics/input file(s) 
        source_input_directory = "."

        # Harmonizing these to do the same thing
        self.copy_demographics_files_to_user_input(simulation_directory, config_json, scenario_path, source_input_directory) 
        self.copy_climate_and_migration_files_to_user_input(simulation_directory, config_json, source_input_directory, scenario_path) 
        self.copy_serialized_population_files(config_json,simulation_directory, scenario_path)
        self.copy_pymod_files(config_json, simulation_directory, scenario_path)

        return

    def crunch_name(self, arg_string):
        # Provides name mapping for the custom reporters; the report name
        # and the file name will be the same after the mapping below.

        arg_string = arg_string.upper()

        arg_string = arg_string.replace('_',             '')
        arg_string = arg_string.replace('LIB',           '')
        arg_string = arg_string.replace('CUSTOMREPORT',  '')

        return arg_string

    def transform_path( self, win_path ):
        return win_path.replace( "\\", "/" ).replace( "bayesianfil01", "mnt" ).replace( "IDM", "idm" ).replace( "//", "/" )

    # Copy just build dlls to deployed places based on commandline argument
    # - The default is to use all of the DLLs found in the location the DLL projects
    #   place the DLLs (<trunk>\x64\Release).
    # - --dll-path allows the user to override this default path
    def copyEModulesOver(self, params):

        print( "src_root = " + params.src_root )

        if params.scons:
            emodule_dir = os.path.join(params.src_root, "build")
            emodule_dir = os.path.join(emodule_dir, "x64")
        else:
            emodule_dir = os.path.join(params.src_root, "x64")
        emodule_dir = os.path.join(emodule_dir, "Release")

        print('Assuming emodules (dlls) are in local directory: ' + emodule_dir)

        if not os.path.exists(emodule_dir):
            print("Except that directory does not exist!  Not copying emodules.")
            return

        dll_dirs = ["disease_plugins",  "reporter_plugins", "interventions"]

        for dll_subdir in dll_dirs:
            suffix = "*.dll" if os.name == "nt" else "*.so"
            dlls = glob.glob(os.path.join( os.path.join(emodule_dir, dll_subdir), suffix )) 
            for dll in dlls:
                print( "Considering dll: " + dll )
                dll_hash = ru.md5_hash_of_file(dll)
                # print( dll_hash )
                # 1) calc md5 of dll
                # 2) check for existence of rivendell (or whatever) for <root>/emodules/<subdir>/<md5>
                # 3) if no exist, create and copy
                # 4) put full path in emodules_json
                # 5) write out emodules_json when done to target sim dir
                try:
                    target_dir = os.path.abspath(os.path.join(params.dll_root, dll_subdir))
                    target_dir = os.path.abspath(os.path.join(target_dir, dll_hash))

                    if(os.path.isdir(target_dir)):
                        if(os.listdir(target_dir)):
                            print(dll + ": Already on cluster")
                        else:
                            os.rmdir(target_dir)

                    if not os.path.isdir(target_dir):
                        print(dll + ": copying to cluster")
                        os.makedirs(target_dir)
                        ru.copy(dll, os.path.join(target_dir, os.path.basename(dll)))

                    dll_path = os.path.join(target_dir, os.path.basename(dll))
                    self.emodules_map[dll_subdir].append(dll_path)

                    try:
                        name = self.crunch_name(os.path.basename(dll_path).split('.')[0])
                        self.dll_name_to_path[name] = dll_path
                    except Exception as ex:
                        print( str( ex ) )

                except IOError as ioex:
                    print( "Failed to copy dll " + dll + " to " + os.path.join(os.path.join(params.dll_root, dll_dirs[1]), os.path.basename(dll)) )
                    print( "Exception " + str( ioex ) )
                    ru.final_warnings += "Failed to copy dll " + dll + " to " + os.path.join(os.path.join(params.dll_root, dll_dirs[1]), os.path.basename(dll)) + "\n"

        return

    def copy_sim_file(self, config_id, sim_dir, filename):
        if len(filename) != 0:
            filename = os.path.join(config_id, filename)
            if os.path.isfile(filename):
                # print( "Copying " + filename )
                ru.copy(filename, sim_dir, True)
            else:
                print("ERROR: Failed to find file to copy: " + filename)
        return

    def filter_emodules(self, custom_reports):
        final_reporters = []

        # Extract the name of all reporters used in the custom_reports file
        reports_json = json.loads(custom_reports)

        # Return empty reporters if the file isn't formatted right or no reporters are provided
        if reports_json.get("Custom_Reports") is None:
            return final_reporters

        reporters = list(reports_json["Custom_Reports"])
        reporters_set = set(reporters)

        # Re-build the list of reporter DLL paths in the emodules map based on the reporters appearing above
        for reporter in reporters_set:
            try:
                final_reporters.append(self.dll_name_to_path[self.crunch_name(reporter)])
            except KeyError:
                print("Warning: when building reporter_plugins emodule list, no path found for '{0}'. "
                      "Continuing.".format(reporter))
        return final_reporters

    def commissionFromConfigJson(self, sim_id, reply_json, scenario_path, report, scenario_type='tests'):
        # scenario_type == 'tests' will compare results to reference
        # scenario_type != 'tests', e.g. 'science' or 'sweep' will skip comparison

        print("Commissioning locally (not on cluster)!")
        sim_dir = os.path.abspath(os.path.join(self.params.local_sim_root, sim_id))
        bin_dir = os.path.abspath(os.path.join(self.params.local_bin_root, self.dtk_hash)) if self.dtk_hash else None

        # create unique simulation directory
        self.sim_dir_sem.acquire()
        os.makedirs(sim_dir)
        self.sim_dir_sem.release()

        # only copy binary if new to us; copy to bin/<md5>/Eradication.exe and run from there
        # Would like to create a symlink and run from the sim dir, but can't do that on cluster; no permissions!

        # JPS - can't we just check for existence of that file?  This seems overly complicated...

        # check in bin_dir to see if our binary exists there...
        foundit = False
        bin_path = None 
        if scenario_type == "pymod":
            bin_path = "python " # py script needs to come from folder not hardcoded
            if os.name == "posix":
                # This is not the correct solution but I don't know yet how to make sure we use python3 where it's present or just python
                # Test on windows bamboo
                bin_path = "python3 "
            script_name = os.path.basename( scenario_path.strip('/') ) + "_test.py"
            bin_path += script_name
            foundit = True
        else:
            bin_path = os.path.join(bin_dir, "Eradication" if os.name == "posix" else "Eradication.exe")
        if bin_dir and os.path.exists(bin_dir):
            if os.path.exists(bin_path):
                foundit = True
        elif bin_dir:
            os.makedirs(bin_dir)
        
        if not foundit:
            print("We didn't have it, copy it up...")
            ru.copy(self.params.executable_path, bin_path)
            print("Copied!")

        reply_json["bin_path"] = bin_path
        reply_json["executable_hash"] = self.dtk_hash

        # JPS - not sure what some of the rest of this stuff does
        # campaign_json is non, and config.json contains double the stuff it needs in the sim dir... :-(
        
        # tease out campaign json, save separately
        if "campaign_json" in reply_json:
            #campaign_json = json.dumps(reply_json["campaign_json"]).replace("u'", "'").replace("'", '"').strip('"')
            with open( sim_dir + "/" + self.campaign_filename, 'w' ) as camp_file:
                 try:
                    camp_json = reply_json["campaign_json"]
                    if isinstance( camp_json, dict ):
                        json.dump(camp_json, camp_file)
                    else:
                        camp_json_grrr = json.loads( camp_json )
                        json.dump(camp_json_grrr , camp_file)
                 except Exception as ex:
                     print( "Exception writing campaign.json for " + scenario_path )
                     print( str( ex ) ) 
            reply_json["campaign_json"] = None 
        elif "Campaign_Filename" in reply_json["parameters"]:
            camp_filename = reply_json["parameters"]["Campaign_Filename"]
            if len(camp_filename) > 0:
                try:
                    shutil.copy( os.path.join( scenario_path, camp_filename ), os.path.join( sim_dir, camp_filename ) )
                except Exception as ex:
                    print( "Exception copying campaign file to sim dir: " + str(ex) )


        # tease out custom_reports json, save separately
        if "custom_reports_json" in reply_json and reply_json["custom_reports_json"] is not None:
            reports_json = json.dumps(reply_json["custom_reports_json"]).replace("u'", "'").replace("'", '"').strip('"')
            reply_json["custom_reports_json"] = None
            # save custom_reports.json
            with open(sim_dir + "/custom_reports.json", 'w') as f:
                # f.write( json.dumps( reports_json, sort_keys=True, indent=4 ) )
                f.write(str(reports_json))

        # Use a local variable here because we don't want the PSP in the config.json that gets written out to disk
        # but we need it passed through to the monitor thread execution in the reply_json/config_json.
        py_input = None
        if "parameters" in reply_json and "Python_Script_Path" in reply_json["parameters"]:
            py_input = "."
            for py_file in glob.glob(os.path.join(scenario_path, "dtk_*.py")):
                self.copy_sim_file(scenario_path, sim_dir, os.path.basename(py_file))

            source_dir = os.path.join("shared_embedded_py_scripts", "dtk_test")
            destin_dir = os.path.join(sim_dir, "dtk_test")
            shutil.copytree(source_dir, destin_dir)

            source_dir = os.path.join("shared_embedded_py_scripts", "dtk_ep4")
            destin_dir = os.path.join(sim_dir, "dtk_ep4")
            shutil.copytree(source_dir, destin_dir)

        self.copy_input_files_to_user_input(sim_id, scenario_path, reply_json)

        # save config.json
        with open(sim_dir + "/config.json", 'w') as f:
            f.write(json.dumps(reply_json, sort_keys=True, indent=4))

        # now that config.json is written out, add Py Script Path back (if non-empty)
        if py_input is not None:
            reply_json["PSP"] = self.params.py_path
            del(reply_json["parameters"]["Python_Script_Path"])

        try:
            self.emodules_map["reporter_plugins"] = self.filter_emodules(reports_json)
        except UnboundLocalError:
            # reports_json is undefined, so use no reporters
            self.emodules_map["reporter_plugins"] = []
        with open(sim_dir + "/emodules_map.json", 'w') as f:
            f.write(json.dumps(self.emodules_map, sort_keys=True, indent=4))

        # ------------------------------------------------------------------
        # If you uncomment the following line, it will copy the program database
        # file to the directory where a simulation will run (i.e. with the config.json file).
        # This will help you get a stack trace with files and line numbers.
        # ------------------------------------------------------------------
        # print( "Copying PDB file...." )
        # ru.copy( "../Eradication/x64/Release/Eradication.pdb", sim_dir )
        # ------------------------------------------------------------------

        if os.path.isfile(os.path.join(scenario_path, "dtk_pre_process.py")):
            self.copy_sim_file(scenario_path, sim_dir, "dtk_pre_process.py")
        if os.path.isfile(os.path.join(scenario_path, "dtk_post_process.py")):
            self.copy_sim_file(scenario_path, sim_dir, "dtk_post_process.py")

        monitorThread = None    # need scoped here

        # print "Creating run & monitor thread."
        monitorThread = regression_local_monitor.Monitor(sim_id, scenario_path, report, self.params, reply_json, scenario_type)

        monitorThread.daemon = False
        monitorThread.start()

        return monitorThread

    def doSchemaTest(self):
        # print( "Testing schema generation..." )
        test_schema_path = "test-schema.json"
        subprocess.call([self.params.executable_path, "--get-schema", "--schema-path", test_schema_path], stdout=open(os.devnull))
        try:
            ru.load_json(test_schema_path)
            print("schema works.")
            os.remove(test_schema_path)
            return "passed"
        except Exception as ex:
            print(str(ex))

        return "failed"     # Well, it doesn't seem to have passed...
