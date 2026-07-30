#include "voting_system.h"

// Global Variables
unordered_map<string, Voter> voters;
unordered_map<string, Candidate> candidates;
set<string> votedVoters;
stack<tuple<string, string>> undoStack;
queue<tuple<string, string>> voteQueue;

string adminUsername = "admin";
string adminPassword = "admin123";

Voter::Voter(string id, string pass) : voterID(id), password(pass), hasVoted(false) {}
Voter::Voter() : voterID(""), password(""), hasVoted(false) {}

Candidate::Candidate(string id, string n) : candidateID(id), name(n), votes(0) {}
Candidate::Candidate() : candidateID(""), name(""), votes(0) {}

int main() {
    loadData();

    int choice;
    cout << endl; cout << endl; cout << endl; cout << endl; cout << endl; cout << endl; cout << endl; cout << endl; cout << endl;
    cout << "\t \t \t\t \t= = = Online Voting System = = =\n\n\t\t \t \t\t  Press Enter to continue: ";
    cin.ignore();

    do {
        cout << "\n=== Online Voting System ===\n";
        cout << "1. Admin Panel\n";
        cout << "2. Voter Panel\n";
        cout << "3. Exit\n";
        cout<<"==========================\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                if (adminLogin()) {
                    adminMenu();
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;
            case 2:
                voterMenu();
                break;
            case 3:
                saveData();
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}

void adminMenu() {
    int choice;
    do {
        cout << "\n=== Admin Panel ===\n";
        cout << "1. Add Candidate\n";
        cout << "2. Remove Candidate\n";
        cout << "3. View Leaderboard\n";
        cout << "4. Declare Winner\n";
        cout << "5. Change Admin Password\n";
        cout << "6. Display Vote History\n";
        cout << "7. Back to Main Menu\n";
        cout<<"==========================\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addCandidate();
                break;
            case 2:
                removeCandidate();
                break;
            case 3:
                viewLeaderboard();
                break;
            case 4:
                declareWinner();
                break;
            case 5:
                {
                    string newPassword;
                    cout << "Enter new admin password: ";
                    cin >> newPassword;
                    adminPassword = newPassword;
                    cout << "Password updated successfully.\n";
                }
                break;
            case 6:
                displayVoteHistory();
            case 7:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);
}

void voterMenu() {
    int choice;
    cout << "\n=== Voter Panel ===\n";
    cout << "1. Login\n";
    cout << "2. Sign Up\n";
    cout << "3. Back to Main Menu\n";
    cout<<"==========================\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        return;
    }

    switch (choice) {
        case 1:
            if (voterLogin()) {
                do {
                    cout << "\n=== Voter Actions ===\n";
                    cout << "1. Cast Vote\n";
                    cout << "2. Undo Last Vote\n";
                    cout << "3. Back to Main Menu\n";
                    cout<<"==========================\n";
                    cout << "Enter your choice: ";
                    if (!(cin >> choice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (choice) {
                        case 1:
                            castVote();
                            break;
                        case 2:
                            undoVote();
                            break;
                        case 3:
                            return;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                } while (choice != 3);
            } else {
                cout << "Invalid voter credentials. Returning to main menu.\n";
            }
            break;
        case 2:
            voterSignUp();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

void addCandidate() {
    string id, name;
    cout << "Enter Candidate ID: ";
    cin >> id;
    cout << "Enter Candidate Name: ";
    cin.ignore();
    getline(cin, name);

    if (candidates.find(id) != candidates.end()) {
        cout << "Candidate with this ID already exists!\n";
        return;
    }

    candidates[id] = Candidate(id, name);
    cout << "Candidate added successfully.\n";
}

void removeCandidate() {
    string id;
    cout << "Enter Candidate ID to Remove: ";
    cin >> id;

    if (candidates.erase(id)) {
        cout << "Candidate removed successfully.\n";
    } else {
        cout << "Candidate not found!\n";
    }
}

void castVote() {
    string voterID, candidateID;
    cout << "Enter your Voter ID: ";
    cin >> voterID;

    if (votedVoters.find(voterID) != votedVoters.end()) {
        cout << "You have already voted!\n";
        return;
    }

    cout << "Available candidates:\n";
    for (const auto &pair : candidates) {
        cout << "Candidate ID: " << pair.first << " | Name: " << pair.second.name << "\n";
    }

    cout << "Enter Candidate ID to Vote: ";
    cin >> candidateID;

    if (candidates.find(candidateID) == candidates.end()) {
        cout << "Candidate not found!\n";
        return;
    }

    candidates[candidateID].votes++;
    votedVoters.insert(voterID); // Mark the voter as voted
    voters[voterID].hasVoted = true;

    // Push to undo stack and vote queue
    undoStack.push(make_tuple(voterID, candidateID));
    voteQueue.push(make_tuple(voterID, candidateID));

    cout << "Vote cast successfully.\n";
}

void viewLeaderboard() {
    if (candidates.empty()) {
        cout << "No candidates available.\n";
        return;
    }
    cout << "\n=== Leaderboard ===\n";
    for (const auto &pair : candidates) {
        cout << "Candidate: " << pair.second.name << " | Votes: " << pair.second.votes << "\n";
    }
}

void undoVote() {
    if (undoStack.empty()) {
        cout << "No votes to undo!\n";
        return;
    }
    auto lastVote = undoStack.top();
    undoStack.pop();
    string voterID = std::get<0>(lastVote);
    string candidateID = std::get<1>(lastVote);
    candidates[candidateID].votes--;
    votedVoters.erase(voterID);
    voters[voterID].hasVoted = false;
    cout << "Undo successful. Vote for " << candidates[candidateID].name << " has been removed.\n";
}

void declareWinner() {
    if (candidates.empty()) {
        cout << "No candidates to declare a winner.\n";
        return;
    }

    string winnerID;
    int maxVotes = -1;
    for (const auto &pair : candidates) {
        if (pair.second.votes > maxVotes) {
            maxVotes = pair.second.votes;
            winnerID = pair.first;
        }
    }

    cout << "The winner is: " << candidates[winnerID].name << " with " << maxVotes << " votes.\n";
}

void saveData() {
    ofstream votersFile("voters.txt");
    for (const auto &pair : voters) {
        votersFile << pair.second.voterID << " " << pair.second.password << " " << pair.second.hasVoted << "\n";
    }

    ofstream candidatesFile("candidates.txt");
    for (const auto &pair : candidates) {
        candidatesFile << pair.second.candidateID << " " << pair.second.name << " " << pair.second.votes << "\n";
    }
}

void loadData() {
    ifstream votersFile("voters.txt");
    string id, pass;
    bool hasVoted;
    while (votersFile >> id >> pass >> hasVoted) {
        voters[id] = Voter(id, pass);
        voters[id].hasVoted = hasVoted;
    }

    ifstream candidatesFile("candidates.txt");
    string name;
    int votes;
    while (candidatesFile >> id >> name >> votes) {
        candidates[id] = Candidate(id, name);
        candidates[id].votes = votes;
    }
}

bool adminLogin() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    return username == adminUsername && password == adminPassword;
}

bool voterLogin() {
    string voterID, password;
    cout << "Enter your Voter ID: ";
    cin >> voterID;
    cout << "Enter your password: ";
    cin >> password;

    auto it = voters.find(voterID);
    if (it != voters.end() && it->second.password == password) {
        return true;
    }
    return false;
}

void voterSignUp() {
    string voterID, password;
    cout << "Enter your Voter ID: ";
    cin >> voterID;
    cout << "Enter your password: ";
    cin >> password;

    voters[voterID] = Voter(voterID, password);
    cout << "Sign-up successful! You can now login to vote.\n";
}
void displayVoteHistory() {
    if (voteQueue.empty()) {
        cout << "No votes have been cast yet.\n";
        return;
    }

    queue<tuple<string, string>> tempQueue = voteQueue; // Create a copy of the queue
    cout << "\n=== Vote History ===\n";
    while (!tempQueue.empty()) {
        auto vote = tempQueue.front(); // Get the front element
        string voterID = get<0>(vote); // Extract Voter ID
        string candidateID = get<1>(vote); // Extract Candidate ID

        // Print the vote details
        cout << "Voter ID: " << voterID << " -> Voted for Candidate ID: " << candidateID << "\n";

        tempQueue.pop(); // Remove the front element
    }
}
