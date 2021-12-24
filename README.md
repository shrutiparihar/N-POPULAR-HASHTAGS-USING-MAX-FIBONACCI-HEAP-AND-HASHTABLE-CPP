# N-POPULAR-HASHTAGS-USING-MAX-FIBONACCI-HEAP-AND-HASHTABLE-CPP

How to run the code - 
g++ -std=c++11 hashtag.cpp -o hashtag

You are required to implement a system to find the n most popular hashtags that appear on social media  such as Facebook or Twitter. For the scope of this project hashtags will be given from an input file.  Basic idea for the implementation is to use a max priority structure to find out  the most popular hashtags. 

You must use the following structures for the implementation. 
 
1. Max Fibonacci heap: use to keep track of the frequencies of hashtags. 
 
2.  Hash table: The key for the hash table is the hashtag, and the value is the pointer to the corresponding  node in the Fibonacci heap. 
 
You can assume there will be a large number of hashtags appearing in the stream and you need to perform  increase key operation many times. Max Fibonacci heap is recommended because it has an amortized complexity of O(1) for the increase key operation. You should implement all Fibonacci heap functions  discussed  in  class.  For  the  hash  table,  you  can  use  an  implementation  from  any  library  that  your  programming languages supports (e.g., STL). 

For each query n, you need to write the n most popular hashtags (i.e., highest frequency) to the output file in descending order of frequency (ties may be broken arbitrarily). The output for a query should be a comma separated list occupying a single line in the output “output_file.txt”. There should be no space character after the commas. 

You can have  following assumptions 
 
1. No uppercase letters in the input stream 
2. No spaces  in the hashtags. (only one  word per  one  hashtag) 
3. One query has only one integer. 
4. Query integer, n<=20. i.e. you need to find at most 20 popular hashtags. 
5. Input file may consist of more than 1 million hashtags. 

All the details regarding the project creation is mentioned in the REPORT pdf in the repository.
