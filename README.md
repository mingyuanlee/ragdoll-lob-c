### How We Achieve High Speed

Limit Map: Each limit price maps to Limit Node
Order Map: Each order id maps to Order Node
The two maps are used for looking up, they don't own the nodes, the nodes are owned by the Tree.

The limit node owns the order node.

### Responsibility

LLRBTree is responsible for:
1. handle duplicate price limit node ingestion (will return if already exists)
1. make sure there is no dangling limit node (= limit node with no order), this means the tree will delete the whole node when the last order is removed

LOB is responsible for:
1. updating best ask and bid prices.

### Dangling Limit Nodes

Allowing dangling limit nodes will avoid frequent insertion/deletion, give every node a timeout value, we can use a scheduling task to remove all nodes if timeout value is reached.
BUT this will make things complicated, for example:
- The LLRB tree operations must not ignore dangling limit nodes otherwise the tree will not be balanced anymore
- The query operations must ignore dangling limit nodes because they are not considered valid. For example, a dangling pointer should never indicate best ASK/BID price, and should never be included in the range search. 

### LLRB Tree

The reason of choosing RB Tree over AVL Tree
LOB is insert-intensive instead of lookup-intensive.
Reference: https://stackoverflow.com/questions/13852870/red-black-tree-over-avl-tree#:~:text=Both%20red%2Dblack%20trees%20and,hence%20provide%20faster%20look%2Dups.


### References

https://www.uoitc.edu.iq/images/documents/informatics-institute/Competitive_exam/DataStructures.pdf
https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html

### Features to do

1. How do users interact with the LOB?
2. Multiple LOBs on their own channels, and we have a centralized manager.

### Tech Debt

- We haven't examined memory leak and dangling pointer problem in deletion