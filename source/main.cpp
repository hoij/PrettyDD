#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include "parse.h"

using namespace std;


int main(void) {
	string line;
	ifstream logstream("../../test/test_damage.txt");
	vector<Player> players;

	if (logstream.is_open()) {
		// Go to the end of the file
		//logstream.seekg(0, logstream.end);
		//logstream.close();

		// Copying a log file with all examples for testing purposes
		//ifstream inlogstream("test_damage.txt");
		//ofstream outlogstream("log.txt");
		//while (getline(inlogstream, line)) {
		//	outlogstream << line << endl;
		//}
		//outlogstream.close();
		//inlogstream.close();

		//ifstream logstream("log.txt");

		bool is_running = true;
		while (is_running) {
			while (getline(logstream, line)) {
				cout << line << endl;
				parse(line, players);
			}
			if (!logstream.eof()) {  // Why did I check for this?
				cout << "Error: Not EOF!" << endl;
				is_running = false;
			}
			logstream.clear();
			this_thread::sleep_for(std::chrono::milliseconds(250));

			// Listen for user input to quit. Use SDL?

		}
		cout << "Closing the logstream" << endl;
		logstream.close();
	}
	else {
		cout << "Could not open the log file." << endl;
	}

	//for (auto& p : players)
	//	cout << p.get_name() << endl;

	getchar();
	return 0;
}
