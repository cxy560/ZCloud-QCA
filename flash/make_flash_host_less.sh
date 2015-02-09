#!/bin/sh
AR6002_REV7_VER=2
FLASH_FLAVOR="_IOT"
BD_FILENAME="boardData_2_1_SP143.bin" 
DSET_PATCH_FILENAME="dset_DB142.bin"
MAKE_FLASH_APPS_ONLY=0
#APPS_FILENAME="../../.output/AR6002/hw7.2/bin/flashapps_iot.bin"
APPS_FILENAME="flashimg.bin"
INC_FILENAME="flashapps.bin"
APPS_OUTFILE="sdk_shell.out"
ATHWLAN_FLAVOR="_iot"
TUNE_PARAM=0
DSET_ADDR=0x467104
export AR6002_REV7_VER FLASH_FLAVOR BD_FILENAME ATHWLAN_FLAVOR TUNE_PARAM DSET_PATCH_FILENAME MAKE_FLASH_APPS_ONLY APPS_FILENAME APPS_OUTFILE DSET_ADDR

SUPPORT_DIR="."
OBJDUMP=${OBJDUMP:-objdump}
NM=${NM:-nm}
PATTERN_FIND_TOOL="extract.out"
EXT_BD_SUBMIT="0"
FLASH_LENGTH=0
BD_A_FILENAME="fakeBoardData_p1.bin"
BD_B_FILENAME="fakeBoardData_p2.bin"
PATCH_DIR="../../patch/AR6006/v${AR6002_REV7_VER}"
IMAGE_DIR="../.output/QCA4004/SP143/image"
BIN_DIR="../.output/QCA4004/SP143/image"
INCLUDE_DIR="../../../include"
FINAL_FLASH_FILENAME="raw_flashimage_AR400X_REV${AR6002_REV7_VER}${FLASH_FLAVOR}.bin"
BIN_FILENAME=${BIN_FILENAME:-"flashimage.bin"}
CFG_FILENAME="config_header.bin"
OUT_FILENAME="flash_AR400X_REV${AR6002_REV7_VER}${FLASH_FLAVOR}.bin"
INC_FILENAME="sdk_flash.bin"
ELF_FILENAME="sdk_flash.out"
PART_TABLE_FILENAME="parttable.bin"
FLASH_DESC_PATTERN_OFFSET=20
INC_LENGTH=0
CURR_DIR=$PWD
# FLASH_DESC_FILENAME - is a file that describes the flash chip
FLASH_DESC_FILENAME="flash_desc.bin"
# FLASH_DESC_NAME - is the input name for nvram_seek()
FLASH_DESC_NAME="flash_descriptor"
PARTITION_ENTRY_1=0x00040000
PARTITION_ENTRY_2=0xFFFFFFFF
PARTITION_ENTRY_3=0xFFFFFFFF
PARTITION_TABLE_SIZE=12
FLASH_CAPACITY=0x00200000
FLASH_BLOCKSIZE=0x00010000
FLASH_SECTORSIZE=0x00001000
FLASH_PAGESIZE=0x00000100

PARTITION_TABLE_START=$(($PARTITION_ENTRY_1 - $PARTITION_TABLE_SIZE))

if [ -z "$DSET_PATCH_FILENAME" ]; then
DSET_PATCH_FILENAME="../../dset_DB142.bin"
fi

#
# Hostproxy param is athwlan_start_addr<<8 + EXT_BD_LENGTH
# this results in the start address and the BD_LENGTH overlapping by 4 bits which is ok as
# it is assumed the lower 4-bits of the start address will always be 0.
#
#START_ADDR=$($OBJDUMP -f $IMAGE_DIR/athwlan${ATHWLAN_FLAVOR}.out | grep "start address" | sed -e 's/  */ /g' | cut -d " " -f 3)
START_ADDR=0x$($OBJDUMP -f $IMAGE_DIR/$APPS_OUTFILE | grep "start address" | cut -b 17-24)
echo "app-start=" $START_ADDR
START_ADDR=$(printf "%d\n" $START_ADDR)
START_ADDR=$(($START_ADDR*256))
#PROXY_PARAM=$(($START_ADDR+$EXT_BD_LENGTH))
PROXY_PARAM=$START_ADDR
#PROXY_PARAM=0x00000000
echo "proxy param =" $(printf "%08x" $PROXY_PARAM)

