#include "LSystemProcessor.h"

// Map containing illegal strings as keys, and their corrosponding legal string as values.
// // For use in correcting lsys variables and rulesets.
LSystemProcessor::LSystemProcessor(std::atomic<float>& generations)
    : generations(generations) {}

// Main function to generate and run everything necessary to produce an L system.
void LSystemProcessor::generateLSystem(const uint8_t& gens)
{
    DBG("Begin Generating L System------");
    DBG("Translating rulesets/vars");
    translateSet(current_lsysRulesets);
    translateSet(current_lsysVars);
    std::unordered_map<std::string, std::string> newLsysRulemap = generateRuleset();
    LSystem lsystem(lsysAxiom, newLsysRulemap);
    lsystem.generate(generations);
    lSystems.push_back(lsystem);

    //FOR TESTING PURPOSES=============================================================
    const LSystem& testLSystem = lSystems[0];
    notesPool = generateNotesPool(testLSystem.genString);
}

// We need to verify that all variables in l system are single characters for future rule application.
// Searches through each string in the set and replaces illegal substrings with legal ones using the replacementRules map.
void LSystemProcessor::translateSet(juce::SortedSet<std::string>& stringSet)
{
    juce::SortedSet<std::string> translatedRuleset;
    // Iterates through each line in the ruleset.
    for (const std::string& ruleLine : stringSet)
    {
        // Iterates through each key, value pair in map.
        std::string newRuleLine = ruleLine;
        for (const auto& [key, value] : replacementRulesToChar)
        {
            // Replace any instance of the key (illegal string) in the ruleLine with the value (legal string)
            size_t pos = 0;
            while ((pos = newRuleLine.find(key, pos)) != std::string::npos)
            {
                newRuleLine.replace(pos, key.length(), value);
                pos += value.length();
            }
        }
        translatedRuleset.add(newRuleLine);
    }
    stringSet = std::move(translatedRuleset);
}

// Converts the final string generated by the lSystem into a vector<int> representing the interval in half steps from the root note (axiom).
// Example: "3" represents a major 3rd, so it's converted to 4 (half-steps from the root).
std::vector<int> LSystemProcessor::generateNotesPool(const std::string& genString)
{
    std::vector<int> notesPlus;
    for (char c : genString)
    {
        DBG("Char: " << c);
        std::string s{ c };
        auto it = replacementRulesToInt.find(s);
        if (it != replacementRulesToInt.end())
        {
			DBG("Adding " << it->second);
            notesPlus.push_back(it->second);
        }
        else
        {
            DBG("REPLACEMENT RULES AND L STRING MISMATCH");
            return notesPlus;
        }
    }
    DBG("Done generating. Returning notesPlus");
    return notesPlus;
}

// Generates ruleset map from ruleset set.
std::unordered_map<std::string, std::string> LSystemProcessor::generateRuleset()
{
    DBG("Generating Ruleset");
    std::unordered_map<std::string, std::string> map;
    for (const std::string& ruleLine : current_lsysRulesets)
    {
        size_t equalsPos = ruleLine.find('=');
        std::string leftHandSide = ruleLine.substr(0, equalsPos);
        std::string rightHandSide = ruleLine.substr(equalsPos + 1);
        map.emplace(leftHandSide, rightHandSide);
    }
    return map;
}

void LSystemProcessor::saveLSystem(LSystem lSystem)
{
    // TODO: Implement saving feature
}