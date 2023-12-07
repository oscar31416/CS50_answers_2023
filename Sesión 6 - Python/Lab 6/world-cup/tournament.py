# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000000

def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []

    # DONE - Read teams into memory from file
    input_file = open(sys.argv[1], 'r')
    data = csv.DictReader(input_file)
    for row in data:
        new_team = {}
        new_team["team"] = row["team"]
        new_team["rating"] = int(row["rating"])
        teams.append(new_team)

    # 'input_file' is closed.
    input_file.close()

    # 'input_file', 'new_team' and 'data' are deleted.
    # They're no longer required.
    del new_team
    del input_file
    del data

    counts = {}

    # DONE - Simulate N tournaments and keep track of win counts
    for i in range(N):
        winner = simulate_tournament(teams)
        if not (winner in counts.keys()):
            counts[winner] = 1
        else:
            counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """ DONE - Simulate a tournament. Return name of winning team. """

    """ Half of the teams move on from one round to the next, so... """
    remaining = []
    remaining = simulate_round(teams)

    # While the number of remaining teams is greater than 1...
    while (len(remaining) > 1):
        remaining = simulate_round(remaining)

    # Name of the winning team
    return remaining[0]["team"]

if __name__ == "__main__":
    main()
