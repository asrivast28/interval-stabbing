##
# @file SConscript
# @brief Provides functionality for building the executable for stabbing intervals.
# @author Ankit Srivastava <asrivast@gatech.edu>
#
# Copyright 2018 Georgia Institute of Technology
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import platform

Import('env')

boostLibs = [
             'boost_filesystem',
             'boost_program_options',
             'boost_system',
             ]

if platform.system() == 'Linux':
  boostLibs = [ File('%s/lib%s.a'%(env.boostLibPath, lib)) for lib in boostLibs ]

apLibs = [
          'apexec',
          'apcompile',
          ]

cppLibs = boostLibs + apLibs 


srcFiles = [
            'LabelingAlgorithms.cpp',
            'Points.cpp',
            'Intervals.cpp',
            'ProgramOptions.cpp',
            'driver.cpp',
            ]

allLibs = env.get('LIBS', [])

env.Program(target = env.targetName, source = srcFiles, LIBS = allLibs + cppLibs)

env.Install(env.topDir, env.targetName)
