# c-hash-table

Basic hash table utility C exercise

## Getting Started

Build a hash table with sorted bucket link list in ascending order, duplicated keys will be discardedr, the unit test will be performed by the main program.

## Supported Functions
 * Key Insertion with User Data
 * Key Deletion
 * Key Seeking with User Data retrieval
 * All Keys Listing
  
## Future Expansion
 * Multiple Hash Table Instances
 * Table Locking
 * Optional User Data

## Running the Unit Test

Tested with pre-defined string arrayi in code, test scenario covers 
 * Unique key insertion
 * Discard duplicated key while inserting
 * Existed keys search
 * Non-existed key search
 * Keys traversal listing 
 * Non-existed key deletion
 * Existed key deletion

## Built With AMD64 Ubuntu 18.04 LTS GNU Tool Chain 

* [GNU Tool Chain Configuration]
  CC: gcc
  CXX: g++
  CFLAGS: -Werror -Wno-deprecated -O2 -g -DDEBUG
  INCLUDES: <none>
  LIBS: <none>

## Authors

* **Frederic Chen** - *Test Succeed* 

## License

* None
