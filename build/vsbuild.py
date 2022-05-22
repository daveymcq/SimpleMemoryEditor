#!/usr/bin/env python3

import os
import sys
import platform
import subprocess

def list_vs_installations():

    for index, vs in enumerate(get_vs_installations()):

        print(f'{index + 1}: "{vs}"')

def get_vs_installations():

    vs_installations_vcvarsall = []
    vs_installations = subprocess.getoutput('call build\\vs\\vsdetect.bat').split(',')

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

                print('\r\nError: Visual Studio is installed. Please install Visual Studio from https://visualstudio.microsoft.com \r\n')
                return False

            
            for vs in vs_installations:

                cmd = f'call build\\vs\\vsbuild.bat "{vs}"'
                print(f'\r\nUsing {vs} ...\r\n')

                if subprocess.getoutput(cmd) != 'ERROR':

                    print(subprocess.getoutput(cmd))
                    return True
                            

        else:

            cmd = f'call build\\vs\\vsbuild.bat "{vcvarsall}"'

            print(f'\r\nUsing {vcvarsall} ...\r\n')

            if subprocess.getoutput(cmd) != 'ERROR':

                print(subprocess.getoutput(cmd))
                return True

            else:

                print('\r\nError: The path to your Visual Studio installation\'s "vcvarsall.bat" is invalid. \r\n')
                return False
    else:

        print('\r\nError: You must be running Windows to compile using Visual Studio. \r\n')
        return False

