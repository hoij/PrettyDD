#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "parse.h"

using namespace std;


int main(void) {
	string line;
	ifstream logstream("log.txt");
	vector<Player> players;

	if (logstream.is_open()) {
		logstream.seekg(0, logstream.end);

		// Copying a log file with all examples for testing purposes
		logstream.close();
		ifstream inlogstream("test_damage.txt");
		ofstream outlogstream("log.txt");
		while (getline(inlogstream, line)) {
			outlogstream << line << endl;
		}
		outlogstream.close();
		inlogstream.close();
		ifstream logstream("log.txt");
		//

		vector<string> previous_words;
		while (true) {
			while (getline(logstream, line)) {
				cout << line << endl;
				vector<string> words;
				split(line, ',', words); // Split the line and store it in words
				if (!words.size() == 0) {
					parse(previous_words, words, players);
					previous_words = words;
				}
			}
			if (!logstream.eof())
				break;
			logstream.clear();
			this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		logstream.close();
	}
	else {
		cout << "Could not open the file." << endl;
	}

	getchar();
	return 0;
}