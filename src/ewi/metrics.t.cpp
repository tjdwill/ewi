// metrics.t.cpp
#include "metrics.hpp"

#include <cassert>
#include <iostream>

#include <Eigen/Eigen>

#include <ewi/record.hpp>
#include <ewi/entry.hpp>

// imports 
using namespace std::chrono_literals;
using namespace ewi;
using Date = std::chrono::year_month_day;


// Helper functions
void test_ewi_calc();
void test_mean_calc();
void test_plot_ewi();
void test_record_metrics();

int main()
{
    test_record_metrics();
    test_mean_calc();
    test_ewi_calc();
    test_plot_ewi();
}


namespace 
{
    static std::vector<double> METRICS { 0.0, 1.0, 5.0, 6.0 };
    // Global date vector for instantiation
    std::vector<Date> const dates
    {
        Date{ 2024y / std::chrono::April / 25d },
        Date{ 2024y / std::chrono::October / 26d },
        Date{ 2025y / std::chrono::September / 27d },
        Date{ 2028y / std::chrono::October / 28d },
    };


    auto entry_from(Date d) -> Entry
    {
        return Entry(d, "", METRICS);
    }

    auto gen_record(int num_entries) -> Record
    {
        std::vector<Entry> entries {};
        for (int i=0; i<num_entries; i++)
            entries.push_back(entry_from(dates[i]));
        return Record(entries);
    }
}


void test_record_metrics()
{
    std::cout << "\n<test_record_metrics>\n---------------------" << "\n";

    assert( !(get_record_metrics(Record {})) );

    Record r = gen_record(static_cast<int>(dates.size()));
    auto metrics = get_record_metrics(r);
    assert(metrics);
    assert(static_cast<int>(metrics.value().rows()) == r.size());
    assert(static_cast<int>(metrics.value().cols()) == r.metric_dim());
    std::cout << "The Record:\n" << r << "\n";
    std::cout << "Record metrics:\n" << *metrics << "\n";
}

void test_mean_calc()
{
    std::cout << "\n<test_mean_calc>\n----------------" << "\n";

    Record r = gen_record(static_cast<int>(dates.size()));
    auto metrics = get_record_metrics(r);
    assert(metrics);

    auto means = get_record_means(metrics.value());
    /// All entries in the defined record have the same metrics, so the means should equal
    /// the original values.
    auto baseline = Eigen::Map<Eigen::Vector<double, 4> >(METRICS.data());
    assert(means.isApprox(baseline));


    auto rowwise_means = get_record_means(*metrics, false);
    /// The METRICS vector has a mean of 3.
    assert(rowwise_means.isApprox(Eigen::VectorXd::Ones(METRICS.size()) * 3));
    
    // Printouts
    std::cout << "Means (col-wise): " << means.transpose() << "\n";
    std::cout << "Means (row-wise): " << rowwise_means.transpose() << "\n";
    
}

void test_ewi_calc()
{
    std::cout << "\n<test_comparison_calc>\n----------------------" << "\n";

    Eigen::VectorXd global_means { { 1, 4.5, 0, 2, 10 } };
    Eigen::VectorXd local_means { { 2, 2.35, 4, 2.5, 100 } };

    std::cout << calculate_ewi(local_means, global_means).transpose() << "\n";

    // Test divide by zero method
    Eigen::VectorXd zeros = Eigen::VectorXd::Zero(5);
    Eigen::Vector<double, 5> locals { { -2, -1, 0, 1, 2 } };
    
    constexpr double BASE { 1.5 };
    Eigen::Vector<double, 5> check 
    { 
        {
            -std::pow(BASE, 2),  
            -std::pow(BASE, 1),
            1, 
            std::pow(BASE, 1),
            std::pow(BASE, 2) 
        }
    };
    auto compare = calculate_ewi(locals, zeros);
    std::cout << "Zero Check: " << compare.transpose() << "\n";
    assert(compare.isApprox(check));
}

void test_stl_conversion()
{
    /*
    Eigen::MatrixXd m 
    {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
    };
    */
    Eigen::VectorXd m { { 0, 1, 2, 3, 4, 5, 6, 7, 8} }; 

    std::vector<double> check 
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8
    };

    assert(to_std_vec(m) == check);
    std::cerr << "Vec Size: " << to_std_vec(m).size() << "\n";
}

void test_plot_ewi()
{
    std::cout << "\n<test_plot_ewi>\n---------------" << "\n";

    PlotCustomization opts
    {
       "test_ewi.png",
       "Test Plot Export",
    };
    Eigen::VectorXd zeros = Eigen::VectorXd::Zero(5);
    Eigen::Vector<double, 5> locals { { -2, -1, 0, 1, 2 } };
    auto eigen = calculate_ewi(locals, zeros);
    auto data = to_std_vec(eigen);

    assert(data.size() > 0);

    assert(plot_ewi(data, opts));
}
