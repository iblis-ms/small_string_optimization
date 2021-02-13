#!/usr/bin/python3

# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system

import os

from .utils import Utils

class GCovCoverage:

    def appendArgParse(self, arg_parser):
        arg_parser.add_argument(
            '-gcov',
            '--gcov',
            action="store_true",
            help='GCov: enable'
            )
        arg_parser.add_argument(
            '-gcov_p_t',
            '--gcov_per_target',
            action="store_true",
            help='GCov: TODO: show code coverage per target not entire project'
            )
        arg_parser.add_argument(
            '-gcov_conf',
            '--gcov_config_file',
            help='GCov: Path to configuration file'
            )
        arg_parser.add_argument(
            '-gcov_o',
            '--gcov_output',
            help='GCov: output path'
            )
        
        return arg_parser

    def getCmakeDefines(self, args):
        cmake = []
        if not args.gcov:
            return cmake
        
        cmake.append('-DGCOV_ENABLE=1')
        if not args.gcov_config_file:
            args.gcov_config_file = str(os.path.join(args.output, "gcov_config.txt")).replace('\\', '/')
        cmake.append('-DGCOV_CONF_PATH=' + str(args.gcov_config_file).replace('\\', '/'))
            
        if not args.gcov_output:
            folders = os.path.join(args.output, "code_coverage")
            os.makedirs(folders, exist_ok=True)
            args.gcov_output = os.path.join(folders, "index.html")

        cmake.append('-DGCOV_OUTPUT_PATH=' + str(args.gcov_output).replace('\\', '/'))
        
        return cmake

    def afterRunTests(self, args):
        if not args.gcov:
            return 0
        cmd = ['gcovr', '--config', args.gcov_config_file]
        return_code = Utils.run(cmd, args.output)
        return return_code