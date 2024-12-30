// survey.hpp
// This component defines the tools necessary for eventually creating surveys
// and processing the results.
#ifndef INCLUDED_EWI_SURVEY
#define INCLUDED_EWI_SURVEY

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_EWI_BASIC_ID
#include <ewi/basic_id.hpp>
#endif

namespace ewi
{
    class Entry;
    struct Job 
    {
        BasicID id;
        std::string title;
    };

    // While Technical questions may change depending on job type, the questions for
    // evaluating the personal component do not (at least, I don't envision them being
    // variable in the program's final form).
    //
    // Therefore, host the necessary information in a struct.
    struct PersonalSurvey 
    {
        /// Represents the results of a human-factored survey that aims to ascertain
        /// emotional components that influence perceived workload.
        /// Values range from 1 to 5.
        /// Assumes the median (normal) value of each question is three.
        static auto questions() -> std::vector<std::string> const&
        {
            static const std::vector<std::string> questions =  {
                "How much confidence do you have in your direct leadership?",
                "How passionate are you about the department's current mission?",
                "How much do you think your work challenges you?",
                "In general, how peaceful is your life?", 
                "My target deadlines are easy to hit."
            };
            return questions;
        };
        // Define survey answer extremes
        // If you change the range, ensure there are an odd number of values
        // such that the mean is a whole number.
        static constexpr int MIN_VAL = { 1 };
        static constexpr int MAX_VAL { 5 };
        static constexpr int IDEAL_MEAN { (MIN_VAL + MAX_VAL)/2 };
    };

    /// The output of the parsed job file.
    /// Contains:
    ///     - Job Label (formal id number + human-readable title)
    ///     - The role-specific questions
    ///     - Estimated Averages for each question
    struct ParsedProfile
    {
        public:
            Job job_label;
            std::vector<std::string> questions;
            std::vector<double> averages;  // The average value for each metric.

            // Returns amount of quantitative questions
            inline auto metric_cnt() const -> int { return (int) questions.size(); }
    };

    /// Parses the data provided by the role file
    /// (*.role?, *.jrl, or regular ole *.txt)
    /// The first line of the file is taken to be the Job information (`formal_id: informal name`).
    /// After a blank line, the questions are processed. Each question lives on its own line with its estimated mean
    /// value. Because I have no insight into what metrics a given industry or job requires, few assumptions can be
    /// made regarding what values are valid. Therefore, the values are parsed as-is.
    auto load_profile(std::string const& file_path) -> ParsedProfile;


    /// Stores the answers to a given survey. At the time of writing, the results will be a
    /// std::vector of std::strings. The first element will be a std::string capturing the
    /// entry date. Entries 1..=(size - 2) are the answers to the numeric-based role questions.
    /// Finally, the last entry is extra information the user provides. 
    class SurveyResults
    {
        public:
            // CONSTRUCTORS
            SurveyResults(std::vector<std::string> const& responses, int metric_cnt);

            // ACCESSORS
            inline auto get_responses() const -> std::vector<std::string> const& { return d_responses; }
            inline auto metric_cnt() const -> int const { return d_metric_cnt; }
            auto extract_metrics() const -> std::vector<double> const;
            auto to_entry() const -> Entry;
        private:
            SurveyResults() = delete;
            int d_metric_cnt;  // How many questions were number-based?
            std::vector<std::string> d_responses;   // All of the form responses
    };

} // namespace ewi
#endif // INCLUDED_EWI_SURVEY
