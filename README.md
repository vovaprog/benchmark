Benchmark
=========

System information:<br/>
c++ compiler: /usr/bin/c++  
c++ compiler version: c++ (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609  
compiler flags:  -std=c++11 -march=native -Wall -pedantic -O3 -DNDEBUG  
cpu: Intel(R) Core(TM) i3-5010U CPU @ 2.10GHz  
boost: 1_58  
os: Ubuntu 16.04.2 LTS  

map search
----------

Variable number of items is added to map (number of items stored in map is the value on X axis).
Then constant number of map searches is done. Number of microseconds that test takes is the value of Y axis.
Key is random uint64 value.

comparison of tree, hash table and binary search maps
-----------------------------------------------------

Comparison of tree, hash and binary search structures with up to 350 elements:<br/>
![map find](./plots/map_find_350_compare.png)


Comparison of tree, hash and binary search structures with up to 3500 elements:<br/>
![map find](./plots/map_find_3500_compare.png)


Comparison of tree, hash and binary search structures with up to 100000 elements:<br/>
![map find](./plots/map_find_100000_compare.png)


comparison of tree maps
-----------------------

Comparison of tree structures with up to 350 elements:<br/>
![map find](./plots/map_find_350_tree.png)

Comparison of tree structures with up to 3500 elements:<br/>
![map find](./plots/map_find_3500_tree.png)

Comparison of tree structures with up to 100000 elements:<br/>
![map find](./plots/map_find_100000_tree.png)


comparison of hash table maps
-----------------------------

Comparison of hash table structures with up to 350 elements:<br/>
![map find](./plots/map_find_350_hash.png)

Comparison of hash table structures with up to 3500 elements:<br/>
![map find](./plots/map_find_3500_hash.png)


Comparison of hash table structures with up to 100000 elements:<br/>
![map find](./plots/map_find_100000_hash.png)


comparison of binary search maps
--------------------------------

Comparison of binary search structures with up to 350 elements:<br/>
![map find](./plots/map_find_350_bin_search.png)

Comparison of binary search structures with up to 3500 elements:<br/>
![map find](./plots/map_find_3500_bin_search.png)

Comparison of binary search structures with up to 100000 elements:<br/>
![map find](./plots/map_find_100000_bin_search.png)

search in map with string keys
------------------------------

X number of items is added to map, than constant number of searches are performed.
Key is a random string. String is std::string, char p is a char pointer.

comparison of tree and hash table maps with string key
------------------------------------------------------

Tree and hash table maps with 16 byte string keys:<br/>
![string map](./plots/string_map_compare_16_100.png)

Tree and hash table maps with 16 byte string keys:<br/>
![string map](./plots/string_map_compare_16_1000.png)

tree maps with string key
-------------------------

Tree maps with 16 byte string keys:<br/>
![string map](./plots/string_map_find_16_100.png)

Tree maps with 16 byte string keys:<br/>
![string map](./plots/string_map_find_16_1000.png)

Tree maps with 4 byte string keys:<br/>
![string map](./plots/string_map_find_4_100.png)

Tree maps with 4 byte string keys:<br/>
![string map](./plots/string_map_find_4_1000.png)

hash table maps with string key
-------------------------------

hash table maps with 16 byte string keys:

![string map](./plots/string_unordered_map_find_16_100.png)

hash table maps with 16 byte string keys:

![string map](./plots/string_unordered_map_find_16_1000.png)

hash table maps with 4 byte string keys:

![string map](./plots/string_unordered_map_find_4_100.png)

hash table maps with 4 byte string keys:

![string map](./plots/string_unordered_map_find_4_1000.png)


map insert
----------

X number of items are added to map.
Time that takes to insert X items is on Y axis.
Key is uint64 random number.

![map insert](./plots/map_insert_10000.png)

comparison of switch and maps
-----------------------------

X is number of cases in switch and number of items in map.
Constant number of searches is done. Y is time to complete searches.

![switch](./plots/switch.png)

arithmetic
----------

comparison of arithmetic operations done on different data types.

![arithmetic](./plots/arithmetic.png)
