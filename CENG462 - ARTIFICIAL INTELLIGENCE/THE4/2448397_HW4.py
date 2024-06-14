import copy
import random

# Define actions as macros for better readability
LEFT_ACTION = "<"
UP_ACTION = "∧"
RIGHT_ACTION = ">"
DOWN_ACTION = "v"

# This is a class to store the parameters coming from the input file.
class MDPDefinition():
    def __init__(self, height, width, obstacles, goal_states, start_state, reward, b, a, c, learning_rate, gamma, epsilon, episode_count) -> None:
        self.height = height
        self.width = width
        self.obstacles = obstacles
        self.goal_states = goal_states
        self.start_state = start_state
        self.reward = reward
        self.action_noise = (a,b,c)
        self.learning_rate = learning_rate
        self.gamma = gamma
        self.epsilon = epsilon
        self.episode_count = episode_count

# This function is responsible for parsing the input file and loading parameters.
def inputParser(file_path):
    height = None
    width = None
    obstacles = []
    goal_states = []
    start_state = None
    reward = None
    a = None
    b = None
    c = None
    learning_rate = None
    gamma = None
    epsilon = None
    episode_count = None

    with open(file_path, 'r') as file:
        i = 0
        for line in file:
            line = line.rstrip("\n")

            if i == 0 or i == 2 or i == 4 or i == 6 or i == 8 or i == 10 or i == 14 or i == 16 or i == 18 or i == 20:
                pass
            elif i == 1:
                splitted_line = line.split(" ")
                height = int(splitted_line[0])
                width = int(splitted_line[1])
            elif i == 3:
                splitted_line = line.split("|")
                for element in splitted_line:
                    obstacles.append(eval(element))
            elif i == 5:
                splitted_line = line.split("|")
                for element in splitted_line:
                    splitted_element = element.split(":")
                    cell = eval(splitted_element[0])
                    value = eval(splitted_element[1])
                    goal_states.append((cell, value))
            elif i == 7:
                start_state = eval(line)
            elif i == 9:
                reward = eval(line)
            elif i == 11:
                b = eval(line)
            elif i == 12:
                a = eval(line)
            elif i == 13:
                c = eval(line)
            elif i == 15:
                learning_rate = eval(line)
            elif i == 17:
                gamma = eval(line)
            elif i == 19:
                epsilon = eval(line)
            elif i == 21:
                episode_count = eval(line)
            
            i+=1
 
    return MDPDefinition(height, width, obstacles, goal_states, start_state, reward, b, a, c, learning_rate, gamma, epsilon, episode_count)

# This is a class representing an entry in the Q table.
class QCell:
    def __init__(self, i=None, j=None, left=None, up=None, right=None, down=None) -> None:
        self.i = i
        self.j = j
        self.left = left
        self.up = up
        self.right = right
        self.down = down
    
    def __str__(self) -> str:
        return f" (i:{self.i}, j:{self.j}:, left:{self.left}, up:{self.up}, right:{self.right}, down:{self.down}) "

# This is a class representing an entry in the U table.
class UCell:
    def __init__(self, i=None, j=None, value=None, action=None) -> None:
        self.i = i
        self.j = j
        self.value = value
        self.action = action
    
    def __str__(self) -> str:
        return f" (i:{self.i}, j:{self.j}:, value:{self.value}, action:{self.action}) "
    
# This function is responsible for checking if a state is terminal state or not.
def isTerminalState(terminal_states, state):
    flag = False
    for terminal_state in terminal_states:
        terminal_state_itself = terminal_state[0]
        terminal_state_reward = terminal_state[1]
        if terminal_state_itself == state:
            flag = True
            break
    return flag

