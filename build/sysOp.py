#!/usr/bin/python3

# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system

import logging
import platform

class SysOp:
    def __init__(self):
        self.windows = False
        self.linux = False
        self.macos = False
        plt = platform.system()

        if plt == "Windows":
            self.windows = True
        elif plt == "Linux":
            self.linux = True
        elif plt == "Darwin":
            self.macos = True
        else:
            logging.error('Unidentified system')
