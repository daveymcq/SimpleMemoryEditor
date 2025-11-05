#!/usr/bin/env python3

import os
import sys
import platform
import subprocess

def list_vs_installations():

    vs_installations = get_vs_installations()

    if len(vs_installations) > 0:

        print('\r')

        for index, vs in enumerate(vs_installations):

            print("{}: {}".format(index + 1, vs))
    else:

        print('\r\nVisual Studio is not installed on this machine.\n')

def get_vs_installations():

    vs_installations_vcvarsall = []
    vs_installations = subprocess.getoutput('call build\\vs\\detect.bat').split(',')

    for vs in vs_installations:

        for root, dirs, files in os.walk(vs):

            for file in files:

                if str(file) == 'vcvarsall.bat':

                    vcvarsall = (root + '\\' + str(file))
                    vs_installations_vcvarsall.append(vcvarsall)
        

    return vs_installations_vcvarsall

def vs_compile(vcvarsall = None):

    if platform.system() == 'Windows':

        if vcvarsall == None:

            vs_installations = get_vs_installations()

            if len(vs_installations) == 0:

                print('\r\nError: Visual Studio is not installed on this machine. Please install Visual Studio from https://visualstudio.microsoft.com\n')
                return False

            
            for vs in vs_installations:

                cmd = "call build\\vs\\build.bat \"{}\"".format(vs)
                print("\r\nUsing \"{}\" \r\n".format(vs))

                if subprocess.getoutput(cmd) != 'ERROR':

                    print(subprocess.getoutput(cmd))
                    return True
                            

        else:

            if os.path.isfile(vcvarsall):
                
                cmd = "call build\\vs\\build.bat \"{}\"".format(vcvarsall)
                print("\r\nUsing \"{}\" \r\n".format(vcvarsall))

                if subprocess.getoutput(cmd) != 'ERROR':

                    print(subprocess.getoutput(cmd))
                    return True

            else:

                print('\r\nError: The path to your Visual Studio installation\'s "vcvarsall.bat" is invalid.\n')
                return False
    else:

        print('\r\nError: You must be running Windows to compile using Visual Studio.\n')
        return False

