# Design Journal: Employee Workload Index

## Quick Navigation

- [Journal](#journal)
- [Living Questions List](#living-questions) 
- [To-Do List](#ewi-todo)

---

## Journal

### 26 December 2024

- Decided to change the EWIUi tester to hold the EWIUi object within its layout. Now, I
  don't have to overload relevant display functions; Qt will handle it for me.
- Refactored `EmployeeRecordIOUtils` to allow individual entry exports. The idea is to
  provide the means to export newly created entries to a temp file for data loss
  prevention. This will need additional thought, but at the mechanism is there now.
- Had to change the survey response signal to include the survey type in the transmission.
- Implemented `ewiQt::EWIUi` about page.

### 25 December 2024

I realized that I forgot to include the personal survey results in the ewi calculation
plot. When thinking about how to integrate this feature, I realized my code needs some
refactoring. I think I can write `ewi::metrics` without the dependency on `ewi::record`.

I'm also considering refactoring some of the public interface to `ewi::Record`, but this
may not be a good idea. My trepidation was the use of custom types in the method
signatures, but the types are defined in the component itself; users that use `Record`
would have access to these types anyhow.

Instead, I will focus on writing an additional method that returns metrics from a Record.
My original thought process to exclude this makes little sense in hindsight, so I'm going
to add this feature.

**Update:** I was able to implement the changes, but I want to note here that I wound up
using a `const_cast` in order to get the parts playing nicely. Part of the motivation for
this refactor was to instantiate the `Eigen` data types via references to vectors instead
of copies. It makes little sense to have to copy all relevant metrics vectors during
retrieval only to have them copied again in the matrix construction. 

I learned about the use of `std::reference_wrapper` and helpers `std::ref()`/`std::cref`.
As it turns out, `Eigen` doesn't want to instantiate Vector types from `const` STL vector
references; it only worked with mutable references.  This was an issue because the entire
`Entry` and `Record` APIs deal with const references pretty much exclusively. Instead of
adding redundant getters and setters, leading to changing and extending multiple APIs, I
decided to simply cast the `const` away in the implementented `Record::metrics()` function.
I justify this by noting that metrics should never be modified, so there is no context in
which they should be changed directly. Also, the internal data member storing metrics are
non-const, so `const_cast` does not invoke undefined behavior.

Time will tell if this was a wise decision.


Also, I realized that I never included the data processing for personal surveys in my
metrics component, so I did so. The Personal Index is a single value ranging from [-1, 1].
It is calculated using the method I wrote about on [21 October 2024's
entry](21-october-2024).

### 23 December 2024

- Implemented a basic Qt ↔ STL type converter.
- Edited multiple files for increased style conformity
- Began draft for the app controller.

I will need to learn how to display images via Qt. I must also decide how I want the
visualization action to work in practice. Who should be in charge of displaying the image?
Exporting it to a file?

**Update:** Added a new class, `ewiQt::ImgViewer`, to handle the image display and export.
The idea is to pass a `QPixmap` via a signal from the controller and have the `EWIUi`
instantiate a viewer in a receiving slot.

### 22 December 2024

I was able to implement the proposed solution for the divide by zero error.
I also added the necessary `vcpkg` configuration in order to use `Matplot++`. Now, I need
to program the visualization. The first iteration will be simple. The idea is to have a
basline centered at one, and scatter points at discrete `(idx, ewi)` pairs.

**Update**: I was able to create a function that exports the EWI visualization to a file.

### 20 December 2024

So, it's been a month since my last entry, but I've been actively working. Specifically,
I've been experimenting with Qt for designing the application, and I've learned a
tremendous amount in the process. Now, I have a barebones application that works in
testing. The goal at this stage is to implement the controller for the app in order to
connect the interface logic with the business logic.

Doing this will require the controller to have the means to convert between Qt data
structures and STL data structures. This will likely incur a performance penalty, but
that shouldn't be too intense for the purpose of this application; it's not a high-speed
trading app, for instance.

Also, I still need to implement the metrics plotting by integrating `matplot++`. 

I am, however, concerned about the building and release of the application. Quite frankly,
I don't know what I'm doing. I don't know how to package dependencies nor do I know how to
make the build more flexible. For instance, currently the top-level `CMakeLists.txt` file
has to include the installation path to `Eigen` in order to build properly. I don't know
why that's the case.

#### Proposed Idea for Handling Divide by Zero

Based on my proposed metric comparison method, this situation arises when the global mean
for a given metric is zero. Ordinarily, floating point operations in which the denominator
is zero results in `inf` (ex. `1.0/0`) or `nan` (`0./0.0`). However, for the purposes on
this application, we actually want to extract information from the global average being
zero. 

For example, if heart surgeons have a global fatality average of 0, a given surgeon
having a local average above that could be cause for (great) concern. In other words, I
need a way to communicate a level of intensity based on the local average's distance from
zero. This intensity should grow quickly the further away from zero the value is.

I propose using an exponential function. The base is must lie between (1, 2) in order to
get a correct "feel" in terms of growth. I've selected 1.5. The idea is to do the
following:

    When global average is 0:

    EWI(x) = 
    {
        -(1.5^|x|), x<0

          1.5^x,    x>=0
    }

This way, the calculate value is still 1 if the local and global values are equivalent, and
we get rapid group as the local average trails away from 0.

### 15 November 2024

Let's regroup a bit. So far, I've been working on the infrastructure of my application,
creating data structures and functions that store user data, job data, and information
regarding metrics. I've created a file format to store the entire record of a given
employee and wrote functions to import from and export to said format. Finally, I created
a format that allows users to define the metrics associated with their given job. 

Now, I want to focus on actually calculating reportable data. I've [already
laid](#22-october-2024) some
[groundwork in this area](#21-october-2024), so let's continue along that track.

The entire point of this tool is that workload is difficult to measure because the metrics
vary depending on the industry and the roles *within* said industry. My idea is to have a
definable set of metrics for a job and over time collect numerous survey responses that
will ultimately enable comparing a given employee's workload (using technical and
personal/emotional metrics) to a global average. 

This proposed method makes sense to me due to how we implicitly evaluate workload. We tend
to compare our current or recent quantity of work with what we perceive as "normal".
Therefore, having a relative comparison seems more of a faithful representation of
workload than attempting to create a singular formula. However, I acknowledge that I may
change my mind as I continue this project and improve my understanding of mathematics and
model creation.


### 14 November 2024

Working job profile parser. This parser loads any type of file (may change that in the
future), but does expect a certain format:

```
JobCode: Informal Job Title

<Question 1 is asked here> | <Metric Estimate>  # don't include the angled brackets
```

As an aside, why doesn't C++ have the useful string algorithm functions like `trim` or
`split`?

### 13 November 2024

I'd planned to move on to a different portion of the program, but I ended up reworking the
API to the record types. Some of the functions are cleaner now, and I think the return
types are more uniform. In the process, I had a fun lesson in the use of `auto` and
implicit copies due to neglecting to specify `auto &`. That was fun to debug.

Soon, I want to figure out the survey component as well as the metrics. The latter will be
especially important because I will need to decide how to present workload data. In my
mind, I want a `Matplotlib`-style figure, but I'm not sure how to 1.) make that with C++ and
2.) display it via Qt. Qt has a data visualization module, but that's for Commerical and
GPLv3 licenses, so no, especially for the former.

I think I may look into the [`matplot++` library](https://github.com/alandefreitas/matplotplusplus). It's MIT licensed, and the plots I've seen
are beautiful.


### 12 November 2024

I've completed the parsing functions and wrote a test to validate them. In the process, I
had to implement comparison operators for `Record`, `WIRecord`, `EmployeeRecord`, and
`Employee`. This was to make testing easier by allowing equality comparisons between the
original and parsed `EmployeeRecord` objects.

I also wound up needing to add a dependency upon `StringFlattener` to `Entry` for printing
the notes on a single line. Also, I found a useful way to write a for loop with
heterogeneous-but-related values using `std::pair` (and the more general `std::tuple`).

We can write structured bindings in `for` loops. Check
`EmployeeRecordIOUtils::export_record`'s implementation in `employee_record.cpp`.


### 11 November 2024

Implemented parsing functions for `EmployeeRecordIOUtils`. Now, I need to write the
`EmployeeRecordIOUtils::import_record()` and `EmployeeRecordIOUtils::export_record()`
static methods. I also need to test `EmployeeRecordIOUtils::parse_employee()`.

Also, I changed the format for the EmployeeRecord file. Now, the metrics come last for any
given line.

### 10 November 2024

The goal for the `EmployeeRecord` parser is two-fold:

- Write an `EmployeeRecord` to file.
- Load `EmployeeRecord` from file.

To make the `EmployeeRecord` parser, I assume the following:

1. All functions must take either an `ifstream` or an `ofstream`.
2. Parsing functions each being with the payload on a non-whitespace character.
3. Any error will ultimately result in a `cpperrors::Exception`.

Performance Note:

Initially, I was going to structure the program such that I could simply append a line to
the employee rec file when a new Entry was created, which would be faster than overwriting
the entire file with a new export. However, I don't actually have any data that proves the
latter method would be slow. Therefore, I'm going to use the slower approach since it's
easier, but I'm making a note here in case I need to make a change. Basically, the parsing
function will have a per-entry focus instead of a `WIRecord` focus.

So, to be clear, the program will do a complete export of the `EmployeeRecord` every time a
write operation occurs.

**Update**:

I wrote a test to benchmark the performance for writing the entire `EmployeeRecord`. The
program takes in the number of lines to write to a file and subsequently writes a long
*lorem ipsum* string. I ran the test using multiple `N` values to simulate potential load.
Each `n` value is run five times using a `bash` script. Not sure if this is the "official" way
to test, but it serves as a good baseline benchmark. 

`n` cases:

- 1: Simulates the case in which we only append a single `Entry` to the file instead
  writing the entire `EmployeeRecord`
- 3650: Number of entries when recording two entries a day for five years. This is what I assume to be more
  "realistic" in terms of load.
- 36500: Recording two entries a day for 50 years. This is the upper extreme.

I'm testing using my Chromebook 14b-na0001nr with an AMD Athlon Silver 3050C with Radeon Graphics CPU.

| `n`   | Avg. Real Time (s) | Std. Deviation (s) |
| ---   | ------------------ | ------------------ |
| 1     |       0.0032       |      0.000748      |
| 3650  |       0.008        |      0.0021        |
| 36500 |       0.0198       |      0.00527       |

As we can see, even in the extreme case, the write operation should be on the order of
milliseconds, making the overwrite method a viable option.

#### Benchmark Script

```c++
// write_speed.t.cpp
// This test is meant to serve as a performance test for writing files.
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./write_speed <Num of lines to write>" << "\n";
        std::exit(1);
    }
    int num_lines { std::stoi(argv[1]) };
    // Generator: https://www.lipsum.com/
    std::string const long_line {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vel commodo lorem, eget commodo ligula. Pellentesque euismod interdum lorem, sit amet dapibus urna scelerisque quis."};

    std::ofstream ofs { "/tmp/write_speed.txt" };
    if (!ofs.is_open()) {
        std::cerr << "Could not open file." << "\n";
        std::exit(1);
    }
    for (int i {}; i < num_lines; ++i)
        ofs << long_line << "\n";

    if (!ofs) {
        std::cerr << "Write Error." << "\n";
        std::exit(1);
    }
}
```

```bash
$ for i in {0..4}; do time ./write_speed <n>; done
```

### 8 November 2024

Still working on the `EmployeeRecord` transmission mechanism. I now know how to compress a
string down to fit into a line, but now I want to flesh the mechanism fully. I think I may
need a parser struct that can import and export `EmployeeRecord`s. The good news is that
these files are meant to be created internally, so the parser can assume incoming files are
written correctly, throwing an error if not.

### 6 November 2024

I've been working on a lot without writing about it. So far, I've done a few refactors,
segmenting the codebase into components and modules. Currently, I'm working on the file
types and parsing operations described earlier in the journal. I need to review C++ I/O as
well as basic string manipulation. 

One goal for exporting `EmployeeRecord`s to file is to represent each `Entry` within a line
(one per line). To do so, I need to find a way to convert a potentially multi-line string
to a one-line string. I'm thinking of replacing all `\n` with `\1` since no one uses that
ASCII sequence. I wanted to use `\0`, but that makes the compiler think the string has
terminated (word to C-style strings).

### 1 November 2024

So, I was able to achieve the vast majority of my goals. Most importantly, I learned how to
run tests using CMake, which will greatly improve my workflow. I also learned more about
the system in general in terms of best practices, but CMake is definitely something that
requires time and experimentation to learn properly.

In terms of design, I made some small adjustments to the classes. `Entry` has had its Eigen
dependency removed. It now only concerns itself with std::vector<double>. For `Record`, I
removed the dependency on `MetricStats`. Now, I simply have it return an Eigen matrix that
can then be used to construct a MetricStats object. I did it this way, however, so that
neither `Record` nor `MetricStats` need know about the other. finally, I added some more
convenience methods and moved the greater record types to the draft space for now; I still
need to determine how to organize them into components.

### 27 October 2024

Lord willing, the next two weeks will be research and skill improvement focused. I
especially plan to concentrate on CMake to make the build process easier to complete.
Testing needs careful attention, so investing time up front to learn will have great payoff
down the road.

To summarize, here are my goals:

- [x] Read ~~two~~ eight chapters of the CMake book.
- [x] Write notes on const correctness and CMake
- ~~[ ] Skim Ch. 4 of Qt book.~~
- [x] Read various blog articles in my backlog.

### 26 October 2024

- Fixed `Record::find_entries` and passed all tests.
- Implemented many methods for `Record`.
    - `update_entry`, `add_entry`, `remove_entry`, `find_entry`
- Wrote `operator<<` overloads for `Entry` and `Record`.
- Learned that we can't return references from a `std::optional<T>`

### 25 October 2024

Worked on `Record::find_entries`. Found a bug. Need to find a way to organize my data such
that I can think of edge cases as normal cases.

### 24 October 2024

Worked to implement `Record::find_entries()` method.

### 23 October 2024

EEO Job code: 0260


### 22 October 2024

Ok, let's rethink the goal here. Ultimately, I want to write a piece of software that
allows employees to more adequately track and represent their workloads. Every mental
iteration of this idea has some visual component that allows employers to "see" the
employee's workload. I wanted to create a numeric index, Employee Workload Index (EWI),
that communicates in one number how intense the workload is. This number would depend on
two factors: an objective job-related component and a subjective emotional component. Both
are important because the latter colors the perception of the former. 

However, the job-related component also requires statistical data in order to calculate
its portion of the EWI. The idea is that the further the employee's workload metrics
deviate from the mean, the "stronger" the effect of the workload (or lack thereof). I have
no such data, though, nor do I know how to get it. This is because the metrics associated
with a given role aren't determined. Industries would need to converge on a uniform profile
for their respective fields, and then *that* collected data could be used as a mean.

Since I can't access that data, my method of reporting will need to change. As of now, I
cannot calculate EWI. That means the idea is on backburner. It'll still exist in the
codebase as a declaration (maybe I'll comment it out), but I won't use it until I can implement it.

For now, I think I should concentrate on a more visual means of communication. One thing I
could do is create double-bar graphs that compare the mean for a given metric to the user's
mean value for a given date range. The problem with this, however, is that the units
between metrics may vary, so the graph could be skewed by metrics with high default values.

Alternatively, I could produce one graph per metric, but this is also undesirable for cases
with a large quantity of metrics. What if I produced a graph that showed how each metric
deviates from the mean? For each metric, I could take the average for the user and subtract
it from the average of the metric. Then, I could divide this difference by the metric
average to determine how large the deviation is? This would enable a more uniform plot (and
also would be centered around zero as desired).

### 21 October 2024

#### Calculating EWI

The *Employee Workload Index* (EWI) is an estimate of how a given employee likely perceives
their workload. Given data regarding the employee's current workload, the role's average
workload, and information on the employee's emotional state, a score is calculated to
determine how burdensome the workload is.

The method of calculation is up to me. I need to create a formula that:

- Acknowledges that emotional state colors perception of workload intensity.
    - Inversely proportional
- Allows significant deviations from the norm to have great impact on the resulting score.

Ideally, I would have information regarding the standard deviation for a given metric, but,
as of now, I have no such data, and can't base the calculation off of that. However, this
shouldn't be a major issue because the implementation of the EWI calc can always be
adjusted.

So, let's create a prototype.

```
EWI = <Role Impact Score> + <Emotion Impact Score>
```

##### Emotional Impact

This aspect is tricky to handle properly. On one hand, a subpar emotional index should
amplify the role index in both directions.

Scratch that. I think I'll report the two elements separately. This would offload the
interpretation onto the viewer.

That being said, here's one way to calculate the emotional index.

Currently, the accepted values range from one to five with the baseline value being three
(normal) and an exceptionally positive emotional state being five. Regardless of the range,
however, it is imperative that there be an odd number of possible values. This makes the
average center around a whole number in a monotonoically-increasing sequence of values.

I want a scale that communicates how widely an employee's emotional state deviates from the
norm:

```
 1  2 3 4 5
      _     // average

-2 -1 0 1 2
      _     // centered around the average

-1 -0.5 0 0.5 1 
        _   // normalized
```

So, a given procedure could be:

0. Subtract all scores by the ideal average.
1. Sum all scores.
2. Divide by (Max\_Value * # of responses)

This results in a normalized score whose center is 0 (neutral).

#### Data Formats

- Entry
    - Date
    - Metric Data 
    - Extra Notes
- Record
    - Collection of entries
- CompleteRecord
    - Job-based Data Record
    - Emotion-based Data Record
- EmployeeRecord
    - Identifier
    - Collection of CompleteRecords mapped to JobCodes
- JobCode
    - Identifier for a given job position

Note: I don't really need to worry about the size of a given person's record. Assuming 365
entries a year for 50 years, the person would accumulate 18,250 entries. Most people don't
work for that long at one company, and they certainly don't work every day for that period.

#### File Formats

**Employee Record**

Vision: text file with identification information + space-delimited records. Must be easy
to append to.

```
# Employee Record file
## Identification Region
Name: <HERE>


---
## Records
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
...
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
```

The Notes string must be enclosed within quotation marks and MUST live on a single line. To
do so, newline sequences will be converted to escape sequences (i.e. `U+0020` -> `\\n`)

**Survey Profile**

Vision: text file allowing the job-specific questions to be specified.

See the format in [yesterday's post](#20-october-2024)

#### Statistical Data

I'm going to need to be able to compare the user's answers with the average values for each
question. However, I need the averages to do so. Should I make a class that represents the
statistics for a role? The problem is that, for a newly created job profile, there's no
data. I *could* allow estimates for a given role, but I would lack information on how the
data varies among the set.

I only need this data when generating the EWI.

### 20 October 2024

#### Dynamic Questions

Let's say we want to define a role. We can do so via a text file of the following format:

```
# <ROLE TITLE>: <some unique ID>

<Question/Metric Here> | <average value>
<Question/Metric Here> | <average value>
<Question/Metric Here> | <average value>
...
<Question/Metric Here> | <average value>
//empty line or EoF
```

Notes: 

- Ignore leading and trailing whitespace
- `|` is the delimiter between the metric and the average value
- All entries are assumed to have associated numeric answers.
- Each non-empty line corresponds to a metric.
- Duplicates are not checked.


In theory, I could then parse this file (should I create a file extension? `.role`) and extract the
questions and averages.

**Question**: How could I update averages? Figure it out later.

#### Flow

With this method of allowing profiles to be created, I can envision the following flow:

- Load user and role profiles; parse role file
- Prompt user to create a new entry; Generate form from parsed questions
- Get survey results; convert to numeric data.
- Store entry data
- Give Emotion survey if necessary (based on time since last completion)


### 19 October 2024

I assume a given employee's records are only valid for the company they are currently employed by.
Knowing that the questions used to evaluate workload will vary according to the role the employee
occupies, there must be a way to contain records of survey responses over time for each role. One
method of tracking the survey responses over time is to use a map of key-value pairs. The keys would
be the date of a given entry (valid because dates YYYY-MM-DD are unique on a per-person, per-role
basis), and the values would be the remaining entry data. 

I could take this map even further out, using this map of entries as values in an outer hash map whose keys
are job role IDs. Typically, a job will have some unambiguous code associated with it within a company, so
a given employee could have:

```
Michael = {
    "role_id_0": {
        "2024-01-01": <all of the data for this entry>,
        "2024-01-02": <all of the data for this entry>,
        ...
    },
    "manager_position": {
        "2024-09-04": <all of the data for this entry>,
    }
}
```

However, does a given data entry look like? 
- How do I allow the entry questions to change as a function of the role?
- How do I keep the averages in sync with the entry questions?

#### Barebones Prototype

- Get data through survey forms
    - Role-related survey ("daily")
    - Emotional component survey (~weekly-ish)
- Role-related surveys can be overwritten in case of a mistake.
- EWI is week-based.
- Static averages
- Only one profile

### 18 October 2024

I'm not going to get the design right on the first try, so let's just try something that can work.

I know I need:

- A Job profile
    - What are the common, measurable metrics associated with a given field or role?
    - What are the average statistics for these metrics?
- A way of recording and updating an employee's workload data over time.
- A report system

Rather than re-inventing the wheel as the first option, can I repurpose existing technology?

### 14 October 2024

#### Evaluating Employee Workload

In order to get the data I need, I want the following:

**Technical**

- Date (look into std::chrono)
- Time Spent: Job-related correspondence 
- Time Spent: Meetings/Trainings
- Quantity: Interviews observed
- Quantity: Extrapositional requests fielded
- Quantity: Active cases; (Weekly recording?) ~~- Quantity: Contacts made (Weekly recording?)~~

**Human/Emotional Component**

The following questions are rated from one to five, and the assumed median value is, of course,
three.

- Confidence in leadership
- Investment/Interest in department mission
- How challenging do you find your work?
- How peaceful is your life?


I'm honestly not sure how to structure this data. Should a given entry be a type? What is the domain
language for this application's business logic?

---

## Living Questions

Here are some questions that have come up as I work this project.

### Language

- [x] How do I compare two Eigen objects?
    - use the `isApprox()` method. The default delta for doubles is 1e-12.
- [ ] When do I specify `noexcept`?
- [x] East-style const?
- [x] `class` vs. `struct`: semantic meanings?
- [x] How are boolean comparision operator overloads implemented? What is the spaceship operator `<=>`?
- [x] Difference: Initialization vs. Assignment

### Software Design/Project Management

- [x] Is it worth it to test *every* class (ex. a class of basic getters and setters?)
    - Personally, I don't think so, unless the getter or setter is more involved than
      simply returning a reference to some inner object.
- [x] When do I need to create a physical dependency vs. use a forward declaration?
    - This was a misguided question because even if you *do* only use a forward
      declaration, the physical dependency on the component defining that entity still
      exists. What I really wanted to know was when to add an include directive in a
      component header. That's answered in Section 6.3.7 of *Large Scale C++ Software
      Design* (Lakos 1996).
- [x] Is it wise to use a namespace-wide enum class that serves as an error type?
    - No, because you create a project-wide dependecy. What happens when you need to modify
      the enum, either by renaming something or removing? You'd have to change *every*
      single use across the project.

---

## EWI ToDo

### Implementation

- [x] `ewi/metrics.cpp`
- [x] `ewi/employee_record.cpp`
- [x] `ewi/survey.cpp`
- [ ] Front-end (UI, front-end/back-end coordinator)
    - [ ] Add basic Quick Help page.
    - [ ] Implement slots for the app controller.

### Learning

- Eigen
    - [ ] Eigen semantics
    - [ ] Instantiation, comparison, arithmethmetic operations 
    - [ ] Manipulation, basic and advanced access
- Qt
    - [x] Ch. 4 of Book
    - [x] Read fundamentals in docs
- CMake Book
    - [x] Learn to write tests
    - [x] How to structure a project
    - [x] Compiling components
- Notes Updates
    - [x] East-style const (basics)
    - [x] Function declarations using `auto` and `->`
    - [ ] Important keywords (`explicit`, `noexcept`, `protected`, `friend`, `private/public`, `inline`, `static`, `constexpr`, `const`)
    - [ ] Making a third-party library visible
