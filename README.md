nonogram
========
this is original nonogram resolver
build under FreeBSD

run
nonogram < numbers
or
nonogram -a < numbers
for better asciiart

numbers file format
Horizontal Lines
*
Vertical Lines

Empty lines allowed

Example:
1 1
2 2
5
1 1
1 1 1 1
1 1
1 3 1
1 1
5
*
5
3 1
2 1 1 1
1 1 1
2 1 1 1
3 1
5

give

             2   2
             1 1 1
           3 1 1 1 3
         5 1 1 1 1 1 5 
    1 1|  []      []  |
    2 2|  [][]  [][]  |
      5|[][][][][][][]|
    1 1|[]          []|
1 1 1 1|[]  []  []  []|
    1 1|[]          []|
  1 3 1|[]  [][][]  []|
    1 1|[]          []|
      5|  [][][][][]  |
