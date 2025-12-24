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

/* Reset election */
void restartElection() {
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        votes[i] = 0;
        candidates[i][0] = '\0';
    }
    spoiledVotes = 0;
    candidateCount = 0;
    initialized = 0;

    printf("\n Election reset! Time to cast new votes.\n");
}

/* Save election data to file (ONLY on reset) */
void saveResultsToFile() {
    if (!initialized) {
        return;
    }

    FILE *file = fopen("election_results.txt", "a");
    if (file == NULL) {
        printf("\n Error saving file!\n");
        return;
    }

    int totalVotes = spoiledVotes;
    for (int i = 0; i < candidateCount; i++)
        totalVotes += votes[i];

    fprintf(file, "\n============================\n");
    fprintf(file, "ELECTION RESULTS\n");
    fprintf(file, "Total Candidates: %d\n\n", candidateCount);

    for (int i = 0; i < candidateCount; i++) {
        double percent = totalVotes == 0 ? 0 :
                         ((double)votes[i] / totalVotes) * 100;
        fprintf(file, "%s - %d votes (%.2f%%)\n",
                candidates[i], votes[i], percent);
    }

    fprintf(file, "\nSpoiled Votes: %d\n", spoiledVotes);
    fprintf(file, "Total Votes: %d\n", totalVotes);

    int maxVotes = 0;
    for (int i = 0; i < candidateCount; i++)
        if (votes[i] > maxVotes)
            maxVotes = votes[i];

    fprintf(file, "\nWinner(s):\n");
    if (maxVotes == 0) {
        fprintf(file, "No votes cast.\n");
    } else {
        for (int i = 0; i < candidateCount; i++)
            if (votes[i] == maxVotes)
                fprintf(file, "%s (%d votes)\n",
                        candidates[i], votes[i]);
    }

    fprintf(file, "============================\n");

    fclose(file);
}

/* Input candidates */
void inputCandidates() {
    char input[10];

    printf(" Enter total number of candidates : ");
    fgets(input, sizeof(input), stdin);
    candidateCount = atoi(input);

    if (candidateCount < 1 || candidateCount > MAX_CANDIDATES) {
        printf(" Invalid number of candidates.\n");
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
                    printf(" Duplicate name found. Try again.\n");
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

/* Cast vote */
void castVote() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    char input[10];
    int choice;

    printf("\n Please choose your Candidate:\n");
    for (int i = 0; i < candidateCount; i++) {
        printf(" %d. %s\n", i + 1, candidates[i]);
    }
    printf(" %d. None of These\n", candidateCount + 1);

    printf(" Enter your choice: ");
    fgets(input, sizeof(input), stdin);
    choice = atoi(input);

    if (choice >= 1 && choice <= candidateCount) {
        votes[choice - 1]++;
        printf("\n Thanks for voting!\n");
    } else if (choice == candidateCount + 1) {
        spoiledVotes++;
        printf("\n Your vote has been marked as spoiled.\n");
    } else {
        printf("\n Invalid choice! Vote not counted.\n");
    }
}

/* Show vote count */
void voteCount() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    int totalVotes = spoiledVotes;
    for (int i = 0; i < candidateCount; i++)
        totalVotes += votes[i];

    if (totalVotes == 0) {
        printf("\n No votes have been cast yet.\n");
        return;
    }

    printf("\n Voting Statistics:\n");
    for (int i = 0; i < candidateCount; i++) {
        double percent = ((double)votes[i] / totalVotes) * 100;
        printf(" %s - %d votes (%.2f%%)\n",
               candidates[i], votes[i], percent);
    }
    printf(" Spoiled Votes - %d\n", spoiledVotes);
}

/* Leading candidate */
void getLeadingCandidate() {
    if (!initialized) {
        printf("\n Oops! YOU NEED TO ADD CANDIDATES FIRST\n");
        return;
    }

    int maxVotes = 0, tie = 0;
    for (int i = 0; i < candidateCount; i++)
        if (votes[i] > maxVotes)
            maxVotes = votes[i];

    if (maxVotes == 0) {
        printf("\n NO VOTES CAST YET\n");
        return;
    }

    for (int i = 0; i < candidateCount; i++)
        if (votes[i] == maxVotes)
            tie++;

    printf("\n Leading Candidate:\n");
    if (tie > 1) {
        printf(" It's a tie with %d votes:\n", maxVotes);
        for (int i = 0; i < candidateCount; i++)
            if (votes[i] == maxVotes)
                printf(" %s\n", candidates[i]);
    } else {
        for (int i = 0; i < candidateCount; i++)
            if (votes[i] == maxVotes) {
                printf(" %s leading with %d votes.\n",
                       candidates[i], maxVotes);
                break;
            }
    }
}

int main() {
    char input[20];
    int choice;

    do {
        printf("\n\t\t BALLOT FREEDOM\n");
        printf("Commands: type reset to Restart the election\n");

        printf("\n 1. Enter Candidates");
        printf("\n 2. Cast Vote");
        printf("\n 3. Show Vote Count");
        printf("\n 4. Show Leading Candidate");
        printf("\n 0. Exit");
        printf("\n Enter your choice: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        /* ONLY RESET WILL SAVE */
        if (strcmp(input, "reset") == 0) {
            saveResultsToFile();   
            restartElection();
            continue;
        }

        choice = atoi(input);

        switch (choice) {
            case 1: inputCandidates(); 
                    break;
            case 2: castVote(); 
                    break;
            case 3: voteCount(); 
                    break;
            case 4: getLeadingCandidate(); 
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
