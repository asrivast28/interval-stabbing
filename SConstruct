##
# @file SConscript
# @brief Provides the environment for building the executable for stabbing intervals.
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

import os
import platform

topDir = os.getcwd()
cpp = None
buildDir = None
targetName = 'stab-intervals'


cppPaths = [
            topDir,
            ]

cppFlags = [
            ]

cppDefs = [
           ]

libPaths = [
            ]

# Flag for building in debug mode. Defaults to release build.
releaseBuild = ARGUMENTS.get('DEBUG', 0) in [0, '0']
# Location of boost static libraries.
boostLibPath = ARGUMENTS.get('BOOSTLIBPATH', '/usr/lib/x86_64-linux-gnu')
# SDK version
import subprocess
import re
sdkVersion = int(re.search('version 1.7.(\d+)', subprocess.check_output(['apadmin', '--version'])).group(1))
if platform.system() in ['Darwin', 'Linux']:
  cpp = 'g++'
  cppFlags.extend([
              '-Wall',
              '-std=c++0x',
              ])
  if releaseBuild:
      cppFlags.append('-O3')
      cppDefs.append('NDEBUG')
  else:
      cppFlags.append('-g')

cppDefs.append('APSDKVERSION=%d'%sdkVersion)

# For OS X
if platform.system() == 'Darwin':
  cppPaths.append('/opt/local/include')
  libPaths.append('/opt/local/lib')
elif platform.system() == 'Linux':
  cppPaths.append('/usr/include')


if releaseBuild:
    buildDir = 'release'
else:
    buildDir = 'debug'
    targetName += '_debug'

env = Environment(ENV = os.environ, CXX = cpp, CXXFLAGS = cppFlags, CPPPATH = cppPaths, CPPDEFINES = cppDefs, LIBPATH = libPaths)

env.targetName = targetName
env.topDir = topDir
env.boostLibPath = boostLibPath

buildDir = os.path.join('builds', buildDir)

apsdk = SConscript('apsdk/SConscript', exports = 'env', src_dir = 'apsdk', variant_dir = os.path.join(buildDir, 'apsdk'),  duplicate = 0)
env.Append(LIBS = [apsdk])
SConscript('SConscript', exports = 'env', src_dir = '.', variant_dir = buildDir, duplicate = 0)