#
# If we did not specify an APPS_FILENAME, supply
# a default and build it from scratch.  If we DID
# supply such a name, then only build it if it
# doesn't already exist.
#
if [ -z "$APPS_FILENAME" ]; then
    APPS_FILENAME=flashapps.bin
    rm -f $APPS_FILENAME
fi

MAKE_FLASH_APPS_ONLY=${MAKE_FLASH_APPS_ONLY:-0}
#if [ \( \! -r $APPS_FILENAME \) -o \( "$MAKE_FLASH_APPS_ONLY" -eq 1 \) ]; then
    #
    # acquire the DSET address from targaddrs.h
    #
    #DSET_ADDR=$(cat $INCLUDE_DIR/targaddrs.h | grep "define AR6006_REV${AR6002_REV7_VER}_DATASET_PATCH_ADDRESS" | grep -o '0x[[:xdigit:]]\{6,8\}')
    #DSET_ADDR=0x467104
    echo "my dset_addr=" $DSET_ADDR
    echo "ver=" $AR6002_REV7_VER
    echo "rune-para=" $TUNE_PARAM

    # Create the applications segment of the flash image from ELF files
    ${SUPPORT_DIR}/makenvrampart.sh \
            -out $APPS_FILENAME \
            -new \
            -word $PARTITION_TABLE_START \
            -fill 0xffffffff 3 \
            -magic \
            -name tune -exec $IMAGE_DIR/tune.out $TUNE_PARAM \
            -name hostproxy -exec $IMAGE_DIR/sdkproxy.out $PROXY_PARAM \
            -name boarddata -bddata $BD_FILENAME \
            -name bootrouter -exec $IMAGE_DIR/boot_router.out 0x0 \
            -name otp -exec $IMAGE_DIR/otp_iot.out 0x2 \
            -name cdrtest -load $IMAGE_DIR/$APPS_OUTFILE 0x0 \
            -stop -name dsetpatch -data $DSET_PATCH_FILENAME $DSET_ADDR \
            -name $FLASH_DESC_NAME -noload $FLASH_DESC_FILENAME
    if [ "$MAKE_FLASH_APPS_ONLY" -eq 1 ]; then
        exit
    fi
#fi

echo my rev $AR6002_REV7_VER

cp $APPS_FILENAME $BIN_FILENAME

# Construct ReadOnly DataSet Image
# File to contain Read-Only DataSet image
RODATASET_FILENAME=${RODATASET_FILENAME:-"rodsetimage.bin"}
export RODATASET_FILENAME

# (TBD: these are temporary)
#RODATASET_MAXSIZE=$((0x10000))
#RWDATASET_MAXSIZE=$((0xfff4))
RODATASET_MAXSIZE=0
RWDATASET_MAXSIZE=0
RWDATASET_START=$(($PARTITION_TABLE_START - $RWDATASET_MAXSIZE))
RODATASET_START=$(($RWDATASET_START - $RODATASET_MAXSIZE))
#RWDATASET_START=0
#RODATASET_START=0

if [ -r $RODATASET_FILENAME ]; then
    RODATASET_LENGTH=$(stat --format=%s $RODATASET_FILENAME)
    if [ $RODATASET_LENGTH -gt $RODATASET_MAXSIZE ]; then
        echo "Error: ReadOnly DataSet image is too large."
        exit
    fi
else
    RODATASET_FILENAME=""
    RODATASET_LENGTH=0
fi

FLASH_RODSET_ABS_START=0xffffffff # TBD: not yet used
FLASH_RODSET_START=$RODATASET_START
FLASH_RWDSET_START=$RWDATASET_START
FLASH_RWDSET_SZ=$RWDATASET_MAXSIZE

# Flash descriptor (see nvram_dset_configuration_s)
${SUPPORT_DIR}/makenvrampart.sh \
        -out $FLASH_DESC_FILENAME \
        -new \
        -word  $FLASH_CAPACITY \
        -word  $FLASH_BLOCKSIZE \
        -word  $FLASH_SECTORSIZE \
        -word  $FLASH_PAGESIZE \
	-word  $FLASH_RODSET_ABS_START \
	-word  $FLASH_RODSET_START \
	-word  $FLASH_RWDSET_START \
	-word  $FLASH_RWDSET_SZ

${SUPPORT_DIR}/makenvrampart.sh \
        -out $BIN_FILENAME \
        -name $FLASH_DESC_NAME -noload $FLASH_DESC_FILENAME

