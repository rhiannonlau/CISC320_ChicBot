// csv_utils.cpp
#include "csv_utils.h"
#include <nlohmann/json.hpp>
#include <numeric> // For std::accumulate

// Function to safely concatenate array elements into a comma-separated string
std::string concatenateArray(const json& json_array, const std::string& default_value) {
    if (!json_array.is_array() || json_array.empty()) {
        return default_value;
    }

    // Handle case where the array contains only "None" or similar placeholders
    bool all_none = true;
    for (const auto& elem : json_array) {
        if (elem.get<std::string>() != "None" && elem.get<std::string>() != "Invalid") {
            all_none = false;
            break;
        }
    }
    if (all_none) {
        return default_value;
    }

    // Concatenate array elements into a comma-separated string
    std::vector<std::string> elements = json_array.get<std::vector<std::string>>();
    return std::accumulate(
        std::next(elements.begin()), elements.end(), elements[0],
        [](const std::string& a, const std::string& b) -> std::string {
            return a + ", " + b;
        }
        );
}

double safeStringToDouble(const std::string& str, double defaultValue) {
    try {
        size_t processed;
        double result = std::stod(str, &processed);
        // Check if the entire string was processed
        if (processed == str.length()) {
            return result;
        }
    } catch (const std::exception&) {
        // Handle conversion errors
    }
    return defaultValue;
}

double parseDouble(const std::string& input) {
    // Create a copy of the input string with only numeric characters and '.'
    std::string cleaned;
    for (char c : input) {
        if (std::isdigit(c) || c == '.') {
            cleaned += c;
        }
    }

    // Convert the cleaned string to a double
    return cleaned.empty() ? 0.0 : std::stod(cleaned);
}

// Updated retrieveItems function to return a formatted string
std::string retrieveItems(const json& aiOutput, const std::vector<json>& csvItems) {
    std::stringstream output;
    bool anyMatch = false;

    for (size_t i = 0; i < aiOutput["clothing_items"].size(); ++i) {
        const auto& item = aiOutput["clothing_items"][i];

        // Safely concatenate array fields
        std::string color = concatenateArray(item["color"]);
        std::string size = concatenateArray(item["size"]);
        std::string type = concatenateArray(item["type"]);
        std::string brand = concatenateArray(item["brand"]);
        std::string material = concatenateArray(item["material"]);

        // Access max_price and min_price safely
        std::string max_price = item.value("max_price", "None");
        std::string min_price = item.value("min_price", "None");

        output << "\nMatching Items:\n" << std::endl;

        bool foundMatch = false;

        // Iterate through CSV items to find matches
        for (const auto& csvItem : csvItems) {
            bool isMatch = true;

            // Check each attribute for a match
            auto checkAttribute = [](const json& aiAttr, const json& csvAttr) {
                if (aiAttr.empty() || (aiAttr.size() == 1 && aiAttr[0] == "None")) {
                    return true;
                }
                for (const auto& value : aiAttr) {
                    if (value == "None") continue;
                    for (const auto& csvValue : csvAttr) {
                        if (value == csvValue) return true;
                    }
                }
                return false;
            };

            if (!checkAttribute((item["color"]), (csvItem["color"]))) isMatch = false;
            if (!checkAttribute((item["size"]), (csvItem["size"]))) isMatch = false;
            if (!checkAttribute((item["type"]), (csvItem["type"]))) isMatch = false;
            if (!checkAttribute((item["brand"]), (csvItem["brand"]))) isMatch = false;
            if (!checkAttribute((item["material"]), (csvItem["material"]))) isMatch = false;

            // Check price range if specified
            if (max_price != "None" && min_price != "None") {
                // **Modified Lines Start Here**
                // Split the get and parse into two steps to avoid compiler confusion
                std::string min_price_str = item["min_price"].get<std::string>();
                double minPriceVal = parseDouble(min_price_str);

                std::string max_price_str = item["max_price"].get<std::string>();
                double maxPriceVal = parseDouble(max_price_str);
                // **Modified Lines End Here**

                double csvPrice = safeStringToDouble(csvItem["min_price"].get<std::string>());

                if (csvPrice < minPriceVal || csvPrice > maxPriceVal) {
                    isMatch = false;
                }
            }

            // If all criteria match, add to results and append to output
            if (isMatch) {
                if (!foundMatch) {
                    output << "Found Match:" << std::endl;
                    foundMatch = true;
                }
                anyMatch = true;
                output << "  Name: " << csvItem.value("name", "N/A") << std::endl;
                output << "  Description: " << csvItem.value("description", "N/A") << std::endl;
                output << "  Color: " << concatenateArray(csvItem["color"]) << std::endl;
                output << "  Size: " << concatenateArray(csvItem["size"]) << std::endl;
                output << "  Type: " << concatenateArray(csvItem["type"]) << std::endl;
                output << "  Price: " << csvItem["min_price"] << std::endl;
                output << "  Brand: " << concatenateArray(csvItem["brand"]) << std::endl;
                output << "  Material: " << concatenateArray(csvItem["material"]) << std::endl;
                output << "  Quantity: " << csvItem.value("quantity", 0) << std::endl;
                output << "--------------------------------------" << std::endl;
            }
        }

        if (!anyMatch) {
            output << "No matching items found for these criteria." << std::endl;
        }
        output << "--------------------------------------" << std::endl;
    }

    return output.str();
}

