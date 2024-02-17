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

#### Iterator
To initialize an iterator use:
```
struct bx_iterator bxi;
start_bx_iterator(&bx,&bxi);
```
Here are the operations allowed for an iterator:
```
has_next_bx_iterator(&bxi); 
next_bx_iterator(&bxi);    /* move to next value */

get_key_bx_iterator(&bxi); /* return key at current position */
get_val_bx_iterator(&bxi); /* get value at current position */

set_bx_iterator(100,&bxi); /* sets the current key to hold a new value */
rem_bx_iterator(&bxi);     /* remove the current key */
```
You can invalidate all iterators and check validation as well if you want. I do not do this in any of the functions though.
```
valid_bx_iterator(&bxi); /* is this iterator valid? */

invalidate_bx_iterators(&bx); /* now all iterators are invalid */
```
You can resue an iterator by just calling start_bx_iterator to move it back to the beginning.   
To loop through all elements and print them you could use something like:
```
start_bx_iterator(&bx,&bxi);

while (has_next_bx_iterator(&bxi)) {
    next_bx_iterator(&bxi);
    printf("< | KEY: %20ld | VAL: %10d | >",bxi->key,bxi->val);
}
```

## TODO
- Modify find index method to use binary search
- Better caching method?

