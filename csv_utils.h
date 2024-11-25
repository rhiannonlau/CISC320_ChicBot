// csv_utils.h
#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Enum Definitions
const std::vector<std::string> COLOR_ENUM = {
    "Red", "Blue", "Green", "Yellow", "Brown", "Black", "Orange", "Pink", "Purple", "White", "Grey", "None"
};

const std::vector<std::string> SIZE_ENUM = {
    "XXS", "XS", "S", "M", "L", "XL", "XXL", "None"
};

const std::vector<std::string> TYPE_ENUM = {
    "Shirt", "Pants", "Jacket", "Shorts", "Skirt", "Dress", "Sweater", "Coat", "Hat", "Socks", "Shoes", "Scarf", "Gloves", "Belt", "Underwear", "None"
};

const std::vector<std::string> BRAND_ENUM = {
    "Nike", "Adidas", "Puma", "Reebok", "UnderArmour", "NewBalance", "Gucci", "LouisVuitton", "Prada", "Chanel", "None"
};

const std::vector<std::string> MATERIAL_ENUM = {
    "Cotton", "Polyester", "Wool", "Silk", "Leather", "Denim", "Nylon", "Linen", "Suede", "Spandex", "None"
};

// Function Declarations
std::string concatenateArray(const json& json_array, const std::string& default_value = "None");
double safeStringToDouble(const std::string& str, double defaultValue = 0.0);
double parseDouble(const std::string& input);
std::string retrieveItems(const json& aiOutput, const std::vector<json>& csvItems);
std::vector<json> readCSVData(const std::string& filename);

// Function to build the JSON schema with enums
json buildResponseFormat();

#endif // CSV_UTILS_H

