program: btree_node.o btree.o command.o database.o external_index.o internal_index.o lexer.o list_node.o list.o main.o parser.o rbtree_node.o rbtree.o record.o
	g++ btree_node.o btree.o command.o database.o external_index.o internal_index.o lexer.o list_node.o list.o main.o parser.o rbtree_node.o rbtree.o record.o -std=c++11 -O3
btree_node.o: btree_node.cpp btree_node.h
	g++ -c btree_node.cpp -std=c++11 -O3
btree.o: btree.cpp btree.h
	g++ -c btree.cpp -std=c++11 -O3
command.o: command.cpp command.h
	g++ -c command.cpp -std=c++11 -O3
database.o: database.cpp database.h
	g++ -c database.cpp -std=c++11 -O3
external_index.o: external_index.cpp external_index.h
	g++ -c external_index.cpp -std=c++11 -O3
internal_index.o: internal_index.cpp internal_index.h
	g++ -c internal_index.cpp -std=c++11 -O3
lexer.o: lexer.cpp lexer.h
	g++ -c lexer.cpp -std=c++11 -O3
list_node.o: list_node.cpp list_node.h
	g++ -c list_node.cpp -std=c++11 -O3
list.o: list.cpp list.h
	g++ -c list.cpp -std=c++11 -O3
main.o: main.cpp parser.h database.h external_index.h
	g++ -c main.cpp -std=c++11 -O3
parser.o: parser.cpp parser.h command.h
	g++ -c parser.cpp -std=c++11 -O3
rbtree_node.o: rbtree_node.cpp rbtree_node.h
	g++ -c rbtree_node.cpp -std=c++11 -O3
rbtree.o: rbtree.cpp rbtree.h
	g++ -c rbtree.cpp -std=c++11 -O3
record.o: record.cpp record.h
	g++ -c record.cpp -std=c++11 -O3
