// survey.hpp
#ifndef INCLUDED_SURVEY
#define INCLUDED_SURVEY

#ifndef INCLUDED_RECORD
#include "record.hpp"
#endif 

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_EIGEN
#include <Eigen/Eigen>
#define INCLUDED_EIGEN
#endif


namespace ewi
{

    /// Emotion Survey Related things
    // This could be a class, but I'm making it a namespace
    // because I don't need to create an object.
    namespace Emotion 
    {
        /// Represents the results of a human-factored survey that aims to ascertain
        /// emotional components that influence perceived workload.
        /// Values range from 1 to 5.
        /// Assumes the median (normal) value of each question is three.
        
        static std::vector<std::string> SURVEY {
            "How much confidence do you have in your direct leadership?",
            "How passionate are you about the department's current mission?",
            "How much do you think your work challenges you?",
            "In general, how peaceful is your life?", 
            "My target deadlines are easy to hit."
        };
        // Define survey answer extremes
        // If you change the range, ensure there are an odd number of values
        // such that the mean is a whole number.
        int constexpr MIN_VAL = { 1 };
        int constexpr MAX_VAL { 5 };
        int constexpr IDEAL_MEAN { (MIN_VAL + MAX_VAL)/2 };
    } // namespace Emote

    /// The output of the parsed job file.
    /// Contains:
    ///     - Role ID
    ///     - The role-specific questions
    ///     - Estimated Averages for each question
    struct ParsedProfile
    {
        public:
            JobID d_role_id;
            std::vector<std::string> d_questions;
            MetricStats d_averages;  // The average value for each metric.

            // CONSTRUCTORS
            explicit ParsedProfile (
                    JobID role_id,
                    std::vector<std::string> questions,
                    MetricStats averages
            ) noexcept
                : d_role_id{ std::move(role_id) }, d_questions{ std::move(questions) }, d_averages{ std::move(averages) }
            {}
            ParsedProfile() = delete;

            // METHODS

            // Returns amount of quantitative questions
            inline auto metric_cnt() const -> int { return (int) d_questions.size(); }
    };

    /// Parses the data provided by the role file
    /// (*.role?, *.jrl)
    ParsedProfile load_profile(std::string const& file_path);


    /// Stores the answers to a given survey. At
    /// the time of writing, the results will be a std::vector of
    /// std::strings. The first element will be a std::string capturing
    /// the entry date. Entries 1->m are the answers to the
    /// numeric-based role questions. Finally, the last entry
    /// is extra information the user provides. 
    class SurveyResults
    {
        public:
            // CONSTRUCTORS
            SurveyResults(std::vector<std::string> const& responses, int metric_cnt)
                : d_metric_cnt{ metric_cnt }, d_responses{ std::move(responses) } {
                    // 3 is the minimum length of a valid survey result std::vector.
                    // It would be [date, metric_questions, extra_info]
                    assert((int) d_responses.size() > metric_cnt && metric_cnt > 0); 
                }

            // ACCESSORS
            inline auto get_responses() const -> std::vector<std::string> const& { return d_responses; }
            inline auto metric_cnt() const -> int const { return d_metric_cnt; }

            // METHODS
            std::vector<double> extract_metrics() const;

        private:
            int d_metric_cnt;            // How many questions were number-based?
            std::vector<std::string> d_responses;   // All of the form responses
    };

} // namespace ewi
#endif // INCLUDED_SURVEY
