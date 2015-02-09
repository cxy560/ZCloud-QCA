#!/bin/bash
# tool to add OTA header at the front of firware
# Usatge: ota.sh <input-file> <output-file>

# Append a 32-bit word to the output file.
print_32bit()
{
	word32=$(printf "%08x\n" $1)

	byte0=$(echo $word32 | cut -b 7-8)
	byte1=$(echo $word32 | cut -b 5-6)
	byte2=$(echo $word32 | cut -b 3-4)
	byte3=$(echo $word32 | cut -b 1-2)

	printf "\x$byte3" >> $out_file
	printf "\x$byte2" >> $out_file
	printf "\x$byte1" >> $out_file
	printf "\x$byte0" >> $out_file
}


# Add a file (optionally compressing it) at a specified address to the output file.
add_ota_header() {
	in_file=$1
	out_file=$2
	LENGTH=$(stat --format=%s $in_file)
	echo $LENGTH
	let LENGTH=LENGTH-20
	echo $LENGTH

	tail -c $LENGTH $in_file > tmp.bin
	
	MD5=$(md5sum tmp.bin | cut -f1 -d " ")
	echo $MD5
	
	rm -f $out_file

	print_32bit 0x1234
	print_32bit $LENGTH
	echo -n $MD5 >> $out_file
	cat tmp.bin >> $out_file

	#rm -rf tmp.bin

}

add_ota_header $1 $2