${SUPPORT_DIR}/makenvrampart.sh \
        -out $PART_TABLE_FILENAME \
        -new \
        -word $PARTITION_ENTRY_1 \
        -word $PARTITION_ENTRY_2 \
        -word $PARTITION_ENTRY_3

#
# Create RAW flash image for use by customers that wish to (pre-)program
# flash chips directly. {
#
FLASH_LENGTH=$(stat --format=%s $BIN_FILENAME)
PAD1_LENGTH=$(($RODATASET_START - $FLASH_LENGTH))
tail -c $PAD1_LENGTH fill.bin >> sub_fill1.bin

# Padding between end of RO DataSets and start of Partition Table
PAD2_LENGTH=$(($PARTITION_TABLE_START - $RODATASET_START - $RODATASET_LENGTH))
tail -c $PAD2_LENGTH fill.bin >> sub_fill2.bin

# DEBUG
echo "FLASH_LENGTH=" $FLASH_LENGTH
echo "PAD1_LENGTH=" $PAD1_LENGTH
echo "PAD2_LENGTH=" $PAD2_LENGTH
echo "RODATASET_START=" $RODATASET_START "RODATASET_LENGTH=" $RODATASET_LENGTH "RODATASET_FILENAME=" $RODATASET_FILENAME
#echo "PAD2_LENGTH=" $PAD2_LENGTH
echo "PARTITION_TABLE_START=" $PARTITION_TABLE_START "PARTITION_TABLE_SIZE=" $PARTITION_TABLE_SIZE
#echo "PARTITION_ENTRY_1=" $PARTITION_ENTRY_1
#echo "my proxy param =" $PROXY_PARAM

cat $BIN_FILENAME sub_fill1.bin $RODATASET_FILENAME sub_fill2.bin $PART_TABLE_FILENAME > $FINAL_FLASH_FILENAME
#TMP_FLASH_LENGTH=$(($PARTITION_TABLE_START - $FLASH_LENGTH))
#echo "part-len=" $PARTITION_TABLE_START
#echo "tmp-len=" $TMP_FLASH_LENGTH
#rm -f sub_fill.bin
#tail -c $TMP_FLASH_LENGTH fill.bin >> sub_fill.bin
#cat $BIN_FILENAME sub_fill.bin $PART_TABLE_FILENAME > $FINAL_FLASH_FILENAME

rm -f sub_fill1.bin
rm -f sub_fill2.bin
#cp -f $BIN_DIR/$INC_FILENAME $CURR_DIR/$INC_FILENAME
cp -f $BIN_FILENAME $CURR_DIR/$INC_FILENAME
cp -f $IMAGE_DIR/$ELF_FILENAME $CURR_DIR/$ELF_FILENAME

# Done creating RAW image }

# Create and append segments (matches flash_segment_hdr) {
sizeof_flash_segment_hdr=12
mem_addr=$(printf "%x" 0x$($NM $ELF_FILENAME | grep " _end$" | cut -d " " -f 1))
echo "mem_addr=" $mem_addr

# Append the applications flash segment
#*******flash segment header***********#
#used by iot_flash
#struct flash_segment_hdr {
#            A_UINT32 flash_magic;
#            A_UINT32 flash_addr;
#            A_UINT32 flash_nbytes;
#        };
#**************************************#
seg_length=$(stat --format=%s $BIN_FILENAME)
flash_addr=0
${SUPPORT_DIR}/makenvrampart.sh \
        -out seg_hdr \
        -new \
        -word 0x48534c46 \
        -word $flash_addr \
        -word $seg_length
cat seg_hdr $BIN_FILENAME > seg
${SUPPORT_DIR}/makeseg.sh -o $INC_FILENAME -nc -data seg $mem_addr
mem_addr=$(printf "%x" $((0x$mem_addr + $seg_length + $sizeof_flash_segment_hdr)))

# Append Partition Table segment
flash_addr=$PARTITION_TABLE_START
seg_length=$PARTITION_TABLE_SIZE
#*******flash segment header***********#
#used by iot_flash
#struct flash_segment_hdr {
#            A_UINT32 flash_magic;
#            A_UINT32 flash_addr;
#            A_UINT32 flash_nbytes;
#        };
#**************************************#
${SUPPORT_DIR}/makenvrampart.sh \
        -out seg_hdr \
        -new \
        -word 0x48534c46 \
        -word $flash_addr \
        -word $seg_length
cat seg_hdr $PART_TABLE_FILENAME > seg

${SUPPORT_DIR}/makeseg.sh -o $INC_FILENAME -nc -data seg $mem_addr
mem_addr=$(printf "%x" $((0x$mem_addr + $seg_length + $sizeof_flash_segment_hdr)))

