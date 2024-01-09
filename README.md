
Limit Map: Each limit price maps to Limit Node
Order Map: Each order id maps to Order Node
The two maps are used for looking up, they don't own the nodes, the nodes are owned by the Tree.

The limit node owns the order node.

LLRBTree is responsible for:
1. handle duplicate price limit node ingestion (will return if already exists)

LOB is responsible for:
1. updating best ask and bid prices.
1. make sure there is no dangling limit node (= limit node with no order)
  1. this means LOB will delete the whole node when the last order is removed 
  1. a possible optimization: don't remove limit node to avoid frequent insertion/deletion, give every node a timeout value, use a scheduling task to remove all nodes if timeout value is reached

The reason of choosing RB Tree over AVL Tree
LOB is insert-intensive instead of lookup-intensive.
Reference: https://stackoverflow.com/questions/13852870/red-black-tree-over-avl-tree#:~:text=Both%20red%2Dblack%20trees%20and,hence%20provide%20faster%20look%2Dups.



Check these:
https://www.uoitc.edu.iq/images/documents/informatics-institute/Competitive_exam/DataStructures.pdf
https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html



Tech Debt:
- We haven't examined memory leak and dangling pointer problem in deletion