/*
 * Student Excuse Generator v2.0
 * -------------------------------------------------------
 * Improvements over v1:
 *  - 25 highly realistic, story-driven excuse templates
 *  - Composite RNG seed (time + clock) for better variety
 *    between rapid successive runs on the same second
 *  - Supports names with spaces via std::getline throughout
 *  - First-name extraction for natural mid-sentence use
 *  - Both {name} (full) and {first} (first name) placeholders
 *  - Excuse shown with a wrapped word-count footer
 *  - "Generate another?" interactive loop
 *  - Shuffle-without-repeat: tracks last index so the same
 *    excuse never appears twice in a row
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <iomanip>

// ── helpers ──────────────────────────────────────────────

std::string trim(const std::string& s) {
    const std::string ws = " \t\r\n";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

std::string firstName(const std::string& fullName) {
    size_t sp = fullName.find(' ');
    return (sp == std::string::npos) ? fullName : fullName.substr(0, sp);
}

// Replace every occurrence of `key` in `tmpl` with `value`
std::string replaceAll(std::string tmpl,
                       const std::string& key,
                       const std::string& value) {
    size_t pos = 0;
    while ((pos = tmpl.find(key, pos)) != std::string::npos) {
        tmpl.replace(pos, key.size(), value);
        pos += value.size();
    }
    return tmpl;
}

std::string fillTemplate(const std::string& tmpl,
                         const std::string& fullName,
                         const std::string& first) {
    std::string out = replaceAll(tmpl,  "{name}",  fullName);
    out             = replaceAll(out,   "{first}", first);
    return out;
}

// Word-wrap at `width` columns
std::string wordWrap(const std::string& text, size_t width = 72) {
    std::istringstream words(text);
    std::string word, line, result;
    while (words >> word) {
        if (!line.empty() && line.size() + 1 + word.size() > width) {
            result += line + "\n";
            line = word;
        } else {
            if (!line.empty()) line += ' ';
            line += word;
        }
    }
    if (!line.empty()) result += line;
    return result;
}

// Composite seed: combine wall-clock seconds with high-res tick count
unsigned int makeSeed() {
    auto now  = std::chrono::high_resolution_clock::now();
    auto tick = static_cast<unsigned int>(
        now.time_since_epoch().count() & 0xFFFFFFFF);
    return static_cast<unsigned int>(std::time(nullptr)) ^ tick;
}

// ── main ─────────────────────────────────────────────────

int main() {
    std::srand(makeSeed());

    // 25 realistic, story-driven excuse templates
    // Placeholders: {name} = full name, {first} = first name only
    const std::vector<std::string> templates = {

        // 1 — tech disaster
        "Dear Teacher,\n\n"
        "{name} respectfully submits this explanation for the missing assignment. "
        "After completing every question, {first} went to save the document and "
        "witnessed the dreaded spinning wheel of doom. The laptop froze, "
        "restarted without warning, and the auto-recovery file turned out to be "
        "a blank page. {first} stayed up an extra hour attempting to rewrite the "
        "work from memory, but exhaustion won. The assignment will be resubmitted "
        "by end of day.\n\n"
        "Sincerely,\n{name}",

        // 2 — medical (realistic)
        "Dear Teacher,\n\n"
        "I am writing on behalf of {name} to explain their absence from today's "
        "submission deadline. {first} experienced a severe migraine that began "
        "Sunday evening and continued through the night, making it impossible to "
        "read or focus on a screen. {first} has taken medication and is feeling "
        "better now. The completed homework will be handed in at the start of "
        "tomorrow's class.\n\n"
        "Thank you for your understanding,\n{name}",

        // 3 — family emergency
        "To Whom It May Concern,\n\n"
        "Please excuse {name} for not submitting the assignment on time. Late "
        "Sunday afternoon our family received an urgent call and had to drive "
        "to the hospital to be with a relative. {first} had every intention of "
        "finishing the work that evening, but the circumstances made that "
        "impossible. We returned home past midnight. {first} will submit the "
        "completed work as soon as possible and is happy to discuss an extension.\n\n"
        "Respectfully,\n{name}",

        // 4 — public transport + rain
        "Dear Teacher,\n\n"
        "{name} would like to apologise for arriving without the printed "
        "assignment today. The bus ran twenty minutes late due to a road closure, "
        "and by the time {first} reached the stop the weather had turned. The "
        "downpour was heavy enough that the papers in {first}'s open bag were "
        "soaked through and the ink bled beyond legibility. {first} has a digital "
        "copy ready and can email it immediately or reprint it during lunch.\n\n"
        "Apologies again,\n{name}",

        // 5 — younger sibling chaos
        "Dear Teacher,\n\n"
        "I'm writing to explain why {name} doesn't have the homework today. "
        "{first} finished it last night and left it on the kitchen table to "
        "dry after underlining key points. This morning, {first}'s five-year-old "
        "sibling decided to 'help' by colouring over every page in thick marker. "
        "By the time {first} noticed, it was too late to rewrite everything before "
        "the bus arrived. {first} will redo the assignment tonight.\n\n"
        "Thank you,\n{name}",

        // 6 — power cut during printing
        "Dear Teacher,\n\n"
        "{name} sincerely apologises for the incomplete submission. The assignment "
        "was finished and sent to the printer just after 10 p.m. when a "
        "neighbourhood power cut interrupted everything. By the time power was "
        "restored — nearly an hour later — it was too late to reprint and still "
        "get enough sleep before school. {first} has the digital file ready to "
        "share right now if that is acceptable.\n\n"
        "Kind regards,\n{name}",

        // 7 — lost USB / cloud sync failure
        "Dear Teacher,\n\n"
        "Please excuse {name} for not being able to hand in the assignment today. "
        "{first} saved everything onto a USB drive, but when it was plugged in "
        "this morning the drive was not recognised on any computer — not at home "
        "and not on the library machines. The cloud backup failed to sync because "
        "the Wi-Fi dropped out during the upload. {first} is actively working to "
        "recover the file and will resubmit before the end of the school day.\n\n"
        "Thank you for your patience,\n{name}",

        // 8 — wrong file attached (email submission)
        "Dear Teacher,\n\n"
        "{name} realised only this morning that the wrong file was attached to "
        "last night's submission email. In the rush to meet the deadline {first} "
        "accidentally sent an old draft instead of the finished version. The "
        "correct, completed document is attached to this message. {first} is "
        "sorry for any confusion and will double-check attachments in future.\n\n"
        "Regards,\n{name}",

        // 9 — unexpected sports commitment
        "Dear Teacher,\n\n"
        "I am writing on behalf of {name}. {first} was selected as a late "
        "replacement for the inter-school athletics team yesterday and did not "
        "receive notice until the afternoon. The competition ran until 8 p.m., "
        "followed by a team debrief and a long journey home. By the time {first} "
        "arrived, it was nearly 10 p.m. and there was simply not enough time to "
        "complete the assignment to an acceptable standard. {first} will have it "
        "ready by tomorrow morning.\n\n"
        "Thank you for your understanding,\n{name}",

        // 10 — internet outage (online submission)
        "Dear Teacher,\n\n"
        "{name} wants to flag that the assignment was completed on time but "
        "could not be uploaded to the portal. Our home broadband went down "
        "yesterday evening and the ISP confirmed a fault in the area that wasn't "
        "fixed until early this morning. {first} tried tethering to a mobile "
        "phone, but the signal was too weak to upload the file. Please find it "
        "attached to this email as proof of completion.\n\n"
        "Apologies for the inconvenience,\n{name}",

        // 11 — burnt the paper (cooking mishap)
        "Dear Teacher,\n\n"
        "This may sound unusual, but please hear {name} out. {first} finished "
        "the written portion of the assignment and placed the papers on the "
        "kitchen counter while making a late dinner. A pot boiled over and, in "
        "the scramble to deal with it, a tea towel landed on the papers near the "
        "hob. By the time {first} smelled the smoke, two sheets were scorched "
        "beyond reading. There was no time to rewrite them before school. {first} "
        "will have a clean copy ready by this afternoon.\n\n"
        "Sincerely,\n{name}",

        // 12 — caring for a sick parent
        "Dear Teacher,\n\n"
        "I hope you will excuse {name} for the late submission. {first}'s parent "
        "fell ill unexpectedly last night and {first} was the only one home to "
        "help. Between calling the doctor, getting to a pharmacy, and staying up "
        "to make sure everything was stable, there was no opportunity to sit down "
        "and work. {first} is committed to submitting the assignment as soon as "
        "possible and would appreciate any flexibility you can offer.\n\n"
        "With appreciation,\n{name}",

        // 13 — stolen bag
        "Dear Teacher,\n\n"
        "{name} is writing to report that their school bag was stolen from the "
        "bus this morning. The completed assignment, along with all textbooks and "
        "stationery, was inside. {first} has already reported the incident to the "
        "school office and the bus company. A police report number is available "
        "if required. {first} will rewrite the assignment as quickly as possible "
        "and asks for a short extension given the circumstances.\n\n"
        "Thank you,\n{name}",

        // 14 — flooding in bedroom
        "Dear Teacher,\n\n"
        "Please excuse {name} for not having the assignment today. A pipe burst "
        "in the ceiling above {first}'s bedroom last night and the room flooded "
        "while {first} was asleep. The homework papers were on the desk and were "
        "completely water-damaged. The family spent most of the night dealing with "
        "the emergency. {first} will resubmit as soon as possible and can provide "
        "confirmation from a parent if necessary.\n\n"
        "Apologies,\n{name}",

        // 15 — allergic reaction (realistic)
        "Dear Teacher,\n\n"
        "{name} was unable to finish the assignment due to an unexpected allergic "
        "reaction that started Sunday evening. {first}'s eyes swelled and were "
        "too irritated to read or write comfortably. After taking antihistamines "
        "{first} fell asleep earlier than planned and could not complete the "
        "remaining questions. {first} is feeling much better today and will "
        "finish the work during study period.\n\n"
        "Kind regards,\n{name}",

        // 16 — car breakdown on the way to school
        "Dear Teacher,\n\n"
        "{name} is sorry to arrive without today's printed work. The family car "
        "broke down on the way to school and {first} had to wait roadside for "
        "over an hour before a relative could arrange alternative transport. The "
        "printed assignment is at home; however, {first} has the digital version "
        "saved on their phone and can share it immediately via email.\n\n"
        "Thank you for your patience,\n{name}",

        // 17 — funeral attendance
        "Dear Teacher,\n\n"
        "Please excuse {name} from today's deadline. {first} had to attend a "
        "family funeral over the weekend, and the travel, arrangements, and "
        "emotional weight of the occasion made it impossible to focus on "
        "schoolwork. {first} had every intention of completing the assignment "
        "on Saturday, but the circumstances were beyond {first}'s control. "
        "Submission will be made by the end of the week.\n\n"
        "With respect,\n{name}",

        // 18 — dog ate it (but told realistically)
        "Dear Teacher,\n\n"
        "{name} is aware of how this sounds, but the homework was genuinely "
        "destroyed by the family dog. {first} left the papers on the coffee "
        "table to get a glass of water, and in those three minutes the dog "
        "chewed through nearly every sheet. {first} has photographic evidence "
        "if it would help. A rewritten copy will be brought in tomorrow without "
        "fail.\n\n"
        "Sincerely (and embarrassed),\n{name}",

        // 19 — school portal down
        "Dear Teacher,\n\n"
        "{name} attempted to submit the assignment through the school portal "
        "before the midnight deadline, but the site was returning a 503 error "
        "for over two hours. Screenshots of the error message and the timestamps "
        "are attached to this email as proof. {first} did not want to submit "
        "late without an explanation. Please confirm a suitable alternative "
        "submission method.\n\n"
        "Thank you,\n{name}",

        // 20 — working night shift to help family
        "Dear Teacher,\n\n"
        "Please excuse {name} for the missing assignment. {first}'s family runs "
        "a small business and a member of staff called in sick, so {first} "
        "stepped in to help cover the evening shift. It was an exceptional "
        "situation and {first} did not get home until very late. {first} will "
        "complete and submit the work tonight and it will not happen again.\n\n"
        "Thank you for your understanding,\n{name}",

        // 21 — software crash mid-essay
        "Dear Teacher,\n\n"
        "{name} is writing to explain a frustrating technical situation. Around "
        "11 p.m. last night, while writing the final paragraph of the essay, the "
        "word processor crashed and refused to reopen. When {first} eventually "
        "got the file to load, the last forty-five minutes of writing had not "
        "been saved. {first} simply ran out of time to rewrite the missing "
        "section before the morning. The partial draft can be shared on request.\n\n"
        "Apologies,\n{name}",

        // 22 — wrong notebook brought to school
        "Dear Teacher,\n\n"
        "{name} would like to explain why the homework is not available today. "
        "{first} completed it by hand in the dedicated exercise book, but in the "
        "early morning rush accidentally packed last year's notebook instead. The "
        "correct book — with the finished work inside — is at home. {first} can "
        "photograph and email the completed pages within the next ten minutes if "
        "that is acceptable.\n\n"
        "Regards,\n{name}",

        // 23 — mental-health day (respectful framing)
        "Dear Teacher,\n\n"
        "I wanted to be honest with you rather than invent an excuse. {name} has "
        "been under considerable pressure this week and by Sunday evening was "
        "too mentally exhausted to produce work that reflects their true ability. "
        "{first} made the decision to rest rather than submit something below "
        "standard. {first} is ready to discuss this privately if needed and will "
        "submit the completed assignment by tomorrow.\n\n"
        "Thank you for your understanding,\n{name}",

        // 24 — printer out of ink, no shop nearby
        "Dear Teacher,\n\n"
        "{name} finished the assignment last night and tried to print it, only "
        "to discover the printer cartridge had run out. The nearest shop that "
        "carries the correct cartridge is several kilometres away and was already "
        "closed by that time. {first}'s parents were unavailable to drive. {first} "
        "has the completed document ready to email right now and will bring a "
        "printed copy tomorrow once a new cartridge has been purchased.\n\n"
        "Apologies for the inconvenience,\n{name}",

        // 25 — misread deadline
        "Dear Teacher,\n\n"
        "{name} takes full responsibility for this mix-up. While reviewing the "
        "assignment calendar last week, {first} noted the deadline as Friday "
        "rather than Thursday. It was an honest misreading, not negligence — "
        "{first} had in fact started the work early precisely to avoid a last- "
        "minute rush. The assignment is largely finished and will be submitted "
        "in full by end of today. {first} will be more careful with deadlines "
        "going forward.\n\n"
        "Sincerely,\n{name}"
    };

    const int N = static_cast<int>(templates.size());

    // ── header ─────────────────────────────────────────
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║   🎓  Student Excuse Generator  v2.0  🎓    ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";

    // ── name input ─────────────────────────────────────
    std::string fullName;
    while (fullName.empty()) {
        std::cout << "  Enter student's full name : ";
        std::getline(std::cin, fullName);
        fullName = trim(fullName);
        if (fullName.empty())
            std::cout << "  [Name cannot be blank — please try again]\n";
    }

    const std::string first = firstName(fullName);

    // ── interactive loop ───────────────────────────────
    int lastIndex = -1;   // prevent same excuse twice in a row
    char again    = 'y';

    while (again == 'y' || again == 'Y') {
        // Pick a random index that differs from the last
        int idx;
        do { idx = std::rand() % N; } while (idx == lastIndex && N > 1);
        lastIndex = idx;

        std::string excuse = fillTemplate(templates[idx], fullName, first);

        std::cout << "\n";
        std::cout << "┌─────────────────────────────────────────────────────────────────────┐\n";
        std::cout << "│  Excuse  #" << std::setw(2) << (idx + 1)
                  << " of " << N
                  << "          Student: " << std::left << std::setw(28)
                  << fullName.substr(0, 27) << "│\n";
        std::cout << "└─────────────────────────────────────────────────────────────────────┘\n\n";

        // Print the excuse line-by-line with word-wrapping applied per line
        std::istringstream stream(excuse);
        std::string line;
        while (std::getline(stream, line)) {
            if (line.empty()) {
                std::cout << "\n";
            } else {
                std::cout << wordWrap(line, 72) << "\n";
            }
        }

        std::cout << "\n──────────────────────────────────────────────────────────────────────\n";
        std::cout << "  Generate another excuse? (y/n) : ";
        std::string ans;
        std::getline(std::cin, ans);
        ans = trim(ans);
        again = ans.empty() ? 'n' : static_cast<char>(std::tolower(ans[0]));
        std::cout << "\n";
    }

    std::cout << "  Goodbye, " << first << ". Good luck with your teacher! 😄\n\n";
    return 0;
}