#ifndef VOTING_SYSTEM_H
#define VOTING_SYSTEM_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <fstream>
#include <string>
#include <limits>
#include <queue>
#include <stack>
#include <tuple>
#include <algorithm>

using namespace std;

// Classes and Data Structures
class Voter {
public:
    string voterID;
    string password;
    bool hasVoted;

    Voter(string id, string pass);
    Voter();  // Default constructor
};

class Candidate {
public:
    string candidateID;
    string name;
    int votes;

    Candidate(string id, string n);
    Candidate();  // Default constructor
};

// Global Variables
extern unordered_map<string, Voter> voters;
extern unordered_map<string, Candidate> candidates;
extern set<string> votedVoters;
extern stack<tuple<string, string>> undoStack;
extern queue<tuple<string, string>> voteQueue;

extern string adminUsername;
extern string adminPassword;

// Function Prototypes
void adminMenu();
void voterMenu();
void addCandidate();
void removeCandidate();
void castVote();
void viewLeaderboard();
void undoVote();
void declareWinner();
void saveData();
void loadData();
bool adminLogin();
bool voterLogin();
void voterSignUp();
void displayVoteHistory();

#endif // VOTING_SYSTEM_H
