#!/bin/sh

check_ok() {
  if [ $? != 0 ]; then
    echo "failed."
    echo ""
    echo "================================================================================"
    if [ $2 = 1 ]; then
      echo "FATAL: $1 failed."
    else
      echo "$1 failed."
    fi
    echo "Please check install.log and fix any problems. If you're still stuck,"
    #echo "then please open a new issue then post all the output and as many details as you can to"
    #echo "  https://github.com/WiringPi/WiringPi-Node/issues"
    echo "================================================================================"
    echo ""
    if [ $2 = 1 ]; then
      exit 1
    fi
  fi
}


check_make_ok() {
  if [ $? != 0 ]; then
    echo "failed."
    echo ""
    echo "================================================================================"
    if [ $2 == 1 ]; then
      echo "FATAL: Making $1 failed."
    else
      echo "Making $1 failed."
    fi
    echo "Please check install.log and fix any problems. If you're still stuck,"
    #echo "then please open a new issue then post all the output and as many details as you can to"
    #echo "  https://github.com/WiringPi/WiringPi-Node/issues"
    echo "================================================================================"
    echo ""
    if [ $2 == 1 ]; then
      exit 1
    fi
  fi
}


make_libWiringPi() {
    echo -n "Making libWiringPi ... "
    cd ./wiringPi/wiringPi/
    make clean >> ../../install.log 2>&1
    make static >> ../../install.log 2>&1
    check_make_ok "libWiringPi" 1
    cd ../../
    echo "done."
}

make_gpio() {
    cd ./wiringPi/gpio/
    echo -n "Unistalling gpio utility ... "
    sudo make uninstall >> ../../install.log 2>&1
    echo "done."

    echo -n "Making gpio utility ... "
    make clean >> ../../install.log 2>&1
    make >> ../../install.log 2>&1
    check_make_ok "gpio utility" 0
    echo "done."

    echo -n "Installing gpio utility ... "
    sudo make install >> ../../install.log 2>&1
    check_make_ok "gpio utility" 0
    cd ../../
    echo "done."
}

make_wiringpi_sx() {
    echo -n "Making wiringpi-sx ... "
    node-gyp rebuild 2>&1 | tee -a ./install.log
    check_make_ok "wiringpi-sx" 1
    echo "done."
}

rm ./install.log 2>/dev/null 1>&2
make_libWiringPi
make_gpio
gpio readall
check_ok "gpio readall" 1
make_wiringpi_sx

