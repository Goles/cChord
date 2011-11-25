/*
 *  example.cpp
 *  A simple main() to try out the Library
 *  Created by Laurent Vanni & Nicolas Goles Domic, 2010
 *
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <myMed/ChordNode.h>
#include <pthread.h>
#include <myMed/ProtocolSingleton.h>

using namespace std;

// This application receives args, "ip", "port", "overlay identifier (unique string)", "root directory)"
int main(int argc, char * const argv[]) 
{
	string backBone[] = {
			// user backbone
            "127.0.0.1",
	};
	
    Node *chord = NULL;
    ChordNode *node = NULL;

	if (argc >= 4) {
		// Create a test node
		node = P_SINGLETON->initChordNode(std::string(argv[1]), atoi(argv[2]), std::string("chordTestBed"), std::string(argv[3]));
        chord = NULL;
        
		// join to an existing chord
		if (argc == 5) {
			cout << "joining...\n";
			int i = 0;
		    chord = new Node(backBone[0], 8000);
			node->join(chord);
		}

		char entry[256];
		string key;
		string value;
		
		while (1) { // sleep...
			cout << "\n0) Print status\n" << 
					"1) Put\n" << 
					"2) Get\n" <<
					"3) Remove\n" << 
					"4) Exit\n\n";
			cout << "---> ";
			cin >> entry;
			int chx = atoi(entry);

			switch (chx) {
    			case 0:
    				cout << "\n" << node->printStatus();
    				break;
    			case 1:
    				cout << "Key = ";
    				cin >> key;
    				cout << "Value = ";
    				cin >> value;
    				node->put(key, value);
    				break;
    			case 2:
    				cout << "Key = ";
    				cin >> key;
    				cout << "\n" << node->get(key) << "------> found!" << endl;
    				break;
    			case 3:
    				cout << "Key = ";
    				cin >> key;
    				node->removekey(key);
    				break;
    			case 4:
    				node->shutDown();
    			default:
				break;
			}       
	    }
    } else {
		cout << "wrong parameters: test.out <hostname> <portNumber> <webContentDirectory> [--join]\n";
	}

    delete node;
    delete chord;
	return 0;
}
