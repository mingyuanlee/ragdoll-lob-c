RB Tree

Limit Map: Each limit price maps to Limit Node
Order Map: Each order id maps to Order Node
The two maps are used for looking up, they don't own the nodes, the nodes are owned by the Tree.

The reason of choosing RB Tree over AVL Tree
LOB is insert-intensive instead of lookup-intensive.
Reference: https://stackoverflow.com/questions/13852870/red-black-tree-over-avl-tree#:~:text=Both%20red%2Dblack%20trees%20and,hence%20provide%20faster%20look%2Dups.



Check these:
https://www.uoitc.edu.iq/images/documents/informatics-institute/Competitive_exam/DataStructures.pdf
https://debby-nirwan.medium.com/how-is-c-map-implemented-8cc10c93684a
