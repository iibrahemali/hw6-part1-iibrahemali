all:
	g++ -g -Wall -o rbt-tests RedBlackTree.cpp RedBlackTreeTests.cpp
	g++ -g -Wall -o rbt-first RedBlackTree.cpp RedBlackTreeTestsFirstStep.cpp

valgrind:
	valgrind --leak-check=full ./rbt-tests
