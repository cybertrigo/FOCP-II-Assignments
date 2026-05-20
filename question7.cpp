#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
 
// Trim leading/trailing whitespace from a string
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}
 
// Replace every occurrence of placeholder in str with replacement
std::string replaceName(std::string str, const std::string& name) {
    const std::string placeholder = "{name}";
    size_t pos = 0;
    while ((pos = str.find(placeholder, pos)) != std::string::npos) {
        str.replace(pos, placeholder.length(), name);
        pos += name.length();
    }
    return str;
}
 
int main() {
    const std::vector<std::string> roasts = {
        // --- Brain / Smarts ---
        "Scientists studied {name}'s brain for years. Their conclusion? Needs more RAM.",
        "I asked {name} to spell 'smart'. They said, 'That's a trick question, right?'",
        "{name} tried to do a word search puzzle and reported the words missing.",
        "Fun fact: {name} once lost a debate with a traffic cone — and felt bad about it.",
        "If knowledge were sunscreen, {name} would be lobster-red from head to toe.",
 
        // --- Effort / School ---
        "{name}'s homework is so rare, museums have offered to display it.",
        "The Wi-Fi goes out once and {name} suddenly has nothing to do. Interesting.",
        "{name} studied for a blood test. Still failed.",
        "Autocorrect gave up on {name}. Even it has standards.",
        "{name}'s GPA and their bowling score are suspiciously close.",
 
        // --- Speed / Energy ---
        "A sloth challenged {name} to a race. The sloth is still waiting for {name} to show up.",
        "{name} moves so slowly that Google Maps added a '{name} walking' option — same ETA as continental drift.",
        "Scientists discovered a new unit of laziness. They named it '{name}' per hour.",
        "{name} set a new record for the 100-meter sprint — two hours and change.",
 
        // --- Pop culture / quirky ---
        "{name} once got lost on a straight road. Twice.",
        "A GPS once rerouted itself away from {name}'s decisions.",
        "{name}'s idea of multitasking is tripping over flat ground while chewing gum.",
        "If common sense were currency, {name} would need a loan.",
        "{name} asked Siri for directions to get their life together. Even Siri paused.",
 
        // --- Wholesome sting ---
        "I'm not saying {name} is an underachiever, but their participation trophy asked for a refund.",
        "{name}, the WiFi password is harder to figure out than you — and that password is 'password'.",
        "Plants lean *away* from {name} when they walk by. Even photosynthesis has limits.",
        "{name}'s search history is just 'how to look like you know what you're doing.'",
        "The dictionary added {name}'s name next to 'unpredictable' — because nobody expected this.",
    };
 
    // Seed with time * process noise for better variance on fast re-runs
    std::srand(static_cast<unsigned>(std::time(nullptr)) * 2654435761u);
 
    // --- Input (keep asking until a non-empty name is given) ---
    std::string name;
    std::cout << "=============================\n";
    std::cout << "  🔥  THE ROAST MACHINE 3000  🔥\n";
    std::cout << "=============================\n";
 
    while (true) {
        std::cout << "Enter your name: ";
        if (!std::getline(std::cin, name)) {
            // EOF / stream error — bail out gracefully
            std::cout << "\nCouldn't read input. Exiting.\n";
            return 1;
        }
        name = trim(name);
        if (!name.empty()) break;
        std::cout << "⚠️  Come on, type your name! We can't roast thin air.\n";
    }
 
    // --- Pick a roast, shuffle-style using Fisher-Yates partial shuffle ---
    std::vector<int> indices(roasts.size());
    std::iota(indices.begin(), indices.end(), 0);
    // One pass of the shuffle to pick the first slot unpredictably
    for (size_t i = roasts.size() - 1; i > 0; --i) {
        size_t j = std::rand() % (i + 1);
        std::swap(indices[i], indices[j]);
    }
    const std::string roast = replaceName(roasts[indices[0]], name);
 
    // --- Output ---
    std::cout << "\n🎤 Roasting " << name << " in 3... 2... 1...\n";
    std::cout << "\n   👉  " << roast << "\n\n";
    std::cout << "(No " << name << "s were harmed in the making of this roast.)\n";
 
    return 0;
}
