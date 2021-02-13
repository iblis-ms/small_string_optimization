#!/usr/bin/python3

# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system

import urllib.request

from .utils import Utils

class Sanitizer:

    def appendArgParse(self, arg_parser):
                    
        memory_sanitizer = []
        clang_posix_sanitizers = ['memory_sanitizer', 'address_sanitizer', 'thread_sanitizer', 'undefined_behavior_sanitizer']
        if Utils.sysOp().windows:
            memory_sanitizer.append('dr_memory')
        if Utils.sysOp().linux:
            memory_sanitizer.append('valgrind')
            memory_sanitizer.extend(clang_posix_sanitizers)
        if Utils.sysOp().macos:
            memory_sanitizer.append('valgrind')
            memory_sanitizer.extend(clang_posix_sanitizers)
                    
        arg_parser.add_argument(
            '-san',
            '--sanitizer',
            choices = memory_sanitizer,
            help='Memory sanitizer like: Dr Memory, Valgrind'
            )
        return arg_parser
    
    def getCmakeDefines(self, args):
        cmake = []
            
        if args.sanitizer is not None:
            if args.sanitizer == 'valgrind':
                cmake.append('-DADD_TARGET_VALGRIND=1')
            if args.sanitizer == 'dr_memory':
                cmake.append('-DADD_TARGET_DR_MEMORY=1')
            if args.sanitizer == 'memory_sanitizer':
                cmake.append('-DADD_TARGET_CLANG_MEMORY_SANITIZER=1')
            if args.sanitizer == 'address_sanitizer':
                cmake.append('-DADD_TARGET_CLANG_ADDRESS_SANITIZER=1')
            if args.sanitizer == 'thread_sanitizer':
                cmake.append('-DADD_TARGET_CLANG_THREAD_SANITIZER=1')
            if args.sanitizer == 'undefined_behavior_sanitizer':
                cmake.append('-DADD_TARGET_CLANG_UNDEFINED_BEHAVIOR_SANITIZER=1')
                
        return cmake