// Function to read CSV data and convert to JSON objects
std::vector<json> readCSVData(const std::string& filename) {
    std::vector<json> items;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return items;
    }

    std::string line;
    // Skip header if it exists
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        // Parse CSV line using ',' as the delimiter
        while (std::getline(ss, cell, ',')) {
            // Trim whitespace from token
            cell.erase(0, cell.find_first_not_of(" \t")); // Left trim
            cell.erase(cell.find_last_not_of(" \t") + 1); // Right trim
            row.push_back(cell);
        }

        // Skip incomplete rows
        if (row.size() < 9) continue;

        // Create JSON object in the same format as clothing_info["clothing_items"]
        json item;

        // Convert single values to arrays to match the format
        item["color"] = json::array({row[2]});  // color
        item["size"] = json::array({row[3]});   // size
        item["type"] = json::array({row[4]});   // type

        // Convert price to min/max price strings
        double price = std::stod(row[5]);
        item["min_price"] = std::to_string(price);
        item["max_price"] = std::to_string(price);

        item["brand"] = json::array({row[6]});    // brand
        item["material"] = json::array({row[7]});  // material

        // Additional fields that might be useful but aren't in the clothing_info format
        item["name"] = row[0];
        item["description"] = row[1];
        item["quantity"] = std::stoi(row[8]);

        items.push_back(item);
    }

    // No need to create a separate JSON structure here; the function returns a vector of JSON objects
    return items;
}

// Function to build the JSON schema with enums
json buildResponseFormat() {
    json response_format;
    response_format["type"] = "json_schema";
    response_format["json_schema"]["name"] = "clothing_info";
    response_format["json_schema"]["strict"] = true;

    // Define the schema properties
    json schema;
    schema["type"] = "object";
    schema["properties"]["clothing_items"] = {
        {"type", "array"},
        {"items", {
                      {"type", "object"},
                      {"properties", {
                                         {"color", {
                                                       {"type", "array"},
                                                       {"items", {
                                                                     {"type", "string"},
                                                                     {"enum", COLOR_ENUM}
                                                                 }}
                                                   }},
                                         {"size", {
                                                      {"type", "array"},
                                                      {"items", {
                                                                    {"type", "string"},
                                                                    {"enum", SIZE_ENUM}
                                                                }}
                                                  }},
                                         {"type", {
                                                      {"type", "array"},
                                                      {"items", {
                                                                    {"type", "string"},
                                                                    {"enum", TYPE_ENUM}
                                                                }}
                                                  }},
                                         {"max_price", {{"type", "string"}}},
                                         {"min_price", {{"type", "string"}}},
                                         {"brand", {
                                                       {"type", "array"},
                                                       {"items", {
                                                                     {"type", "string"},
                                                                     {"enum", BRAND_ENUM}
                                                                 }}
                                                   }},
                                         {"material", {
                                                          {"type", "array"},
                                                          {"items", {
                                                                        {"type", "string"},
                                                                        {"enum", MATERIAL_ENUM}
                                                                    }}
                                                      }}
                                     }},
                      {"required", {"color", "size", "type", "max_price", "min_price", "brand", "material"}},
                      {"additionalProperties", false}
                  }}
    };

    schema["required"] = {"clothing_items"};
    schema["additionalProperties"] = false;

    response_format["json_schema"]["schema"] = schema;

    return response_format;
}
