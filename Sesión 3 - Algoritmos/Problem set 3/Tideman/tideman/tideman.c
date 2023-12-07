#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// (ADDED BY THE STUDENT) Initialisation of the 'differences' array.
// Will become useful when sorting pairs.
int differences[MAX * MAX];

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
int cycle_check(int end_of_cycle, int start_of_cycle);
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
    // (ADDED) Initialisation of the 'preferences' array.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
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
    // Make sure 'name' is in 'candidates'.
    int i = 0;
    for (i = 0; i < candidate_count; i++)
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
    // Candidate 'r' is preferred over candidates who are after 'r' in 'ranks'.
    for (int r = 0; r < candidate_count - 1; r++)
    {
        for (int j = r + 1; j < candidate_count; j++)
        {
            int a = ranks[r];
            int b = ranks[j];
            preferences[a][b]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // We can record the (i, j) pairs that help us
    // locate non-zero items in 'preferences'.
    pair new_pair;
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int x = preferences[i][j] - preferences[j][i];
            if (x > 0)
            {
                new_pair.winner = i;
                new_pair.loser = j;
                pairs[pair_count] = new_pair;
                differences[pair_count] = x;
                pair_count++;
            }
            else if ((preferences[i][j] - preferences[j][i]) < 0)
            {
                new_pair.winner = j;
                new_pair.loser = i;
                pairs[pair_count] = new_pair;
                differences[pair_count] = x * (-1);
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // I will implement the selection sort.
    // The selection criterion uses the items in 'preferences' like this: I take all
    // 'abs(preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner])' values
    // to decide which pair of candidates is locked first. In other words, if Alice beats Bob by 7-2 and Bob
    // defeats Charlie by 6-0, the Bob-Charlie pair shows a greater difference so 'the arrow that goes from Bob
    // to Charlie will be drawn first', so to say.
    //
    // (QUESTION) Is this the expected criterion?
    int x;
    int y;
    for (int i = 0; i < pair_count; i++)
    {
        x = pairs[i].winner;
        y = pairs[i].loser;
    }

    pair aux_pair;
    int j_max;
    int diff_aux;

    // Expected location: 'i'
    for (int i = 0; i < pair_count - 1; i++)
    {
        // (Initially supposed) Location of maximum: 'j_max'
        j_max = i;

        // Let's find out where the maximum really is.
        for (int j = i + 1; j < pair_count; j++)
        {
            if (differences[j] > differences[j_max])
            {
                j_max = j;
            }
        }

        // Time to swap places!
        if (differences[i] < differences[j_max])
        {
            aux_pair.winner = pairs[i].winner;
            aux_pair.loser = pairs[i].loser;
            pairs[i].winner = pairs[j_max].winner;
            pairs[i].loser = pairs[j_max].loser;
            pairs[j_max].winner = aux_pair.winner;
            pairs[j_max].loser = aux_pair.loser;
            diff_aux = differences[i];
            differences[i] = differences[j_max];
            differences[j_max] = diff_aux;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Let's make sure 'locked' is initialised.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    // If the current pair doesn't close a cycle, lock it.
    // 'locked[i][j] == true' means that 'i' beats 'j' or,
    // in other words, that an arrow goes from 'i' to 'j'.
    int w;
    int l;
    for (int i = 0; i < pair_count; i++)
    {
        if (cycle_check(pairs[i].loser, pairs[i].winner) == 0)
        {
            w = pairs[i].winner;
            l = pairs[i].loser;
            locked[w][l] = true;
        }
    }
    return;
}

// Auxiliary recursive function
// that checks whether there's a cycle.
int cycle_check(int end_of_cycle, int start_of_cycle)
{
    // If beginning and end are identical, stop.
    if (end_of_cycle == start_of_cycle)
    {
        return 1;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // If 'end_of_cycle' and 'i' are locked,
        // check whether 'i' and 'start_of_cycle' are identical.
        if (locked[end_of_cycle][i])
        {
            if (cycle_check(i, start_of_cycle) == 1)
            {
                return 1;
            }
        }
    }

    // If there are no cycles at all, we're good to go.
    return 0;
}

// Print the winner of the election
void print_winner(void)
{
    // The winner has no arrows pointing at them.
    int no_arrow_counter;
    for (int i = 0; i < candidate_count; i++)
    {
        no_arrow_counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            // If an arrow goes from 'j' to 'i',
            // break this inner 'for' loop.
            if (locked[j][i] == true)
            {
                no_arrow_counter = 1;
                break;
            }
        }
        // If, at least, one arrow points at 'i',
        // we can jump to the loop's next iteration.
        if (no_arrow_counter == 1)
        {
            continue;
        }
        else
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    // If the programme reaches this point,
    // an error must be somewhere.
    printf("\nUnknown error.\n");
    return;
}
