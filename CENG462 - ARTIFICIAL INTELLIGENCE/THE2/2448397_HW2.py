"""

Heuristic Explanation

My heuristic is defined in heuristic function. It simply calculates the number of misplaced tiles and divides it by 2.
I divide it by 2 because of the following case:

Initial: WWEBB 
Goal:    WWBEB

If I didn't divide by 2, admissibility would be failed in the case above. This is because we can change the position of
two tiles within a step.

My heuristic is admissable because it is a solution to relaxed problem. In which we can change the position of any tile 
to arbitary new position with just 1 cost.

"""

import heapq

def isGoalState(currentState, goalState):
    return currentState == goalState

# This function takes a list of characters and concatenates it.
def listToStr(l):
    s = ""
    for item in l:
        s += item
    return s

# Next states by type 1 move
def generateNextStatesMoveType1(currentState):
    currentState = list(currentState)
    nextStates = set()
    l = len(currentState)

    for i in range(l):
        if currentState[i] == "E":
            if i == 0:
                currentState[i] = currentState[i+1]
                currentState[i+1] = "E"
                nextStates.add((listToStr(currentState), 1))
            elif i == (l-1):
                currentState[i] = currentState[i-1]
                currentState[i-1] = "E"
                nextStates.add((listToStr(currentState), 1))
            else:                
                a = currentState[i-1]
                b = currentState[i]
                c = currentState[i+1]

                currentState[i-1] = b
                currentState[i] = a
                nextStates.add((listToStr(currentState), 1))

                currentState[i-1] = a
                currentState[i] = b

                currentState[i+1] = b
                currentState[i] = c
                nextStates.add((listToStr(currentState), 1))

            break

    return nextStates

# Next states by type 2 move
def generateNextStatesMoveType2(currentState):
    currentState = list(currentState)
    nextStates = set()
    l = len(currentState)

    # Next states by type 2 move
    for i in range(l):
        if currentState[i] == "E":
            if len(currentState) == 3:
                if i == 0:
                    a = currentState[0]
                    c = currentState[2]
                    currentState[0] = c
                    currentState[2] = "E"
                    nextStates.add((listToStr(currentState), 1))

                elif i == 1:
                    pass

                elif i == 2:
                    a = currentState[0]
                    c = currentState[2]
                    currentState[0] = "E"
                    currentState[2] = a
                    nextStates.add((listToStr(currentState), 1))

            elif len(currentState) == 5:
                if i == 0:
                    a = currentState[0]
                    b = currentState[2]
                    c = currentState[3]

                    currentState[2] = "E"
                    currentState[0] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[0] = a
                    currentState[2] = b

                    currentState[3] = "E"
                    currentState[0] = c
                    nextStates.add((listToStr(currentState), 2))

                elif i == 1:
                    a = currentState[1]
                    b = currentState[3]
                    c = currentState[4]

                    currentState[3] = "E"
                    currentState[1] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[1] = a
                    currentState[3] = b

                    currentState[4] = "E"
                    currentState[1] = c
                    nextStates.add((listToStr(currentState), 2))

                elif i == 2:
                    a = currentState[0]
                    b = currentState[2]
                    c = currentState[4]

                    currentState[0] = "E"
                    currentState[2] = a
                    nextStates.add((listToStr(currentState), 1))
                    currentState[0] = a
                    currentState[2] = b

                    currentState[2] = c
                    currentState[4] = "E"
                    nextStates.add((listToStr(currentState), 1))   

                elif i == 3:
                    a = currentState[0]
                    b = currentState[1]
                    c = currentState[3]

                    currentState[0] = "E"
                    currentState[3] = a
                    nextStates.add((listToStr(currentState), 2))  
                    currentState[0] = a
                    currentState[3] = c

                    currentState[1] = "E"
                    currentState[3] = b
                    nextStates.add((listToStr(currentState), 1))        

                elif i == 4:
                    a = currentState[1]
                    b = currentState[2]
                    c = currentState[4]

                    currentState[1] = "E"
                    currentState[4] = a
                    nextStates.add((listToStr(currentState), 2))  
                    currentState[1] = a
                    currentState[4] = c

                    currentState[2] = "E"
                    currentState[4] = b
                    nextStates.add((listToStr(currentState), 1))  
     
            else:
                if i == 0:
                    a = currentState[0]
                    b = currentState[2]
                    c = currentState[3]

                    currentState[2] = "E"
                    currentState[0] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[0] = a
                    currentState[2] = b

                    currentState[3] = "E"
                    currentState[0] = c
                    nextStates.add((listToStr(currentState), 2))

                elif i == 1:
                    a = currentState[1]
                    b = currentState[3]
                    c = currentState[4]

                    currentState[3] = "E"
                    currentState[1] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[1] = a
                    currentState[3] = b

                    currentState[4] = "E"
                    currentState[1] = c
                    nextStates.add((listToStr(currentState), 2))
                    
                elif i == 2:
                    a = currentState[0]
                    b = currentState[2]
                    c = currentState[4]
                    d = currentState[5]

                    currentState[0] = "E"
                    currentState[2] = a
                    nextStates.add((listToStr(currentState), 1))
                    currentState[0] = a
                    currentState[2] = b

                    currentState[4] = "E"
                    currentState[2] = c
                    nextStates.add((listToStr(currentState), 1))
                    currentState[2] = b
                    currentState[4] = c

                    currentState[5] = "E"
                    currentState[2] = d
                    nextStates.add((listToStr(currentState), 2))                    

                elif i == (l-1):
                    a = currentState[l-4]
                    b = currentState[l-3]
                    c = currentState[l-1] # E

                    currentState[l-1] = a
                    currentState[l-4] = "E"
                    nextStates.add((listToStr(currentState), 2))
                    currentState[l-4] = a
                    currentState[l-1] = c

                    currentState[l-1] = b
                    currentState[l-3] = "E"
                    nextStates.add((listToStr(currentState), 1))

                elif i == (l-2):
                    a = currentState[l-5]
                    b = currentState[l-4]
                    c = currentState[l-2]

                    currentState[l-2] = a
                    currentState[l-5] = "E"
                    nextStates.add((listToStr(currentState), 2))
                    currentState[l-5] = a
                    currentState[l-2] = c

                    currentState[l-2] = b
                    currentState[l-4] = "E"
                    nextStates.add((listToStr(currentState), 1))
                    
                elif i == (l-3):
                    a = currentState[l-6]
                    b = currentState[l-5]
                    c = currentState[l-3] 
                    d = currentState[l-1]

                    currentState[l-6] = "E"
                    currentState[l-3] = a
                    nextStates.add((listToStr(currentState), 2))
                    currentState[l-3] = "E"
                    currentState[l-6] = a

                    currentState[l-5] = "E"
                    currentState[l-3] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[l-3] = "E"
                    currentState[l-5] = b

                    currentState[l-1] = "E"
                    currentState[l-3] = d
                    nextStates.add((listToStr(currentState), 1))

                else:
                    a = currentState[i-3]
                    b = currentState[i-2]
                    c = currentState[i]
                    d = currentState[i+2]
                    e = currentState[i+3]

                    currentState[i-3] = "E"
                    currentState[i] = a
                    nextStates.add((listToStr(currentState), 2))
                    currentState[i-3] = a
                    currentState[i] = "E"

                    currentState[i-2] = "E"
                    currentState[i] = b
                    nextStates.add((listToStr(currentState), 1))
                    currentState[i-2] = b
                    currentState[i] = "E"

                    currentState[i+2] = "E"
                    currentState[i] = d
                    nextStates.add((listToStr(currentState), 1))
                    currentState[i+2] = d
                    currentState[i] = "E"                    

                    currentState[i+3] = "E"
                    currentState[i] = e
                    nextStates.add((listToStr(currentState), 2))
                    currentState[i+3] = e
                    currentState[i] = "E"

            break
    
    return nextStates

