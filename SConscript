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