# This function is responsible for applying noise to an action
def getActionAfterNoiseApplied(selected_action, COUNTER_CLOCKWISE_PROBABILITY, FORWARD_PROBABILITY, CLOCKWISE_PROBABILITY):
    rand_num = random.random()  # Generates a random number between 0 and 1
    event = None
    if rand_num < COUNTER_CLOCKWISE_PROBABILITY:
        event = "ccw"
    elif rand_num < COUNTER_CLOCKWISE_PROBABILITY + FORWARD_PROBABILITY:
        event = "forward" 
    else:
        event = "cw"

    if event == "forward":
        return selected_action
    elif event == "ccw":
        if selected_action == UP_ACTION:
            return LEFT_ACTION
        elif selected_action == LEFT_ACTION:
            return DOWN_ACTION
        elif selected_action == DOWN_ACTION:
            return RIGHT_ACTION
        elif selected_action == RIGHT_ACTION:
            return UP_ACTION
    elif event ==  "cw":
        if selected_action == UP_ACTION:
            return RIGHT_ACTION
        elif selected_action == RIGHT_ACTION:
            return DOWN_ACTION
        elif selected_action == DOWN_ACTION:
            return LEFT_ACTION
        elif selected_action == LEFT_ACTION:
            return UP_ACTION

# This function returns a random action such that each action is equally likely.
def getRandomAction():
    rand_num = random.random()  # Generates a random number between 0 and 1
    if rand_num <= 0.25:
        return LEFT_ACTION
    elif rand_num <= 0.5:
        return UP_ACTION
    elif rand_num <= 0.75:
        return RIGHT_ACTION
    else:
        return DOWN_ACTION

# This function is responsible for checking if a state is obstacle state or not.
def isObstacleState(obstacles, state):
    flag = False
    for obstacle in obstacles:
        if obstacle == state:
            flag = True
            break
    return flag

