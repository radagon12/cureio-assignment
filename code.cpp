#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>

struct LogEntry {
    int64_t timestamp;
    std::string log_type;
    float severity;
};

class LogMonitor {
public:
    void addLogEntry(const LogEntry& entry) {
        logs.push_back(entry);
        log_type_map[entry.log_type].push_back(entry);
    }

    void computeStatisticsByLogType(const std::string& log_type) const {
        if (log_type_map.find(log_type) == log_type_map.end()) {
            printStatistics(0.0, 0.0, 0.0);
            return;
        }

        computeStatistics(log_type_map.at(log_type));
    }

    void computeStatisticsBeforeTimestamp(int64_t timestamp) const {
        std::vector<LogEntry> filtered_logs;
        for (const auto& log : logs) {
            if (log.timestamp < timestamp) {
                filtered_logs.push_back(log);
            }
        }

        computeStatistics(filtered_logs);
    }

    void computeStatisticsAfterTimestamp(int64_t timestamp) const {
        std::vector<LogEntry> filtered_logs;
        for (const auto& log : logs) {
            if (log.timestamp > timestamp) {
                filtered_logs.push_back(log);
            }
        }

        computeStatistics(filtered_logs);
    }

    void computeStatisticsBeforeTimestampForLogType(const std::string& log_type, int64_t timestamp) const {
        if (log_type_map.find(log_type) == log_type_map.end()) {
            printStatistics(0.0, 0.0, 0.0);
            return;
        }

        std::vector<LogEntry> filtered_logs;
        for (const auto& log : log_type_map.at(log_type)) {
            if (log.timestamp < timestamp) {
                filtered_logs.push_back(log);
            }
        }

        computeStatistics(filtered_logs);
    }

    void computeStatisticsAfterTimestampForLogType(const std::string& log_type, int64_t timestamp) const {
        if (log_type_map.find(log_type) == log_type_map.end()) {
            printStatistics(0.0, 0.0, 0.0);
            return;
        }

        std::vector<LogEntry> filtered_logs;
        for (const auto& log : log_type_map.at(log_type)) {
            if (log.timestamp > timestamp) {
                filtered_logs.push_back(log);
            }
        }

        computeStatistics(filtered_logs);
    }

private:
    std::vector<LogEntry> logs;
    std::unordered_map<std::string, std::vector<LogEntry>> log_type_map;

    void computeStatistics(const std::vector<LogEntry>& log_entries) const {
        if (log_entries.empty()) {
            printStatistics(0.0, 0.0, 0.0);
            return;
        }

        float min_severity = std::numeric_limits<float>::max();
        float max_severity = std::numeric_limits<float>::lowest();
        float sum_severity = 0.0;

        for (const auto& entry : log_entries) {
            min_severity = std::min(min_severity, entry.severity);
            max_severity = std::max(max_severity, entry.severity);
            sum_severity += entry.severity;
        }

        float mean_severity = sum_severity / log_entries.size();
        printStatistics(min_severity, max_severity, mean_severity);
    }

    void printStatistics(float min_severity, float max_severity, float mean_severity) const {
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Min: " << min_severity << ", Max: " << max_severity << ", Mean: " << mean_severity << '\n';
    }
};

LogEntry parseLogEntry(const std::string& log) {
    std::istringstream ss(log);
    std::string timestamp_str, log_type, severity_str;
    
    std::getline(ss, timestamp_str, ';');
    std::getline(ss, log_type, ';');
    std::getline(ss, severity_str, ';');
    
    int64_t timestamp = std::stoll(timestamp_str);
    float severity = std::stof(severity_str);

    return {timestamp, log_type, severity};
}

int main() {
    LogMonitor monitor;
    std::string input;

    freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	freopen("error.txt","w",stderr);
    
    while (std::getline(std::cin, input)) {
        std::istringstream ss(input);
        int command;
        ss >> command;
        ss.ignore();  // Ignore the space after the command
        
        if (command == 1) {
            std::string log_entry;
            std::getline(ss, log_entry);
            monitor.addLogEntry(parseLogEntry(log_entry));
        } else if (command == 2) {
            std::string log_type;
            std::getline(ss, log_type);
            monitor.computeStatisticsByLogType(log_type);
        } else if (command == 3) {
            std::string direction;
            int64_t timestamp;
            ss >> direction >> timestamp;
            if (direction == "BEFORE") {
                monitor.computeStatisticsBeforeTimestamp(timestamp);
            } else if (direction == "AFTER") {
                monitor.computeStatisticsAfterTimestamp(timestamp);
            }
        } else if (command == 4) {
            std::string direction, log_type;
            int64_t timestamp;
            ss >> direction >> log_type >> timestamp;
            if (direction == "BEFORE") {
                monitor.computeStatisticsBeforeTimestampForLogType(log_type, timestamp);
            } else if (direction == "AFTER") {
                monitor.computeStatisticsAfterTimestampForLogType(log_type, timestamp);
            }
        }
    }

    return 0;
}
