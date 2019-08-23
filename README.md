## ECC
A clustering-based reference selection algorithm for reference-based compression on genome dataset. This program requires **C++14 with g++ >= 5.3** and works on Linux

## Download and Compile
	git clone https://github.com/Tao-Tang/ECC.git
	cd ECC
	./bootstrap.sh
	./configure
	make
## Execution	

./ECC p -r result_name -s src_list -t thread_number (for reference-target pair selection) <br/>
./ECC c -r result_name -s src_list -t thread_number (for reference-target pair selection and compress data via hirgc) <br/>
./ECC d src_file (for decompression) <br/>
	
- p for cluster a set of genome sequences
- c for cluster and compress a set of genome sequences
- d for decompress encoded file
- -r for the name of result file
- -s for the list of to be cluster sequences (each line will be read as a sequence name)
- -t for number of thread


##	Examples:
	./ECC p -r my_archive -s file_list.txt -t 4
	./ECC c -r my_archive -s file_list.txt -t 4
	./ECC d my_archive

## Example
	cp ECC test
	cd test
	./ECC p -r a -s list.txt -t 4
	nano ECC_a_pair.txt  (each line contains two files, first is the reference, second is target)
	./ECC c -r a -s list.txt -t 4
	ls -lh ECC_a.tar
	./ECC d a
	ls -lh ECC_a
	
