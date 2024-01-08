RB Tree

Limit Map: Each limit price maps to Limit Node
Order Map: Each order id maps to Order Node
The two maps are used for looking up, they don't own the nodes, the nodes are owned by the Tree.

The limit node owns the order node.

The reason of choosing RB Tree over AVL Tree
LOB is insert-intensive instead of lookup-intensive.
Reference: https://stackoverflow.com/questions/13852870/red-black-tree-over-avl-tree#:~:text=Both%20red%2Dblack%20trees%20and,hence%20provide%20faster%20look%2Dups.



Check these:
https://www.uoitc.edu.iq/images/documents/informatics-institute/Competitive_exam/DataStructures.pdf
https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html


Deletion example in Introduction To Algorithms
