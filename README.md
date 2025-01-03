# Employee Workload Index (EWI) Tracker

Copyright &copy; 2025 Terrance Williams

EWI Tracker is an application developed to aid users in tracking and
visualizing their workload(s).

## Background

Communicating one's workload is a difficult task due to the large number of
variables and external factors influencing it. Typically, companies attempt to
estimate workload through performance metrics—output is typically the sole
factor informing the perception of an employee's workload. However,
non-performance-based metrics contribute to one's workload as well. For
example, the number of emails, phone calls, and meetings one participates in
can take up a substantial amount of one's workday, even if no tangible
performance metric results from the effort. Another factor in one's workload,
though rarely discussed, is how much time one spends on tasks outside of his or
her official role. It is no uncommon for employees to help peers, juniors, and
even superiors with their work, especially if the employee in question is a
high performer. This volume of work may not be reflected in official
performance metrics (ex. helping a peer revise a presentation, troubleshooting
an everyday task, or advising during urgent, department-related matters).

Another factor affecting the perception of workload is one's emotional
disposition. While this is sometimes acknowledged in passing (ex. "company
morale"), it is difficult to establish a standard method of evaluation. This
difficulty is exacerbated by:

1. Varying levels of emotional intelligence/empathy among leaders.
2. Trepidation on part of the employee due to fear of reprisal (direct or
   indirect).

However, it is important to explicitly acknowledge this component. Someone who
is emotionally fulfilled/invested in their job would likely perceive a high
technical workload in a more positive light when compared to a more
dissatisfied or outright unhappy state. In an especially positive state, the
employee may even *enjoy* the challenge associated with such a load. On the
other hand, for an employee who is in a less fulfilled state, it is possible
that even a normal workload would be perceived as too much. 


With the above in mind, this application attempts to provide a way for
employees to track both aspects of workload perception. Though it's difficult
to objectively quantify the concept of workload, this program uses a two-part
index that the author calls the *Employee Workload Index* (EWI) to estimate
workload intensity. The visualization produced by the program can then be used
to give an imprecise-but-useful indication of how a given employee likely
perceives their current workload.

### Employee Workload Index (EWI)

EWI is a simple model the author created to aid in workload visualization. The
idea is to have two aspects of workload contribution—both a vocation-based
*technical* aspect and an emotion-based  *personal* aspect—and use them to
visualize the given employee's workload. It is important to note that this
application does *not* provide a mathematically-proven heuristic for evaluating
workload. Instead, it uses information provided by the user to provide a
helpful approximation.

The technical aspect of EWI is the more objective of the two. When people
evaluate someone's workload, they do so by implicitly comparing the workload to
some ground truth or "normal" workload. In other words, technical workload is
viewed in this application as a relative concept. The program allows the user
to define a series of metrics associated with a job and specify an estimated
global average for each metric. Then, the user tracks their values for these
metrics over time. Upon request, the program then compares the user's local
averages with the global averages.

As an example, let's consider a role in which daily phone call duration
contributes to technical workload. Say the global average for the role is 30
minutes (0.5 hr) per day. If Jane Doe averages 2 hours of calls per day for a
given calendar period, the program will report that Janes has 4x the average
workload for this given metric. Similarly, if Jane averages 0 minutes, the
program will report the workload for this metric as 0. Since the author is not
prescient enough to know what metrics a given industry may want to define,
negative values are also allowed. However, the program was written with
non-negative numbers in mind.

The personal aspect is simpler. The user is asked a number of pre-determined
questions that aim to assess their emotional satisfaction with their current
role. These questions evaluate confidence in leadership, investment in
company/department mission, and more. Each question requires a response ranging
from 1 to 5 (inclusive). The responses are averaged and normalized to produce a
single value ranging from -1 to 1 (incl.). Whether the calculated average falls
on the positive or negative end of 0 determines how "satisfied" the user is.
This value can then be used to help interpret how the user may perceive their
workload found in the technical portion. Also, unlike the technical aspect in
which indexes are defined on a per-metric basis, the personal index is one
number. The author made this decision in order to help maintain employee
privacy. In other words, even if the personal aspect index is negative, a given
employer or superior wouldn't be able to know *which* questions were answered
negatively. The personal index is also intended to encourage leaders to
communicate with those they lead to help identify unmet needs or the aspects
that work well for that employee.

## Getting Started

Whether using this application on Windows or Linux, the `gnuplot` software is
required to generate the EWI plots. 

For Windows users, the `gnuplot` software distributors provide [a simple
installer accessed here](https://sourceforge.net/projects/gnuplot/). Download
the latest version to keep setup simple. When running the installer, keep the
default options until reaching the "Select Additional Tasks" page. On this
page, scroll to the bottom and make sure the `Add application directory to your
PATH environment variable` option is *checked* (see the image below). It is
important to do so in order for this program to communicate with `gnuplot`. If
you install `gnuplot` without checking this option, simply uninstall it and run
through the installation process again.

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/gnuplotInstallation.png" alt="gnuplot Installation with PATH option enabled" />
</p>

&nbsp;

Linux users can either find `gnuplot` via their package manager or build from
source.

On Fedora Linux:

```bash
$ sudo dnf install gnuplot
```

On Ubuntu/Debian:

```bash
$ sudo apt install gnuplot  # This should work, but is untested by the author
```

Ensure the version is at least `5.2.6`.

Finally, Linux users must have access to the Qt5 binaries to run this
application. At the time of writing, the author is unfamiliar with distribution
for Linux (and, admittedly, is also inexperienced for Windows), so it may be
easier to build this application from source.

After that, run the `ewiTracker` executable to get started.

### Quick Tour

The `EWI Tracker` navigation bar shows all of the options available to the
user:

- `Home`: The Home Page
- `Profile`: Load the profiles necessary to use the app
- `Actions`: Record entries and/or produce visualizations
- `Help`: A "Quick Help" menu to get the user up and running
- `About`: Information about this application.
- `Exit`: Exit the application, saving any updated changes in the process.

<div align="center">

Nav Bar (Action Button Disabled) | Nav Bar (Action Button Enabled)
:-------------------------------:|:--------------------------------:
![Nav Bar](https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/navBar_actionDisabled.png) |  ![Nav Bar (Action Enabled)](https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/navBar_actionEnabled.png)

</div>

## Profile Menu

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/profileMenu.png" alt="Profile Menu" />
</p>

The `Profile` menu houses options for the app to retrieve information necessary
for its operation. The user must provide both a user profile and a job profile
for the app's actions to become enabled. The user profile consists of basic
information: a user ID and a display name. The ID is intended to be a unique
identifier (ex. your employee ID). The display name can be any name the user
chooses, but be aware that this name appears on the visualization image.

The job profile consists of the identifier (job code + human-readable title)
and a list of questions and metrics for tracking. More on the job profile will
be discussed in a later section.

### Creating a User Profile

In order to create a profile for use, click the `Create` button located under
the "User Profile" heading. A window will appear requesting information from
the intended user. Once both fields are occupied with text, the "Ok" button
appears. Pressing this button will create the user.

**Note:** In order to save your newly-created profile, ensure you shut down the
program via the navigation bar's `Exit` button once you're done with the
session. If this is not done, the program will not be able to locate the user
on the next run. In general, make it a habit to use the `Exit` button.

### Loading a User Profile

To load a previously-created user, click the `Load...` button under the "User
Profile" heading and input the user's ID.

### Loading a Job Profile

To load a job profile, select the `Select Job...` button under the "Job
Profile" heading. This will open a file selection dialog, allowing the user to
select a job profile that's been defined. The program expects profiles to be
stored in its `.jobs` directory, but job files may be loaded from anywhere.

### Defining a Job Profile

A job file is simply a text file formatted in a specific way. The first line
defines the job's identifier—its formal code and human-readable name. The
format is:

```
<job code>: <human-readable name>
```
without the angle brackets. Spaces are allowed in the human-readable name. By
"human-readable", I mean the job's title. For example, one role in a restaurant
may have a job code `5555` but the "human-readable" name would be "Head Chef".

A blank line should follow the identifier line.

The rest of the document defines the metrics to track for the role. One metric
is defined per line. A given line is formatted as:

```
<Metric question> | <estimated average>
```

again, no angled brackets. The `|` character is typed by `SHIFT + \` on a
typical QWERTY keyboard. Each metric has a question that is presented to the
user when making an entry (ex. "How many calls were made?"). It is good
practice to include the expected units in parentheses (see the example below).

The estimated average is the value that will be used to compare the user's
tracked data for that metric. The program expects a number for this value. `-`
signs are allowed. Decimal values are also allowed.

To summarize, a job profile consists of the following:

- an identifier
- blank space
- Metric question-average pairs.

Here is an example of a fully-formed job profile:

```
0260: NETC EEO Informal Process Counselor

Time spent on communication (hr)? | 3
Active cases? | 2
Informal contacts? | 2
```

This results in the following entry survey:

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/exTechSurvey.png" alt="Example Survey" />
</p>

Finally, job profiles can be loaded from anywhere, but it may be easier to
store them with the program itself. Save the file to the `<EWITracker
location>/.jobs` folder. If this folder is not present, try running the
application to ensure the program creates the folder. If this folder is still
not visible, ensure `View Hidden Items` is enabled on whatever file explorer
you are using. If all options fail, manually create the folder in the
application's directory.

**Caution**: Unfortunately, once data for a job profile is recorded, the job
profile cannot be changed. This is due to inexperience on the author's part as
it was unclear how to handle the case where the number/order of questions is
changed. This will hopefully be solved in a future iteration.

### Changing Profiles

A user can switch profiles by running through the desired process again. Simply
load another job and/or user. If the user is changed by either loading or
creating another profile, the previous profile is saved before the new profile
is loaded into the program.


## Action Menu

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/actionMenu.png" alt="Action Menu" />
</p>

The `Action` menu houses the operations relevant to recording and reporting
metrics. The first heading on the page allows the user to take surveys. To
record technical metrics, take the technical survey. For personal metrics, take
the personal survey. The program will then open a form that the user will fill
out. Technical forms will present the questions defined in the job profile.
Each field only allows numeric input with the exception of "Additional Notes",
a field that allows the user to record any additional information that may be
relevant to that entry.

**Note**: In the future, the author plans to implement an action that allows
the user to an entry from a specified date. At the time of initial release, the
user can "cheat" by viewing the desired entries in their file located in the
`.usr` directory.

### Taking a Survey

Taking a survey is as easy as clicking the desired survey button. The form is
programmed such that it will only allow submission once all *metric* fields are
populated. The "Additional Notes" field can be empty. It is important to note
that the program expects and enforces chronologically-ordered entries. In other
words, each technical entry *must* have a date that is later than the previous
entry. If the user intends to submit multiple entries (say if they hadn't been
able to access their computer for some time), the entries should be made from
earliest to latest date. The same rule applies to personal entries. However,
technical and personal entries are independent of one another; the program does
not enforce chronological ordering between the two. 

The frequency with which surveys are taken depends on both the user and the job
profile definition. Some metrics may be written to be recorded daily; others
may have a longer timescale. Personal surveys are intended to be taken weekly,
but the program does not enforce this. Frequency is at user discretion. 

### Viewing EWI

To view the EWI report, click the `View Workload...` button under the "Metrics"
heading. The program will then ask for the desired date range for the report.
It is an inclusive range, so if both the `From` and `To` fields have the same
date, the program will generate a report for that day if an entry exists for
it. Depending on your machine, the plot may take some time to display.

The following is an example report based on contrived metric values for the job
defined earlier in this document:

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/main/test_resources/ewiQt/test_ewi.png" alt="Example EWI Report">
</p>

Let's discuss how to interpret the plot. If the user only recorded technical
data for a given date range, only the technical plot will be displayed. If the
user had entries for both technical and personal data within the date range,
the program will output a two-part plot. The left portion will have the
technical data, and the right will have the personal data. If there is no
technical data at all for that date range, no plot is generated.

#### Technical Plot

The technical plot consists of a baseline at `y=1` and a series of scatter
points. The baseline represents the global average for each metric; since each
metric can have varying value scales, the program normalizes each metric such
that the technical index is 1. 

A given scatter point `(x, y)` represents `(Question num, Technical Index)`,
where question number `1` corresponds to the first question on the job profile.
The `Technical Index` (Idx) is the workload approximation. A `y` value of 4 for
example means the user has 4x the average expected workload for that metric.
Knowing this, we can see visually how much the user's workload deviates from
the norm at a glance for each metric.

#### Personal Plot

The right side of the two-part plot displays the Personal Index. This plot is
simple to interpret; where does the non-black point fall on the vertical line?
If it is above the black point, the user's responses indicated a positive
disposition with intensity increasing the closer the index is to 1. If below
the black point, the responses indicated a negative disposition. Finally, if
the non-black point completely overlaps the black point (Idx=0), the responses
indicated that the user is exactly neutral. 

The user can save the generated visualization to their file system via the
`Export` button.

## Exporting Data 

At the time of writing, there is no functional export feature for this app; the
user must do it manually. 

In short:

1. Locate where the program executable is located.
2. In your file explorer, ensure `View Hidden Items` is enabled.
3. Copy the `.usr` folder to backup users and the `.jobs` folder to save jobs.

---

## Dependencies

- cpperrors (0.1.0)
- Eigen (3.4.90)
- Matplot++ (1.2.1)
    - gnuplot (at least 5.2.6) 
- Qt (5.15.2)

### Linux Build Deps

- CMake (3.30.5)
- vcpkg (2024-11-12-eb492805e92a2c14a230f5c3deb3e89f6771c321)
- Clang (19.1.5)
- gcc (14.2.1)
- Ninja (1.12.1)

### Windows Build Deps

- Visual Studio 2022 (Community Edition)
 