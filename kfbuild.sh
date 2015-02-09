# User specific aliases and functions
XTENSA_CORE=KF1_prod_rel_2012_4
XTENSA_TOOLS_ROOT=/cad/tensilica/tools/RD-2012.4-linux/XtensaTools
XTENSA_ROOT=/cad/tensilica/chips/kingfisher/RD-2012.4-linux/$XTENSA_CORE

XTENSA_SYSTEM=$XTENSA_ROOT/config

LM_LICENSE_FILE=27002@192.168.106.113

LM_LICENSE_FILE=~/xtensa/license.dat
PATH=$PATH:$XTENSA_TOOLS_ROOT/bin

export LM_LICENSE_FILE XTENSA_TOOLS_ROOT XTENSA_ROOT XTENSA_SYSTEM XTENSA_CORE PATH

export XTENSA_PREFER_LICENSE=XT-GENERIC

export KF_SCS_DIR=`pwd`
export TARGET=QCA4004
export FLAVOR=SP143
export AR6002_REV=7
export AR6002_REV7_VER=2
export FPGA_FLAG=0
export THREADX_EN=1
export SSL_EN=0
export ALLJOYN_EN=0
export ZC_MODULE_VERSION=0
export ZC_MODULE_TYPE=4
#prepare for make flash image
make
cp image/* .output/QCA4004/SP143/image/
rm -rf output
mkdir output
cp .output/QCA4004/SP143/image/sdk_flash.out output/sdk_shell.out
cd flash
sh make_flash_host_less.sh
cd ../
