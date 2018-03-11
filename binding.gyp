{
    'targets': [
        {
            'target_name': 'wiring_pi_sx',
            'sources': [ 
                'src/addon.cc',
                'src/wpi.cc',
                'src/wiringPi.cc',
                'src/wiringPiSPI.cc',
                'src/wiringSerial.cc'
            ],
            'include_dirs': [
                'wiringPi/wiringPi'
            ],
           'libraries': [
              '<!(pwd)/wiringPi/wiringPi/libwiringPi.a'
            ],
            'cflags': [
                '-Wall'
            ],
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ]
        }
    ]
}

