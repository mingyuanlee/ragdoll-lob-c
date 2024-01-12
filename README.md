Ragdoll LOB is a limit order book model leveraging LLRB Tree to enable high speed trading.


### Speed Expectations

1. Insert Order: O(1). O(logm) for the first-time limit price insertion.
2. Cancel Order: O(1).
3. Market Order Execution: O(1) for small quantity, O(logm) as upper bound.
4. Range Query: Balanced BST range query speed.
5. Bid/Ask size etc Metadata Query: O(1)

Note that m is the number of different limit prices and n is the number of orders. We assume n is much larger than m, and m is a relatively small number - imagine that a trading system has 10m orders and all of them around price 150.00 - 250.00, 10k prices.


### How We Achieve High Speed

- We maintain RB Tree for Ask and Bid orders respectively. The key of treenode is the limit price and data is a collection of orders at that limit price, with all the metadata like volume. This makes the first time tree node insertion O(logm), also enables efficient range search.
- The collection of orders in each limit price tree node are maintained as a doubly linked list so that inserting and removing arbitrary order will cost O(1).
- We use two lookup hashmaps for Limit Price Node and Order Node respectively, so we achieve O(1) speed when locating the tree node and order node.


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