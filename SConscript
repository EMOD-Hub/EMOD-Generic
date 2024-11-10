# -*- mode: python; -*-
# This Python script, SConscript, invoked by the SConstruct in this directory,
#
# 1. delegates to other per-module SConscript files for executable and library 
# (static and/or dynamic)
import os
import sys
import shutil

Import('env')

def InstallEmodules(src, dst):
    
    print( "\nInstalling from " + src + " to " + dst + "..." )
    if os.path.isfile(dst):
        print( "Warning: " + dst + " is a file\n" )
        return;

    if os.path.exists(dst) != True:
        print( "Creating " + dst + " in " + os.getcwd() )
        os.mkdir(dst)

    srcfiles = os.path.join(src,'*.dll')
    for root, dirs, files in os.walk(src):
        for file in files:
            if file.endswith('.dll') or file.endswith('.exe'):
                full_fn = os.path.join(root,file);
                print( "copying: " + full_fn )
                shutil.copy2(full_fn, dst);
    
# if --install is on, just copy the dlls (assumed there already) and finish
dst_path = env['Install']
if dst_path != "":
    InstallEmodules(Dir('.').abspath, dst_path)
    #InstallEmodules(Dir('#').abspath, dst_path)
    print("Finished installing.\n")
    sys.exit(0)

# set the common libraries
env.Append( LIBPATH = [
              "$BUILD_DIR/baseReportLib",
              "$BUILD_DIR/campaign", 
              "$BUILD_DIR/cajun", 
              "$BUILD_DIR/libsqlite", 
              "$BUILD_DIR/snappy", 
              "$BUILD_DIR/lz4", 
              "$BUILD_DIR/utils"])

if os.name != "posix":
    env.Append( LIBS=[
              "baseReportLib", 
              "campaign",
              "cajun",
              "libsqlite",
              "snappy",
              "lz4",
              "utils"])

# First static libs
statlibscriptlist = [
              'baseReportLib/SConscript',
              'cajun/SConscript',
              'campaign/SConscript',
              'Eradication/SConscript_coreLib',
              'snappy/SConscript',
              'lz4/SConscript',
              'utils/SConscript' ]

if os.name != "posix":
    statlibscriptlist.append( "libsqlite/SConscript" )

SConscript( statlibscriptlist )

# Finally executable
SConscript('Eradication/SConscript')

def OptionalScript(sconscript_name):
    sconscript_path = os.path.join(Dir('#').abspath, sconscript_name)
    if os.path.isfile(sconscript_path):
        SConscript(sconscript_name)
    else:
        print("Skipping missing script: '{0}'".format(sconscript_path))

disease = "ALL"
if 'Disease' in env and len(env['Disease']) > 0:
    disease = env["Disease"]

if disease != "Typhoid":
    OptionalScript('reporters/SConscript_Generic_AgeAtInfection')
    OptionalScript('reporters/SConscript_Generic_AgeAtInfectionHistogram')
    OptionalScript('reporters/SConscript_Generic_Basic')
    OptionalScript('reporters/SConscript_Generic_EventCounter')
    OptionalScript('reporters/SConscript_Generic_HumanMigrationTracking')
    OptionalScript('reporters/SConscript_Generic_KmlDemo')
    OptionalScript('reporters/SConscript_Generic_NodeDemographics')
    OptionalScript('reporters/SConscript_Generic_Serosurvey')
    OptionalScript('reporters/SConscript_Generic_StrainTracking')

if( (disease == "ALL") or (disease == "HIV") ):
    pass

if( (disease == "ALL") or (disease == "Malaria") ):
    OptionalScript('reporters/SConscript_Malaria_Filtered')
    OptionalScript('reporters/SConscript_Malaria_Filtered_Spatial')
    OptionalScript('reporters/SConscript_Malaria_Immunity')
    OptionalScript('reporters/SConscript_Malaria_Patient')
    OptionalScript('reporters/SConscript_Malaria_Summary')
    OptionalScript('reporters/SConscript_Malaria_Survey')
    OptionalScript('reporters/SConscript_Malaria_Transmission')
    OptionalScript('reporters/SConscript_Malaria_NodeDemographicsMalaria')
    OptionalScript('reporters/SConscript_Malaria_VectorStatsMalaria')

if( (disease == "ALL") or (disease == "Polio") ):
    pass

if( (disease == "ALL") or (disease == "TBHIV") ):
    OptionalScript('reporters/SConscript_TBHIV_Basic')
    OptionalScript('reporters/SConscript_TBHIV_ByAge')

if( (disease == "ALL") or (disease == "STI") or (disease == "HIV") ):
    OptionalScript('reporters/SConscript_STI_RelationshipMigrationTracking')
    OptionalScript('reporters/SConscript_STI_RelationshipQueue')
    OptionalScript('reporters/SConscript_STI_RelationshipCensus')

if( (disease == "ALL") or (disease == "Vector") or (disease == "Malaria") ):
    OptionalScript('reporters/SConscript_Vector_VectorHabitat')
    OptionalScript('reporters/SConscript_Vector_VectorMigration')
    OptionalScript('reporters/SConscript_Vector_VectorStats')

if( disease == "ALL"):
    OptionalScript('UnitTest++/SConscript')
    OptionalScript('componentTests/SConscript')
