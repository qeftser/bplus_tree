## B+ Tree
Variation of the B-Tree that duplicates parent keys to child nodes, resulting in all keys being present in the leaves of the tree. This allows for efficient iteration over all the elements in the B+ Tree, with the cost of added memory for storing duplicaates. 
### Usage
This B+ Tree stores keys as long integers and values as integers.   
To initialize the B+ Tree use:
```
struct bx_tree bx;
bx.filepath = "/path/to/storage/folder";
start_bx_tree(&bx);
```
To add elements use:
```
add_bx_tree(<long int - key>, <int - val>, &bx);
```
To get a value at an element:
```
get_bx_tree(<long int - key>, &bx); // Returns MIN_INT if value is not found
```
To remove a value at an element:
```
rem_bx_tree(<long int - key>, &bx); // Sets value to MIN_INT if it exists to keep tree ordering.
```
At the end of the program or the B+ Tree's use call:
```
close_bx_tree(&bx); // Unloads any cached values or changes
```
## TODO
- Modify find index method to use binary search
- Add iterator to take advantage of B+ Tree properties

