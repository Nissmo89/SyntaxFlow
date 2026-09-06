#include <string>
#include <vector>
#include <sstream>

class Solution {
public:
    string validIPAddress(string queryIP) {
        if (queryIP.find('.') != string::npos) return isIPv4(queryIP) ? "IPv4" : "Neither";
        if (queryIP.find(':') != string::npos) return isIPv6(queryIP) ? "IPv6" : "Neither";
        return "Neither";
    }

private:
    bool isIPv4(string ip) {
        int dots = 0;
        for (char c : ip) if (c == '.') dots++;
        if (dots != 3) return false;
        
        stringstream ss(ip);
        string segment;
        int count = 0;
        while (getline(ss, segment, '.')) {
            count++;
            if (segment.empty() || segment.size() > 3 || (segment.size() > 1 && segment[0] == '0')) return false;
            for (char c : segment) if (!isdigit(c)) return false;
            if (stoi(segment) > 255) return false;
        }
        return count == 4;
    }

    bool isIPv6(string ip) {
        int colons = 0;
        for (char c : ip) if (c == ':') colons++;
        if (colons != 7) return false;

        stringstream ss(ip);
        string segment;
        int count = 0;
        while (getline(ss, segment, ':')) {
            count++;
            if (segment.empty() || segment.size() > 4) return false;
            for (char c : segment) {
                if (!isxdigit(c)) return false;
            }
        }
        return count == 8;
    }
};
