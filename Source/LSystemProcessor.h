// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

//TODO: IMPLEMENT SAVING FEATURE!!!
// Should be able to save each lsystem into some sort of data format that can be shared across all instances
// of this plugin. Then you can browse and select a format that's already generated.
// Maybe even automate this. So you can combine L systems within the same thing.
class LSystemProcessor
{
public:
    LSystemProcessor(std::atomic<float>& generations);
    // L System Generation ====================================
    juce::SortedSet<std::string> current_lsysVars;
    juce::SortedSet<std::string> current_lsysRulesets;
    std::string current_axiom;
    std::vector<LSystem> lSystems;
    void generateLSystem(const uint8_t& gens);
    // ========================================================
    // APVTS Processor
    std::vector<int> generateNotePool(const std::string& genString);
    std::vector<int> notesPool;


private:
    std::atomic<float>& generations;
    std::unordered_map<std::string, std::string> replacementRulesToChar;
    std::unordered_map<std::string, int> replacementRulesToInt;
    void translateSet(juce::SortedSet<std::string>& stringSet);
    std::unordered_map<std::string, std::string> generateRuleset();
    void saveLSystem(LSystem lSystem);
};