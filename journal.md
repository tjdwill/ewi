# Design Journal: Employee Workload Index

## 19 October 2024

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

### Barebones Prototype

- Get data through survey forms
    - Role-related survey ("daily")
    - Emotional component survey (~weekly-ish)
- Role-related surveys can be overwritten in case of a mistake.
- EWI is week-based.

## 18 October 2024

I'm not going to get the design right on the first try, so let's just try something that can work.

I know I need:

- A Job profile
    - What are the common, measurable metrics associated with a given field or role?
    - What are the average statistics for these metrics?
- A way of recording and updating an employee's workload data over time.
- A report system

Rather than re-inventing the wheel as the first option, can I repurpose existing technology?

## 14 October 2024

### Evaluating Employee Workload

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