# Append custom segment(s), if any
#*******flash segment header***********#
#used by iot_flash
#struct flash_segment_hdr {
#            A_UINT32 flash_magic;
#            A_UINT32 flash_addr;
#            A_UINT32 flash_nbytes;
#        };
#**************************************#
if [ $RODATASET_LENGTH -gt 0 ]; then
    flash_addr=$RODATASET_START
    seg_length=$RODATASET_LENGTH
    ${SUPPORT_DIR}/makenvrampart.sh \
            -out seg_hdr \
            -new \
            -word 0x48534c46 \
            -word $flash_addr \
            -word $seg_length
    cat seg_hdr $RODATASET_FILENAME > seg

    ${SUPPORT_DIR}/makeseg.sh -o $INC_FILENAME -nc -data seg $mem_addr
    mem_addr=$(printf "%x" $((0x$mem_addr + $seg_length + $sizeof_flash_segment_hdr)))
fi

# Append termination flash segment and terminate segmented file with "-done"
flash_addr=0
seg_length=0
#*******flash segment header***********#
#used by iot_flash
#struct flash_segment_hdr {
#            A_UINT32 flash_magic;
#            A_UINT32 flash_addr;
#            A_UINT32 flash_nbytes;
#        };
#**************************************#
${SUPPORT_DIR}/makenvrampart.sh \
        -out seg_hdr \
        -new \
        -word 0x48534c46 \
        -word $flash_addr \
        -word $seg_length
${SUPPORT_DIR}/makeseg.sh -o $INC_FILENAME -nc -data seg_hdr $mem_addr -done
rm -f seg_hdr seg
# done appending segments }

#
# Create the Config Header file for the front of final binary
#
INC_LENGTH=$(stat --format=%s $INC_FILENAME)

#
# to find the mac offset search for 00 03 7f aa bb cc
# to find the partition table start search for the 3 partition values in order
# to find the flash descriptor search for "flash_descriptor" then add 16+4=20 to the
# offset to get past the string and the checksum.
# extract.out searches a binary file for the input pattern and returns the byte offset.
# the patterns for each value are believed to be unique. these patterns must be changed
# if the values used to build INF_FILENAME are changed.
#
#
# make the extract tool on the local machine to ensure compatibility.
#
gcc -o $PATTERN_FIND_TOOL extract.c
chmod 755 $PATTERN_FIND_TOOL
MAC_OFFSET=$(./$PATTERN_FIND_TOOL 00037f200142 $INC_FILENAME)
MAC_OFFSET=0
FLASH_DESC_OFFSET=$(./$PATTERN_FIND_TOOL 666c6173685f64657363726970746f72 $INC_FILENAME)
FLASH_DESC_OFFSET=$(($FLASH_DESC_PATTERN_OFFSET + $FLASH_DESC_OFFSET))
PART_OFFSET=$(./$PATTERN_FIND_TOOL 00000400ffffffffffffffff $INC_FILENAME)
START_ADDR=$($OBJDUMP -f $ELF_FILENAME | grep "start address" | sed -e 's/  */ /g' | cut -d " " -f 3)
echo "start=" $START_ADDR
echo "mac-off=" $MAC_OFFSET
${SUPPORT_DIR}/makenvrampart.sh \
        -out $CFG_FILENAME \
        -new \
        -word  $INC_LENGTH \
        -word  0x1234 \
        -word  $START_ADDR \
        -word  $MAC_OFFSET \
        -word  0x00000000 \
        -word  0x00000000 \
        -word  0x00000000 \
        -word  $PART_OFFSET \
        -word  $FLASH_DESC_OFFSET
#
# Create the final output file via concatenation of config header + iot_flashotp.bin
#
cat $CFG_FILENAME $INC_FILENAME > $OUT_FILENAME


#
# Make the flashimage.inc which will be included by sdk_flash.c
#	
#rm -f $CURR_DIRflashimage/.inc
#make -f Makefile.img flashimage.inc

#
# Make the partitiontable.inc which will be included by sdk_flash.c
#
#rm -f $CURR_DIR/parttable.inc
#make -f Makefile.img parttable.inc

# Make ota image 


# Make ota image
./ota.sh flashimage.bin ota.bin
cp raw_flashimage_AR400X_REV2_IOT.bin ../output/raw_sdkshell.bin
cp ota.bin ../output/ota_sdkshell.bin
