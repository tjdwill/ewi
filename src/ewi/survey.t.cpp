#include "survey.hpp"

#include <cassert>
#include <chrono>
#include <cpperrors>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <ewi/entry.hpp>
#include <utils/string_flattener/string_flattener.hpp>


using cpperrors::Exception;
using ewi::SurveyResults;
using namespace std::chrono_literals;

void test_metrics_extraction();
void test_to_entry();
void test_profile_loader(std::string const& path);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: test_survey some_path" << "\n";
        std::exit(1);
    }
    std::string path {argv[1]};
    try 
    {
        test_metrics_extraction();
        test_profile_loader(path);
    }
    catch (Exception const& e) 
    {
        std::cerr << e.report() << "\n";
        throw;
    }
}


void test_metrics_extraction()
{
    std::vector<std::string> two_metrics {"Some Date", "2", "1.5", "Some Notes"};
    std::vector<std::string> invalid_metrics {"Some Date", "2 ", "1.5 ", "Some Notes"};

    SurveyResults two_test (two_metrics, 2);
    auto metrics = two_test.extract_metrics();
    assert(metrics.size() == 2);
    assert(metrics[0] == 2.0 && metrics[1] == 1.5);

    SurveyResults invalid(invalid_metrics, 2);
    try {
        invalid.extract_metrics();
    } catch (Exception const& e) {
        std::cout << "Caught the Exception." << "\n";
    }
}


void test_profile_loader(std::string const& path)
{
    std::string EEO_CODE {"0260"};
    std::string EEO_TITLE { "NETC EEO Informal Process Counselor" };
    ewi::ParsedProfile job_profile = ewi::load_profile(path);

    auto const& job_info = job_profile.job_label;
    assert(job_info.id == EEO_CODE && job_info.title == EEO_TITLE);
    assert(job_profile.questions.size() == job_profile.averages.size());
    assert(job_profile.questions.size() == 3 && job_profile.questions.size() == job_profile.metric_cnt());
}

void test_to_entry()
{
    std::string NOTES { "Here are some\nnotes" };
    
    std::chrono::year_month_day DATE { 2024y / std::chrono::December / 30d };
    std::ostringstream oss {};
    oss << DATE;
    std::string DATE_STR { oss.str() };
    
    std::vector<std::string> RESULTS { DATE_STR, "2", "1.5", NOTES };
    ewi::Entry TEST { DATE, utils::StringFlattener::flatten(NOTES), {2, 1.5}};

    assert(TEST == SurveyResults(RESULTS, 2).to_entry());
}
