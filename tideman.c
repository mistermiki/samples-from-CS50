#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair compair;

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                compair.winner = i;
                compair.loser = j;
                pairs[pair_count] = compair;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                compair.winner = j;
                compair.loser = i;
                pairs[pair_count] = compair;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp_pairs;
    int swaps_count;

    do
    {
        swaps_count = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            //do a bubble sort - if the next pair has more vicotry strenght (no of voters), swap the pairs
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                temp_pairs = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp_pairs;
                swaps_count++;
            }
        }
     }
     while (swaps_count != 0);
    //keep swapping the pairs as long as there are no more swaps

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        //is the winner also a loser (is a source already a destination)?
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][pairs[i].winner])
            {
                //is the loser also a winner (is a destination already a source)?
                for (int k = 0; k < candidate_count; k++)
                {
                    if (locked[pairs[i].loser][k])
                    {
                        //cycle, do not lock this pair
                        locked[pairs[i].winner][pairs[i].loser] = false;

                        //jump out to check next pair
                        k = candidate_count;
                        j = candidate_count;
                    }
                    else
                    {
                        locked[pairs[i].winner][pairs[i].loser] = true;
                    }
                }
            }
            else
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
            }
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    //find the winner (source)
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //if a candidate is a destination, it cannot be a winner
            if (locked[j][i])
            {
                //jump to the next candidate
                j = candidate_count;
            }

            if (j == candidate_count - 1)
            {
                //print out the name of winner
                printf("%s\n", candidates[i]);

            }
        }
    }

    return;
}