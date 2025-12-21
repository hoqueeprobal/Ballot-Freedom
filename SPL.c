#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 100
#define MAX_NAME_LENGTH 100

char candidates[MAX_CANDIDATES][MAX_NAME_LENGTH];
int votes[MAX_CANDIDATES];
int spoiledVotes = 0;
int candidateCount = 0;
int initialized = 0;

// Input candidate names and initialize vote counts
void inputCandidates() {
    char input[10];

    printf(" Enter total number of candidates : ");
    fgets(input, sizeof(input), stdin);
    candidateCount = atoi(input);

    if (candidateCount < 1 || candidateCount > MAX_CANDIDATES) {
        printf(" Invalid number of candidates. Try again.\n");
        candidateCount = 0;
        return;
    }

    printf(" Enter names of %d candidates:\n", candidateCount);

    for (int i = 0; i < candidateCount; i++) {
        int isDuplicate;
        do {
            printf(" Candidate %d: ", i + 1);
            fgets(candidates[i], MAX_NAME_LENGTH, stdin);
            candidates[i][strcspn(candidates[i], "\n")] = '\0';

            if (strlen(candidates[i]) == 0) {
                printf(" Name cannot be empty. Try again.\n");
                isDuplicate = 1;
                continue;
            }

            isDuplicate = 0;
            for (int j = 0; j < i; j++) {
                if (strcmp(candidates[i], candidates[j]) == 0) {
                    printf(" Duplicate name found. Please enter a different name.\n");
                    isDuplicate = 1;
                    break;
                }
            }
        } while (isDuplicate);

        votes[i] = 0;
    }

    spoiledVotes = 0;
    initialized = 1;
    printf("\n CANDIDATE LIST SAVED SUCCESSFULLY!\n");
}

// Cast a vote
void castVote() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    char input[10];
    int choice;

    printf("\n Please choose your Candidate :\n");
    for (int i = 0; i < candidateCount; i++) {
        printf(" %d. %s\n", i + 1, candidates[i]);
    }
    printf(" %d. None of These\n", candidateCount + 1);

    printf("\n Input your choice (1 - %d): ", candidateCount + 1);
    fgets(input, sizeof(input), stdin);
    choice = atoi(input);

    if (choice >= 1 && choice <= candidateCount) {
        votes[choice - 1]++;
        printf("\n Thanks For Voting!\n");
    } else if (choice == candidateCount + 1) {
        spoiledVotes++;
        printf("\n Thanks! Your vote has been marked as spoiled.\n");
    } else {
        printf("\n Invalid choice! Vote not counted.\n");
    }
}

// Display vote counts
void voteCount() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    printf("\n Voting Statistics :\n");
    for (int i = 0; i < candidateCount; i++) {
        printf(" %s - %d\n", candidates[i], votes[i]);
    }
    printf(" Spoiled Votes - %d\n", spoiledVotes);
}

// Show leading candidate
void getLeadingCandidate() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    int maxVotes = 0;
    int countMax = 0;

    for (int i = 0; i < candidateCount; i++) {
        if (votes[i] > maxVotes) {
            maxVotes = votes[i];
        }
    }

    if (maxVotes == 0) {
        printf("\n NO VOTES CAST. VOTE FIRST TO SEE LEADING CANDIDATE\n");
        return;
    }

    printf("\n Leading Candidate :\n");

    for (int i = 0; i < candidateCount; i++) {
        if (votes[i] == maxVotes) {
            countMax++;
        }
    }

    if (countMax > 1) {
        printf(" It's a tie! %d votes each:\n", maxVotes);
        for (int i = 0; i < candidateCount; i++) {
            if (votes[i] == maxVotes) {
                printf(" %s\n", candidates[i]);
            }
        }
    } else {
        for (int i = 0; i < candidateCount; i++) {
            if (votes[i] == maxVotes) {
                printf(" %s leading with %d votes.\n", candidates[i], maxVotes);
                break;
            }
        }
    }
}

int main() {
    char input[10];
    int choice;

    do {
        printf("\n\t\t\t BALLOT FREEDOM\n");
        printf("\n 1. Enter Total Candidates' Number and Their Names");
        printf("\n 2. Cast Your Vote");
        printf("\n 3. See Total Vote Count");
        printf("\n 4. See the Leading Candidate");
        printf("\n 0. Exit");
        printf("\n Please enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        switch (choice) {
            case 1:
                inputCandidates();
                break;
            case 2:
                castVote();
                break;
            case 3:
                voteCount();
                break;
            case 4:
                getLeadingCandidate();
                break;
            case 0:
                printf("\n Exiting program. Thank you!\n");
                break;
            default:
                printf("\n Invalid choice! Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