# All next states
def generateAllNextStates(currentState):
    return generateNextStatesMoveType1(currentState).union(generateNextStatesMoveType2(currentState))

# Define a class for the state of the puzzle
class State:
    def __init__(self, config, cost, heuristic, parent=None):
        self.config = config  # Configuration of the puzzle
        self.cost = cost      # Cost to reach this state (g)
        self.heuristic = heuristic  # Heuristic value for this state (h)
        self.parent = parent  # Parent state

    def __lt__(self, other):
        # Define comparison for states based on total cost + heuristic
        return self.cost + self.heuristic < other.cost + other.heuristic

# Define heuristic function (Manhattan distance)
def heuristic_function(state, goal):
    n_misplaced = 0
    l = len(state)
    for i in range(l):
        if state[i] != goal[i]:
            n_misplaced += 1
    return n_misplaced / 2

# A* search algorithm
def a_star(initial, goal):
    # Priority queue for open states
    open_states = []
    heapq.heappush(open_states, initial)

    # Set to store visited states
    visited = set()

    while open_states:
        # Get the state with the lowest total cost + heuristic
        current_state = heapq.heappop(open_states)

        # If current state is the goal, return the path
        if isGoalState(current_state.config, goal):
            return current_state

        # Add current state to visited
        visited.add(current_state.config)

        # Generate possible moves
        next_states = generateAllNextStates(current_state.config)
        for state, cost in next_states:
            if state not in visited:
                new_cost = current_state.cost + cost
                new_heuristic = heuristic_function(state, goal)
                new_state = State(state, new_cost, new_heuristic, parent=current_state)
                heapq.heappush(open_states, new_state)

    # If no solution found
    return None

def extract_solution_path(goal_state):
    # Extract the solution path
    path = []
    current_state = goal_state
    while current_state is not None:
        path.append(current_state.config)
        current_state = current_state.parent
    return list(reversed(path))

if __name__ == "__main__":
    # Read inputs from terminal
    n = int(input())
    initial_config = input()
    final_config = input()

    # Perform A* search
    initial_state = State(initial_config, 0, heuristic_function(initial_config, final_config))
    goal_state = a_star(initial_state, final_config)

    if goal_state is not None:
        # Extract solution path
        solution_path = extract_solution_path(goal_state)
        num_moves = len(solution_path) - 1
        total_cost = goal_state.cost

        # Output solution path
        print(f"({num_moves},{total_cost})")
        print(solution_path)
    else:
        print("(0,0)")

