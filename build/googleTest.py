# Author: Marcin Serwach
# License: MIT
# ULR: https://github.com/iblis-ms/python_cmake_build_system
 
class GoogleTest:

    def appendArgParse(self, arg_parser):
        arg_parser.add_argument(
            '--gtest_filter',
            help='GTest: Filter regex for running test cases.'
            )
        arg_parser.add_argument(
            '--gtest_color',
            choices = ['no','yes', 'auto'],
            default = 'yes',
            help='GTest: Use colorful logs.'
            )
        arg_parser.add_argument(
            '--gtest_also_run_disabled_tests',
            action="store_true",
            help='GTest: Run also disabled tests.'
            )
        arg_parser.add_argument(
            '--gtest_repeat',
            type=int,
            help='GTest: Indicate how many times tests shall be run.'
            )
        arg_parser.add_argument(
            '--gtest_brief',
            action="store_true",
            help='GTest: Prints only failures.'
            )
        arg_parser.add_argument(
            '--gtest_shuffle',
            action="store_true",
            help='GTest: Shuffle tests.'
            )
        arg_parser.add_argument(
            '--gtest_print_time',
            action="store_true",
            help='GTest: Do not print execution time'
            )
        arg_parser.add_argument(
            '--gtest_output',
            help='GTest: Path to output file with details.'
            )
        arg_parser.add_argument(
            '--gtest_list_tests',
            action="store_true",
            help='GTest: Print only tests case.'
            )
        arg_parser.add_argument(
            '--gtest_fail_fast',
            action="store_true",
            help='GTest: Stop after the first failure.'
            )
        arg_parser.add_argument(
            '--gtest_print_utf8',
            action="store_true",
            help='GTest: Print in utf-8.'
            )
        arg_parser.add_argument(
            '--gtest_random_seed',
            help='GTest: Seed for shuffle.'
            )
      
        return arg_parser
    
    def getCmakeDefines(self, args):
        cmake = []
        if args.gtest_filter:
            cmake.append('-DGTEST_FILTER=' + str(args.gtest_filter))
        if args.gtest_color:
            cmake.append('-DGTEST_COLOR=' + str(args.gtest_color))
        if args.gtest_also_run_disabled_tests:
            cmake.append('-DGTEST_RUN_DISABLED=' + str(int(args.gtest_also_run_disabled_tests)))
        if args.gtest_repeat:
            cmake.append('-DGTEST_REPEAT=' + str(args.gtest_repeat))
        if args.gtest_brief:
            cmake.append('-DGTEST_BRIEF=' + str(int(args.gtest_brief)))
        if args.gtest_shuffle:
            cmake.append('-DGTEST_SHUFFLE' + str(int(args.gtest_shuffle)))
        if args.gtest_print_time:
            cmake.append('-DGTEST_PRINT_TIME=' + str(int(args.gtest_print_time)))
        if args.gtest_output:
            cmake.append('-DGTEST_OUTPUT=' + str(args.gtest_output))
        if args.gtest_list_tests:
            cmake.append('-DGTEST_LIST_TESTS=' + str(int(args.gtest_list_tests)))
        if args.gtest_fail_fast:
            cmake.append('-DGTEST_FAIL_FAST=' + str(int(args.gtest_fail_fast)))
        if args.gtest_print_utf8:
            cmake.append('-DGTEST_PRINT_UTF8=' + str(int(args.gtest_print_utf8)))
        if args.gtest_random_seed:
            cmake.append('-DGTEST_PRINT_UTF8=' + str(args.gtest_random_seed))
            
        return cmake
        
