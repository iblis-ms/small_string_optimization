#!/usr/bin/python3 

# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system

import argparse
import os
import shutil
from builtins import staticmethod
import urllib.request
from enum import IntEnum, unique, Enum
import logging

from .googleTest import GoogleTest
from .sanitizers import Sanitizer
from .utils import Utils
from .gcovCoverage import GCovCoverage
from .benchmark import Benchmark

@unique
class CppStandard(IntEnum):
    CPP_98 = 98
    CPP_11 = 11
    CPP_14 = 14
    CPP_17 = 17
    CPP_20 = 20

@unique
class LogLevel(Enum):
    ERROR = "ERROR"
    WARNING = "WARNING"
    NOTICE = "NOTICE"
    STATUS = "STATUS"
    VERBOSE = "VERBOSE"
    DEBUG = "DEBUG"
    TRACE = "TRACE"
    
class BuildSystem:
    """
    Class that helps running CMake.
    """

        
    def __init__(self):
        self.logger = logging.getLogger("BuildSystem")
        consoleLoggerhandler = logging.StreamHandler()
        consoleLoggerhandler.setLevel(logging.INFO)
        consoleFormatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        consoleLoggerhandler.setFormatter(consoleFormatter)
        self.logger.addHandler(consoleLoggerhandler)
        self.logger.setLevel(logging.INFO)
        
        self.gtest = GoogleTest()
        self.sanitizer = Sanitizer()
        self.gconv = GCovCoverage()
        self.benchmark = Benchmark()
        
    @staticmethod
    def __downloadData():
        res = True
        script_dir_path = os.path.dirname(os.path.realpath(__file__))
        add_target_URL = 'https://raw.githubusercontent.com/iblis-ms/cmake_add_target/master/AddTarget.cmake'
        add_target_path = os.path.join(script_dir_path, 'addTarget.cmake')
        if not os.path.isfile(add_target_path):
            res = Utils.downloadScript(add_target_URL, add_target_path)

        return res
    

    def get_argument_parser_items(self, description):
        """Get argParse object with generic params.
        
        Parameters
        ----------
        description : string 
            Description.
            
        Returns
        -------
        ArgumentParser
            Client side can append arguments
        """
        
        current_dir = os.getcwd()
        arg_parser = argparse.ArgumentParser(description=description)
        
        arg_parser.add_argument(
            '-p', 
            '--profile', 
            choices=['Release', 'Debug'], 
            default='Debug',
            help='Build type.')
        arg_parser.add_argument(
            '-c',
            '--clean',
            action='store_true',
            help='Clean build - remove output directory before generation.'
            )
        arg_parser.add_argument(
            '-g',
            '--generator',
            help='CMake generator.'
            )
        arg_parser.add_argument(
            '-go',
            '--generate_only',
            help='Generate only.',
            action="store_true"
            )
        arg_parser.add_argument(
            '-c_c',
            '--c_compiler',
            help='C compiler.'
            )
        arg_parser.add_argument(
            '-cxx_c',
            '--cxx_compiler',
            help='CXX compiler.'
            )
        arg_parser.add_argument(
            '-t',
            '--target',
            help='Target to build.'
            )
        arg_parser.add_argument(
            '-cm_def',
            '--cmake_definitions',
            help='Definitions to CMake.',
            action="extend",
            nargs="+", 
            type=str
            )
        arg_parser.add_argument(
            '-c_def',
            '--c_definitions',
            help='Definitions to C/C++.',
            action="extend",
            nargs="+", 
            type=str
            )
        arg_parser.add_argument(
            '-vc_a',
            '--vc_architecture',
            help='Visual Studio generator architecture: .',
            )


        cpp_std_choice = []
        for std in CppStandard:
            cpp_std_choice.append(std.value)

        arg_parser.add_argument(
            '-cpp_std',
            '--cpp_standard',
            choices = cpp_std_choice,
            help='C++ standard',
            type=int,
            default=CppStandard.CPP_17.value
            )

        arg_parser.add_argument(
            '-tests',
            '--run_tests',
            help='Run tests.',
            action="store_true"
            )
        arg_parser.add_argument(
            '-tests_only',
            '--run_tests_only',
            help='Run test only.',
            action="store_true"
            )
        arg_parser.add_argument(
            '-test_inc',
            '--test_include',
            help='Include regex for test target.'
            )
        arg_parser.add_argument(
            '-test_exc',
            '--test_exclude',
            help='Exclude regex for test target.'
            )
        arg_parser.add_argument(
            '-bench_only',
            '--run_benchmarks_only',
            help='Run benchmarks only.',
            action="store_true"
            )
        arg_parser.add_argument(
            '-bench_inc',
            '--benchmark_include',
            help='Include regex for benchmark target.'
            )
        arg_parser.add_argument(
            '-bench_exc',
            '--benchmark_exclude',
            help='Exclude regex for benchmark target.'
            )
        arg_parser.add_argument(
            '-log_out',
            '--log_output_file',
            help='Log output file'
            )
            
        arg_parser.add_argument(
            '-o',
            '--output',
            default = os.path.join(current_dir, 'output'),
            help='Output folder'
            )
            
        arg_parser.add_argument(
            '-i',
            '--input',
            default = os.path.join(current_dir, 'code'),
            help='Input folder'
            )
           
        log_level = []
        for lvl in LogLevel:
            log_level.append(lvl.value)
            
        arg_parser.add_argument(
            '-cm_log',
            '--cmake_log_level',
            choices = log_level,
            help='CMake Log level',
            default=LogLevel.STATUS.value
            )
            
        self.gtest.appendArgParse(arg_parser)
        self.sanitizer.appendArgParse(arg_parser)
        self.gconv.appendArgParse(arg_parser)
        self.benchmark.appendArgParse(arg_parser)

        return arg_parser
        
    def setup(self, arg_parser):
        self.args = arg_parser.parse_args()
        if self.args.log_output_file is not None:
            fileLoggerhandler = logging.FileHandler(self.args.log_output_file)
            fileLoggerhandler.setLevel(logging.INFO)
            fileFormatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
            fileLoggerhandler.setFormatter(fileFormatter)
            self.logger.addHandler(fileLoggerhandler)
            
        return self.args
        
    def generate(self, input_path, output_path, pre_generate_fun = None):
        """Calls CMake to generate build script
        
        Parameters
        ----------
        self.args : argparse.Namespace 
            Parsed arguments.
        input_path : string 
            Path to folder where top CMakeLists.txt is located.
        output_path : string 
            Path to output folder.
        pre_generate_fun  
            Function called just before generation.
            
        Returns
        -------
        int
            Result code of run CMake.
        """
        self.__downloadData()

        cmd = ['cmake']

        if self.args.generator:
            cmd.extend(['-G', self.args.generator])
            
        if self.args.vc_architecture is None:
            self.args.vc_architecture = 'x64'

        if (self.args.generator is None and Utils.sysOp().windows) or (self.args.generator is not None and "Visual" in self.args.generator):
            cmd.extend(['-A', self.args.vc_architecture])
            
        cmd.append('-DCMAKE_BUILD_TYPE=' + self.args.profile)
        cmd.append('-DCMAKE_CXX_STANDARD=' + str(self.args.cpp_standard))

        if self.args.c_definitions:
            c_def = ';'.join(self.args.c_definitions)
            cmd.append('-DGLOBAL_COMPILE_DEFINES=' + c_def)
            
        if self.args.c_compiler:
            cmd.append('-DCMAKE_C_COMPILER=' + str(self.args.c_compiler))
        if self.args.cxx_compiler:
            cmd.append('-DCMAKE_CXX_COMPILER=' + str(self.args.cxx_compiler))
            
        if self.args.cmake_definitions is not None:
            for cmake_def in self.args.cmake_definitions:
                cmd.append('-D' + cmake_def)
        if self.args.test_include is not None:
            cmd.append('-DADD_TARGET_TEST_TARGET_INCLUDE=' + self.args.test_include)
        if self.args.test_exclude is not None:
            cmd.append('-DADD_TARGET_TEST_TARGET_EXCLUDE=' + self.args.test_exclude)
        if self.args.benchmark_include is not None:
            cmd.append('-DADD_TARGET_BENCHMARK_TARGET_INCLUDE=' + self.args.benchmark_include)
        if self.args.benchmark_exclude is not None:
            cmd.append('-DADD_TARGET_BENCHMARK_TARGET_EXCLUDE=' + self.args.benchmark_exclude)
            
        cmd.append('--log-level=' + str(self.args.cmake_log_level))
        cmd.append('-DLOG_LEVEL=' + str(self.args.cmake_log_level))

        if self.args.cmake_log_level == LogLevel.TRACE.name:
            cmd.append('--log-context')
            cmd.append('--debug-output')
            cmd.append('--trace-expand')
        
        if self.args.clean:
            shutil.rmtree(output_path, ignore_errors=True)
            
        if not os.path.isdir(output_path):
            os.makedirs(output_path)
            
        gtest_args = self.gtest.getCmakeDefines(self.args)
        cmd.extend(gtest_args)
        sanitizer_args = self.sanitizer.getCmakeDefines(self.args)
        cmd.extend(sanitizer_args)
        gconv_args = self.gconv.getCmakeDefines(self.args)
        cmd.extend(gconv_args)
        benchmark_args = self.benchmark.getCmakeDefines(self.args)
        cmd.extend(benchmark_args)
        
        if pre_generate_fun is not None:
            if not pre_generate_fun(self.args, input_path, output_path, cmd):
                return False
        
        cmd.append(input_path)
        
        return_code = Utils.run(cmd, output_path)
        
        return return_code

    def build(self, build_path):
        """Calls CMake to generate build script
        
        Parameters
        ----------
        self.args : argparse.Namespace 
            Parsed arguments.
        build_path : string
            Output path
            
        Returns
        -------
        int
            Result code of run CMake.
        """
        
        cmd = ['cmake', '--build', '.']
        
        if self.args.target is not None:
            cmd.extend(['--target', self.args.target])
            
        cmd.extend(['--config', self.args.profile])
        return_code = Utils.run(cmd, build_path)
        
        return return_code

    def runTests(self, output_path):
        cmd = ['ctest', '--verbose']
        cmd.extend(['-C', self.args.profile])
        output_file_path = os.path.join(self.args.output, "testsOutput.txt")
        return_code = Utils.run(cmd, output_path, output_file_path = output_file_path)
        if return_code == 0:
            self.gconv.afterRunTests(self.args)
        return return_code


    def run(self, input_path = None, output_path = None):
        if input_path is None:
            input_path = self.args.input
        if output_path is None:
            output_path = self.args.output
        if not self.args.run_tests_only and not self.args.run_benchmarks_only:
            generate_result = self.generate(input_path, output_path)
            if generate_result != 0:
                return False
            if self.args.generate_only == True:
                return True
    
            build_result = self.build(output_path)
            if build_result != 0:
                return False

        if self.args.run_tests == True or self.args.run_tests_only == True:
            test_result = self.runTests(output_path)
            if test_result != 0:
                return False
        
        if (self.args.run_tests_only and self.args.run_benchmarks_only) or not self.args.run_tests_only: 
            self.benchmark.afterRunTests(self.args)

        return True

    def simpleRun(self, app_name):
        
        arg_parser = self.get_argument_parser_items(app_name)
        self.setup(arg_parser)
        return self.run()
