# OneR CPP
Simple implementation of One R algorithm for Machine Learning.

## Pseudocode
```
For each attribute,
  For each value of that attribute, make a rule as follows:
    count how often each class appears
    find the most frequent class
    make the rule assign that class to this attribute value.
  Calculate the error rate of the rules.
Choose the rules with the smallest error rate.
```

## Compile & Run
Require Visual Studio 2013 / 2015

- Run **_build_vs2013.bat** to generate VS2013 project
- Run **_build_vs2015.bat** to generate VS2015 project

## Sample Output
```
================= Weather Data =================
Using Attribute: Outlook
Total Errors: [4/14]
===================== Rules ====================
Overcast -> yes
Rainy -> yes
Sunny -> no
================================================
================== Prediction ==================
Outlook [Overcast] Temperature [Cool] Humidity [High] Windy [FALSE] Play [yes]
Outlook [Sunny] Temperature [Cool] Humidity [Normal] Windy [TRUE] Play [no]
================================================
```
