#!/usr/bin/python3

# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system

import os
import subprocess
from builtins import staticmethod
import sys
import urllib.request
import logging
        
from  .sysOp import SysOp

class Utils:
    """
    Class that helps running CMake.
    """
    
    @staticmethod
    def sysOp():
        if not hasattr(Utils, 'sysop'):
            Utils.sysop = SysOp()
        return Utils.sysop
            
    @staticmethod
    def downloadScript(url, output_path):
        logger = logging.getLogger("BuildSystem")
        response = requests.get(url, allow_redirects=True)
        if response.status_code != 200:
            logger.error('Downloading ' + str(url) + ' to ' + str(output_path) + ' FAILED')
            return False

        logger.info('Downloading ' + str(url) + ' to ' + str(output_path))
        open(output_path, 'wb').write(response.content)
        return True
            
    @staticmethod
    def run(cmd, working_dir, env = None, collect_output = False, output_file_path = None):
        """Runs given command in given working directory
        
        Parameters
        ----------
        cmd : array
            Command to run. The program and each argument is item in the array, i. e. ['echo', '${PATH']
        working_dir : string
            orking directory - location from command will be run
            
        Returns
        -------
        int
            Error code from run command. 0 means finished successfully.
        """
        
        logger = logging.getLogger("BuildSystem")
        os.environ['PYTHONUNBUFFERED'] = "1" # to not buffer logs, but print immediately
        logger.info("####################################### <run> #######################################")
        logger.info("Working Directory: " + working_dir)
        logger.info("Command: " + ' '.join(cmd))
        shell = False
        if Utils.sysOp().windows:
            shell = True
        proc = subprocess.Popen(cmd,
                                stdout = subprocess.PIPE,
                                stderr=subprocess.STDOUT,
                                cwd = working_dir,
                                shell=shell,
                                env = env
                                )
        logger.info("-------------------------------------- <output> -------------------------------------")

        output_txt = ''
        output_file = None
        if output_file_path is not None:
            output_file = open(output_file_path, "w")
        while True:
            output = proc.stdout.readline()
            pol = proc.poll()
           
            if (output == '' or output == b'') and pol is not None:
                break
            if output:
                strLineWithNewLine = output.decode(errors='ignore')
                if collect_output:
                    output_txt = output_txt + strLineWithNewLine
                if output_file is not None:
                    output_file.write(strLineWithNewLine)
                line = strLineWithNewLine[:-1]
                logger.info(line)
                
        return_code = proc.poll()
        if output_file is not None:
            output_file.close()
        logger.info("-------------------------------------- </output> ------------------------------------")
        logger.info("Return code: " + str(return_code))
        logger.info("####################################### </run> ######################################")
        
        if collect_output:
            return return_code, output_txt
        else: 
            return return_code