def SolveMDP(problem_file_name, random_seed):
    # Set the seed value
    random.seed(random_seed)

    # Load the problem parameters.
    parsed_input = inputParser(problem_file_name)
    HEIGHT = parsed_input.height
    WIDTH = parsed_input.width
    OBSTACLES = parsed_input.obstacles
    GOAL_STATES = parsed_input.goal_states
    START_STATE = parsed_input.start_state
    REWARD = parsed_input.reward
    COUNTER_CLOCKWISE_PROBABILITY = parsed_input.action_noise[0]
    FORWARD_PROBABILITY = parsed_input.action_noise[1] 
    CLOCKWISE_PROBABILITY = parsed_input.action_noise[2]
    LEARNING_RATE = parsed_input.learning_rate
    GAMMA = parsed_input.gamma
    EPSILON = parsed_input.epsilon
    EPISODE_COUNT = parsed_input.episode_count

    # Create the Q table.
    QTABLE = []
    for i in range(HEIGHT):
        temp = []
        for j in range(WIDTH):
            temp.append(QCell(i=i, j=j))
        QTABLE.append(temp)

    # Initialize values of the Q table.
    for i in range(HEIGHT):
        for j in range(WIDTH):
            if isTerminalState(GOAL_STATES, (i,j)):
                # If (i,j) is a terminal state then, initialize its q values to 0.
                QTABLE[i][j].left = 0
                QTABLE[i][j].up = 0
                QTABLE[i][j].right = 0
                QTABLE[i][j].down = 0
            else:
                # If (i,j) is a non-terminal state then, initialize its q values arbitrarily.
                # I will initialize them by taking random values from the uniform distribution between 0 and 1.
                QTABLE[i][j].left = random.uniform(0, 1)
                QTABLE[i][j].up = random.uniform(0, 1)
                QTABLE[i][j].right = random.uniform(0,1)
                QTABLE[i][j].down = random.uniform(0,1)
     
    # Loop for each episode
    for episode in range(EPISODE_COUNT):
        # Initialize agent's current state to start state
        current_state = START_STATE
        
        # Until current_state is a terminal state.
        while isTerminalState(GOAL_STATES, current_state) == False:
            # Choose an action using epsilon-gready method.
            selected_action = None
            rand_num = random.random()  # Generates a random number between 0 and 1.
            if rand_num < EPSILON: # Exploration (i.e. choose a random action from the action space).
                selected_action = getRandomAction()
            else: # Exploitation (i.e. choose the action with the highest estimated reward).
                # Choose the action with the highest Q value.
                i = current_state[0]
                j = current_state[1]
                if QTABLE[i][j].left >= QTABLE[i][j].up and QTABLE[i][j].left >= QTABLE[i][j].right and QTABLE[i][j].left >= QTABLE[i][j].down:
                    selected_action = LEFT_ACTION
                elif QTABLE[i][j].up >= QTABLE[i][j].left and QTABLE[i][j].up >= QTABLE[i][j].right and QTABLE[i][j].up >= QTABLE[i][j].down:
                    selected_action = UP_ACTION
                elif QTABLE[i][j].down >= QTABLE[i][j].left and QTABLE[i][j].down >= QTABLE[i][j].right and QTABLE[i][j].down >= QTABLE[i][j].up:
                    selected_action = DOWN_ACTION
                else:
                    selected_action = RIGHT_ACTION

            # Simulate action noise problem. 
            selected_action = getActionAfterNoiseApplied(selected_action, COUNTER_CLOCKWISE_PROBABILITY, FORWARD_PROBABILITY, CLOCKWISE_PROBABILITY)

            # Apply selected action (after noise applied) to current state to obtaine the next state.
            aimed_state = None # This is the state that is intended to be reached applying the action but it might not be reached (e.g. obstacles or out of border problem)
            if selected_action == UP_ACTION:
                aimed_i = current_state[0] - 1
                aimed_j = current_state[1]
                aimed_state = (aimed_i, aimed_j)
            elif selected_action == LEFT_ACTION:
                aimed_i = current_state[0]
                aimed_j = current_state[1] - 1
                aimed_state = (aimed_i, aimed_j)
            elif selected_action == RIGHT_ACTION:
                aimed_i = current_state[0] 
                aimed_j = current_state[1] + 1
                aimed_state = (aimed_i, aimed_j)
            elif selected_action == DOWN_ACTION:
                aimed_i = current_state[0] + 1
                aimed_j = current_state[1]
                aimed_state = (aimed_i, aimed_j)

            # Now check if the aimed state is valid. That is, if it is not obstacle or out of border, then go there. Otherwise, stay in the current state.
            next_state = None
            if isObstacleState(OBSTACLES, aimed_state) == True: # Aimed state is an obstacle, so stay in the current state.
                next_state = current_state
            elif aimed_state[0] < 0 or aimed_state[1] < 0 or aimed_state[0] >= HEIGHT or aimed_state[1] >= WIDTH: # Aimed state is a out of border, so stay in the current state.
                next_state = current_state  
            else: # Aimed state is valid, so go there.
                next_state = aimed_state

            # Calculate the reward.
            reward = None
            if isTerminalState(GOAL_STATES, next_state): # If it is a goal state, then the reward is the reward of the goal state.
                for goal_state in GOAL_STATES:
                    goal_state_itself = goal_state[0]
                    goal_state_reward = goal_state[1]
                    if goal_state_itself == next_state:
                        reward = goal_state_reward
                        break
            else: # If it is a normal (non-terminal) state, the reward is the default reward.
                reward = REWARD

            
            # Update Q table.
            if selected_action == UP_ACTION:
                QTABLE_current_state_cell = QTABLE[current_state[0]][current_state[1]]
                QTABLE_next_state_cell = QTABLE[next_state[0]][next_state[1]]
                max_a_Q_S_prime_a = max([QTABLE_next_state_cell.left, QTABLE_next_state_cell.up, QTABLE_next_state_cell.right, QTABLE_next_state_cell.down])
                Q_S_A = QTABLE_current_state_cell.up
                QTABLE[current_state[0]][current_state[1]].up = Q_S_A + LEARNING_RATE * (reward + GAMMA * max_a_Q_S_prime_a - Q_S_A)
            elif selected_action == LEFT_ACTION:
                QTABLE_current_state_cell = QTABLE[current_state[0]][current_state[1]]
                QTABLE_next_state_cell = QTABLE[next_state[0]][next_state[1]]
                max_a_Q_S_prime_a = max([QTABLE_next_state_cell.left, QTABLE_next_state_cell.up, QTABLE_next_state_cell.right, QTABLE_next_state_cell.down])
                Q_S_A = QTABLE_current_state_cell.left
                QTABLE[current_state[0]][current_state[1]].left = Q_S_A + LEARNING_RATE * (reward + GAMMA * max_a_Q_S_prime_a - Q_S_A)
            elif selected_action == RIGHT_ACTION:
                QTABLE_current_state_cell = QTABLE[current_state[0]][current_state[1]]
                QTABLE_next_state_cell = QTABLE[next_state[0]][next_state[1]]
                max_a_Q_S_prime_a = max([QTABLE_next_state_cell.left, QTABLE_next_state_cell.up, QTABLE_next_state_cell.right, QTABLE_next_state_cell.down])
                Q_S_A = QTABLE_current_state_cell.right
                QTABLE[current_state[0]][current_state[1]].right = Q_S_A + LEARNING_RATE * (reward + GAMMA * max_a_Q_S_prime_a - Q_S_A)
            elif selected_action == DOWN_ACTION:
                QTABLE_current_state_cell = QTABLE[current_state[0]][current_state[1]]
                QTABLE_next_state_cell = QTABLE[next_state[0]][next_state[1]]
                max_a_Q_S_prime_a = max([QTABLE_next_state_cell.left, QTABLE_next_state_cell.up, QTABLE_next_state_cell.right, QTABLE_next_state_cell.down])
                Q_S_A = QTABLE_current_state_cell.down
                QTABLE[current_state[0]][current_state[1]].down = Q_S_A + LEARNING_RATE * (reward + GAMMA * max_a_Q_S_prime_a - Q_S_A)

            # Update the current state
            current_state = next_state

    # Now, we obtained the q values for each state. Calculate, utility values (U = max Q(s,a))).
    # Create the table.
    UTABLE = []
    for i in range(HEIGHT):
        temp = []
        for j in range(WIDTH):
            temp.append(UCell(i=i, j=j))
        UTABLE.append(temp)
    # Fill the table by taking the max of Q values.
    for i in range(HEIGHT):
        for j in range(WIDTH):
            if QTABLE[i][j].left >= QTABLE[i][j].up and QTABLE[i][j].left >= QTABLE[i][j].down and QTABLE[i][j].left >= QTABLE[i][j].right:
                UTABLE[i][j].value = QTABLE[i][j].left
                UTABLE[i][j].action = LEFT_ACTION
            elif QTABLE[i][j].up >= QTABLE[i][j].left and QTABLE[i][j].up >= QTABLE[i][j].down and QTABLE[i][j].up >= QTABLE[i][j].right:
                UTABLE[i][j].value = QTABLE[i][j].up
                UTABLE[i][j].action = UP_ACTION
            elif QTABLE[i][j].right >= QTABLE[i][j].left and QTABLE[i][j].right >= QTABLE[i][j].down and QTABLE[i][j].right >= QTABLE[i][j].up:
                UTABLE[i][j].value = QTABLE[i][j].right
                UTABLE[i][j].action = RIGHT_ACTION
            else:
                UTABLE[i][j].value = QTABLE[i][j].down
                UTABLE[i][j].action = DOWN_ACTION

    # Construct return values.
    U = dict()
    policy = dict()
    for i in range(HEIGHT):
        for j in range(WIDTH):
            if isTerminalState(GOAL_STATES, (i,j)) == False and isObstacleState(OBSTACLES, (i,j)) == False:
                U[(i,j)] = UTABLE[i][j].value
                policy[(i,j)] = UTABLE[i][j].action

    return U, policy
