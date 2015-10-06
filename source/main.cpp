/******************************************************************************/
/*!
\file   main.cpp
\author Jack Chang
\date   09/27/2015
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
    Implementation of 1 R algorithm.

    The format of input data is slightly modified, so it is easier to read
    Input data is split into training data and testing data
    Testing data is saperated from original input data as [name]_Predict.csv
*/
/******************************************************************************/
//#include <vld.h>        // Visual Leak Detect for VS2013
#include <iostream>     // cout
#include <fstream>      // ifstream
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

// Type Definition
typedef std::vector<std::string> attrtable;
typedef std::vector<std::unordered_map<int, std::string>> datatable;
typedef std::pair<int, std::string> data;
struct dataCmpr {
  bool operator()(const data &lhs, const data &rhs) {
    return lhs.second < rhs.second;
  }
};
typedef std::unordered_map<std::string, std::unordered_map<std::string, int>> ruleboard;
typedef std::unordered_map<std::string, std::string> rules;

// Prototype Definition
void oneR(const char* learning_data, const char* predict_data);
int readData(const char * filename, attrtable& at, datatable& dt);
int processData(attrtable& at, datatable& dt, rules& out_rules, int& out_attr_idx);
void readPredictData(const char * filename, attrtable& at, int attribute_idx, rules& rule);

int main(void)
{
  std::cout << "================= Weather Data =================" << std::endl;
  oneR("assets/WeatherData.csv", "assets/WeatherData_Predict.csv");
  std::cout << std::endl;

  std::cout << "================= Voting Data =================" << std::endl;
  oneR("assets/VotingData.csv", "assets/VotingData_Predict.csv");
  std::cout << std::endl;

  std::cout << "================= Animal Data =================" << std::endl;
  oneR("assets/AnimalData.csv", "assets/AnimalData_Predict.csv");
  std::cout << std::endl;

  return 0;
}

void oneR(const char* learning_data, const char* predict_data)
{
  // Read Data
  attrtable attributes;
  datatable data;
  int dataSize = readData(learning_data, attributes, data);

  // Find Rule
  rules weather_rule;
  int weather_attr_idx;
  int correct_count = processData(attributes, data, weather_rule, weather_attr_idx);

  // Output Result
  std::cout << "Using Attribute: " << attributes[weather_attr_idx] << std::endl;
  std::cout << "Total Errors: [" << (dataSize-correct_count) <<"/" << dataSize << "] "<< std::endl;
  std::cout << "=====================Rules=====================" << std::endl;
  for (auto r : weather_rule)
  {
    std::cout << r.first << " -> " << r.second << std::endl;
  }
  std::cout << "==============================================" << std::endl;

  std::cout << "==================Prediction==================" << std::endl;
  readPredictData(predict_data, attributes, weather_attr_idx, weather_rule);
  std::cout << "==============================================" << std::endl;
}

int readData(const char * filename, attrtable& at, datatable& dt)
{
  int id = 0;

  // Open file
  std::ifstream infile(filename, std::ifstream::in);
  std::string line;
  if (infile.fail()) {
    std::cout << "Failed to read file." << std::endl;
    return id;
  }    

  // Read Attributes
  std::getline(infile, line);
  auto start = line.begin();
  auto end = line.begin();
  while (end != line.end())
  {
    if (*end == ',')
    {
      at.push_back(std::string(start, end));
      start = ++end;
    }
    else
    {
      ++end;
    }
  }
  if (start != end)
  {
    at.push_back(std::string(start, end));
  }
  // Should zero initialize the vector
  dt.resize(at.size());

  // Read Data
  while (std::getline(infile, line))
  {
    int idx = 0;
    start = line.begin();
    end = line.begin();
    ++id;

    while (end != line.end())
    {
      if (*end == ',')
      {
        dt[idx].emplace(id, std::string(start, end));
        ++idx;
        start = ++end;
      }
      else
      {
        ++end;
      }
    }
    if (start != end)
    {
      dt[idx].emplace(id, std::string(start, end));
    }
  }

  return id;
}

int processData(attrtable& at, datatable& dt, rules& out_rules, int& out_attr_idx)
{
  //  Rules
  int max_idx = -1;
  int max_correct = -1;
  
  // For each attribute
  for (int i = 0; i < (int)at.size() - 1; ++i)
  {
    // For checking unique options for each attribute
    std::set<std::string> unique;

    ruleboard errorboard;

    // For each value of that attribute
    // Count how often each class appears
    for (auto value : dt[i])
    {
      // C++11 23.4.4.3 map element access [map.access]
      // If no key found, the int value is zero initalized
      ++errorboard[dt[dt.size() - 1][value.first]][value.second];
      unique.insert(value.second);
    }
    // Find the most frequent class
    rules cur_rule;
    int total_correct= 0;
    for (auto value : unique)
    {
      int max = -1;
      std::string classification;

      for (auto e : errorboard)
      {
        if (e.second[value] > max)
        {
          max = e.second[value];
          classification = e.first;
        }
      }
      total_correct += max;
      // Make the rule assign that to this attribute value
      cur_rule.emplace(std::string(value), std::string(classification));
    }

    // Choose the rules with largest correct rate (same as smallest error rate)
    if (max_correct < total_correct)
    {
      max_idx = i;
      max_correct = total_correct;
      out_rules.clear();
      out_rules = rules(cur_rule);
    }
  }
  // We found the best attribute that has largest correct rate, 
  // return the count of correct
  out_attr_idx = max_idx;
  return max_correct;
}

void readPredictData(const char * filename, attrtable& at, int attribute_idx, rules& rule)
{
  std::ifstream infile(filename, std::ifstream::in);
  std::string line;
  if (infile.fail()) {
    std::cout << "Failed to read file." << std::endl;
    return;
  }

  // Read Data
  while (std::getline(infile, line))
  {
    std::vector<std::string> instance;
    auto start = line.begin();
    auto end = line.begin();

    while (end != line.end())
    {
      if (*end == ',')
      {
        instance.push_back(std::string(start, end));
        start = ++end;
      }
      else
      {
        ++end;
      }
    }
    if (start != end)
    {
      instance.push_back(std::string(start, end));
    }
    // Read the rule and Out the result
    unsigned i;
    for (i = 0; i < at.size() - 1; ++i)
    {
      std::cout << at[i] << " [" << instance[i] << "] ";
    }
    std::cout << at[i] << " [" << rule[instance[attribute_idx]] << "] " << std::endl;
  }
}
