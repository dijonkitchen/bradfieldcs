# Indexes and Joins

## Join algorithms

### Nested-loops join

Loop through two tables and grab all combinations that match.

### Chunk nested-loops join

Loop through in bulk with pages or limit of memory buffers.

### Index nested-loops join

```
Loop through each record of table1
    Loop through each record of table2 where ids match via indexes
        Add matching record to results 
```

### Sort-merge join

```
Sort table1 by id
Sort table2 by id
Set pointer1 to table1's lowest id
Set pointer2 to table2's lowest id
Loop through sorted table1
    Loop through sorted table2
        If pointer1's id matches pointer2's id
            Add matching record to results
        If pointer1's id is less than pointer2's id
            Increment pointer1
        Else 
            Increment pointer2
```

### Hash joins

```
Build hash table of partitions of table1
Loop through table2 records
    If table2 record id matches
        Add matching records to results
```

### Hybrid hashing joins

## B+ Tree

Sort table by index field for bulk loading at one end of tree

```
Set order (number of keys per node)

Create root node of order size 
Point to first record index field of table

Loop through tuples of table
    Walk through tree
        If index field fits in node
            Add pointer to tuple
            Insert tuple into doubly-linked list for sequential search range
        Else
            Split parent node
            If parent is root node,
                Create new root node
                Point root node to children
```