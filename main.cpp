#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <iomanip>
#include <fstream> 

using namespace std;

int main() {
    // Store party names
    map<int, string> party;
    party[1] = "BJP";
    party[2] = "INC";
    party[3] = "AAP";
    party[4] = "TMC";
    party[5] = "NOTA";

    // Store votes
    map<int, int> votes;

    // Load votes from file (database)
    ifstream voteFile("votes.txt");
    int id, count;
    while (voteFile >> id >> count) {
        votes[id] = count;
    }
    voteFile.close();

    // If file empty, initialize votes
    if (votes.empty()) {
        for (auto p : party) {
            votes[p.first] = 0;
        }
    }

    // Load voters from file
    unordered_set<string> voterIDs;
    ifstream voterFile("voters.txt");
    string vid;
    while (voterFile >> vid) {
        voterIDs.insert(vid);
    }
    voterFile.close();

    // Input total voters
    int maxVoters;
    cout << "Enter total number of voters: ";
    cin >> maxVoters;

    cout << "\n--- Voting Started ---\n";

    int totalVotesCasted = 0;

    // Voting loop
    for (int i = 1; i <= maxVoters; i++) {
        string aadhaar;
        cout << "\nVoter " << i << " - Enter your 4-digit Aadhaar ID: ";
        cin >> aadhaar;

        if (aadhaar.length() != 4) {
            cout << "❌ Invalid Aadhaar! Must be 4 digits.\n";
            i--;
            continue;
        }

        if (voterIDs.find(aadhaar) != voterIDs.end()) {
            cout << "⚠️ You have already voted!\n";
            i--;
            continue;
        }

        voterIDs.insert(aadhaar);

        cout << "\nChoose your party:\n";
        for (auto p : party) {
            cout << p.first << ". " << p.second << endl;
        }

        int choice;
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        if (party.find(choice) != party.end()) {
            votes[choice]++;
            totalVotesCasted++;
            cout << "✅ Vote recorded for " << party[choice] << "!\n";

            //  Save voter to file
            ofstream outVoter("voters.txt", ios::app);
            outVoter << aadhaar << endl;
            outVoter.close();

        } else {
            cout << "❌ Invalid choice! Vote not counted.\n";
        }
    }

    //  Save votes to file
    ofstream outVote("votes.txt");
    for (auto v : votes) {
        outVote << v.first << " " << v.second << endl;
    }
    outVote.close();

    // Results
    cout << "\n--- Voting Ended ---\n";
    cout << "\nFinal Results:\n";

    cout << left << setw(10) << "Party" << setw(10) << "Votes" << endl;
    cout << "----------------------\n";

    for (auto v : votes) {
        cout << left << setw(10) << party[v.first] << setw(10) << v.second << endl;
    }

    cout << "\nTotal Votes Casted: " << totalVotesCasted << " out of " << maxVoters << endl;

    // Winner logic
    int maxVotes = 0;
    string winner = "";
    bool tie = false;

    for (auto v : votes) {
        if (v.second > maxVotes) {
            maxVotes = v.second;
            winner = party[v.first];
            tie = false;
        }
        else if (v.second == maxVotes && maxVotes != 0) {
            tie = true;
        }
    }

    cout << "\nResult: ";
    if (tie) {
        cout << "It's a tie between multiple parties.\n";
    }
    else if (winner != "") {
        cout << winner << " wins with " << maxVotes << " votes\n";
    }
    else {
        cout << "No votes cast.\n";
    }

    return 0;
}




// used = votes.txt, voters.txt