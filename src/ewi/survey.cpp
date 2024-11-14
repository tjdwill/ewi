// survey.cpp
#include "survey.hpp"
#include "ewi/basic_id.hpp"

#include <cassert>
#include <cstring>
#include <cpperrors>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using cpperrors::Exception;

namespace {

    // https://stackoverflow.com/a/217605

    // trim from start (in place)
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
}

namespace ewi
{

    SurveyResults::SurveyResults(std::vector<std::string> const& responses, int metric_cnt)
        : d_metric_cnt{ metric_cnt }, d_responses{ std::move(responses) } 
    {
        // 3 is the minimum length of a valid survey result std::vector.
        // It would be [date, metric_responses, notes]
        assert((int) d_responses.size() > metric_cnt && metric_cnt > 0);
        assert((int) d_responses.size() > 2); // must have at least three elements
    }

    auto SurveyResults::extract_metrics() const -> std::vector<double> const
    {
        std::vector<double> metrics {};

        // Only the first and last elements of the responses vector are non-numeric.
        // We skip those as a result.
        for (auto it = d_responses.begin() + 1; it != d_responses.end() - 1; ++it)
        {
            char* rest {};
            auto const& s = *it;
            double val { std::strtod(s.c_str(), &rest) };
            if (std::strlen(rest) != 0)
            {
                std::ostringstream msg {};
                msg << "Invalid metric value: " << rest << "\n";
                throw Exception(msg.str());
            }
            else
                metrics.push_back(val);
        }
        return metrics;
    }

    auto load_profile(std::string const& file_path) -> ParsedProfile
    {
        constexpr char JOB_SEP {':'};
        constexpr char METRICS_SEP {'|'};
        // Stream setup
        std::ifstream file {file_path};
        if (!file.is_open())
            throw Exception("Could not open file: " + file_path );
        std::istringstream line_ss {};
        std::ostringstream str_builder {};
        std::string line {};

        // Job info
        std::string id {}; 
        std::string title {};

        // Parse the job id
        std::getline(file, line);
        if (!file) 
        {
            if (file.eof())
                throw Exception("File ended prematurely. Check the formatting");
            else
                throw Exception("File read error.");
        } else if (line.empty())
            throw Exception("Invalid profile format. File must begin with job id and title.");

        line_ss.str(line);
        char c {};
        while (line_ss >> c)
        {
            if (c == JOB_SEP)
                break;
            str_builder << c;
        }
        id = str_builder.str();
        if (!line_ss || id.empty())
            throw Exception("Incorrect file format. Job code must be followed by a colon `:`");

        // Get job title (human-readable job name)
        str_builder.clear();
        str_builder.str(std::string());
        
        line_ss >> std::ws;
        line_ss >> std::noskipws;
        while (line_ss >> c)
        {
            str_builder << c;
        }
        line_ss >> std::skipws;
        assert(line_ss.eof());
        title = str_builder.str();

        Job job { BasicID{id}, title };
        // Skip blank line(s)
        char check {};
        while (true)
        {
            line_ss.clear();
            std::getline(file, line);
            if (!file)
            {
                if (file.eof())
                    throw Exception("File ended prematurely. Check the formatting");
                else
                    throw Exception("File read error.");
            }
            line_ss.str(line);
            // Chew up whitespace by attempting to read a character. This works due to the
            // default behavior of an istream to skip whitespace.
            if (line_ss >> check)
                break;
        }

        // Parse questions and metric averages
        // Questions come first, followed by the separator and the metric value.
        std::vector<std::string> questions {};
        std::vector<double> averages {};
        std::string question {};
        std::string value {};
        char* remaining_str {};
        while (!line.empty())
        {
            line_ss.clear();
            line_ss.str(line);
            std::getline(line_ss, question, METRICS_SEP);
            // Get rid of trailing whitespace.
            // Source: https://stackoverflow.com/a/217605
            trim(question);
            if (!(line_ss >> value))
                throw Exception("Question must be followed by an estimated average.");

            questions.push_back(question);
            double val = std::strtod(value.c_str(), &remaining_str);
            if(!remaining_str || std::string(remaining_str) == value)
                throw Exception("Invalid metric value. Must be a number.");
            averages.push_back(val);

            std::getline(file, line);
            if (!file && !file.eof())
                throw Exception("File read error.");
        }
        return ParsedProfile { job, questions, averages };
    }
} // namespace ewi
