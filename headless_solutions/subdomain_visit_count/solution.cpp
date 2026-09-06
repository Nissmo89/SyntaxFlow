#include <vector>
#include <string>
#include <unordered_map>

class Solution {
public:
    std::vector<std::string> subdomainVisits(std::vector<std::string>& cpdomains) {
        std::unordered_map<std::string, int> counts;
        for (const std::string& cd : cpdomains) {
            int spaceIdx = cd.find(' ');
            int count = std::stoi(cd.substr(0, spaceIdx));
            std::string domain = cd.substr(spaceIdx + 1);
            
            counts[domain] += count;
            for (int i = 0; i < domain.size(); ++i) {
                if (domain[i] == '.') {
                    counts[domain.substr(i + 1)] += count;
                }
            }
        }
        std::vector<std::string> result;
        for (auto const& [domain, count] : counts) {
            result.push_back(std::to_string(count) + " " + domain);
        }
        return result;
    }
};
