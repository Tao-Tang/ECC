#ECC
A clustering-based reference selection algorithm for reference-based compression on genome dataset. This program requires **C++14 with g++ >= 5.3** and works on Linux

## Download and Compile
	git clone https://github.com/Tao-Tang/ECC.git
	cd ECC
	./bootstrap.sh
	./configure
	make
	
## Clusteinrg and Compression
	./ECC p -r result_name -s src_list -t thread_number for reference-target pair selectoin
	./ECC c -r result_name -s src_list -t thread_number for reference-target pair selectoin and compress data via hirgc
	./ECC d src_file for decompression
	Examples:
	./ECC p -r my_archive -s file_list.txt -t 4
	./ECC p -r my_archive -s file_list.txt -t 4
	./ECC d my_archive

## Example
	cp ECC test
	./ECC c -r a -s list.txt -t 4
	ls -lh ECC_a.tar
	./ECC d a
	ls -lh ECC_a
	
