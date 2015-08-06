#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "player.h"
#include "player_vector.h"
#include "formatted_line.h"
#include "logger.h"
#include "parser.h"
#include "configuration.h"

#include "write_stats_to_file.h"


std::string currentTime() {
    std::chrono::time_point<std::chrono::system_clock> time_p = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(time_p);  // Time since 1970
    return std::ctime(&time);  // ctime converts to string of todays date
}

int main(void) {
    errorLog.write("");
    errorLog.write("Program started at: ", false);
    errorLog.write(currentTime());

    Configuration config;
    if(!config.read()) {
        return 1;
    }
    Parser parser(config.getplayerRunningProgram());
    PlayerVector<Player> pv;

	std::ifstream logstream("../../test/test_damage.txt");

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

        std::string logLine;
		bool is_running = true;
		while (is_running) {
			while (getline(logstream, logLine)) {
				std::cout << logLine << std::endl;
				FormattedLine formattedLine(logLine);
                // TODO: Figure out some way to handling this check nicer.
				if (formattedLine.isFormatted()) {
                    LineInfo lineInfo = parser.parse(formattedLine);
                    if(lineInfo.hasStats) {
                        pv.addToPlayers(lineInfo);
                    }
                    else if (!lineInfo.command.empty()) {
                        // write a new class/header for this that deals with input
                        if (lineInfo.command == "dd") {
                            writeDamageDealtOverview(pv);
                            writeDamageReceivedOverview(pv);
                            writeDamageDealtPerOpponent(pv.getPlayer("You"));
                        }
                    }
                }
            }
			if (!logstream.eof()) {  // Why did I check for this?
				std::cout << "Error: Not EOF!" << std::endl;
				is_running = false;
			}
			logstream.clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	}
	else {
		std::cerr << "Could not open the log file." << std::endl;
	}

    errorLog.write("");
	errorLog.write("Program ended at: ", false);
    errorLog.write(currentTime());

	std::getchar();
	return 0;
}
