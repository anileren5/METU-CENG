# Global variables
minimax_number_of_nodes = 0
alpha_beta_number_of_nodes = 0

class NodeState:
    def __init__(self, piles, node_type, parent=None, value=None, children=[], value2=None):
        self.piles = piles
        self.node_type = node_type # takes value of "min" or max
        self.parent = parent
        self.value = value  # takes value of 1, -1 or None if the value is not determined yet
        self.children = children
        self.value2 = value2

    def __str__(self) -> str:
        return f"{self.piles}, {self.node_type}, {self.value}"

    def add_child(self, child):
        self.children.append(child)

def generateNextStates(currentState):
    temp1 = currentState.copy()
    temp2 = currentState.copy()
    nextStates = []
    n_piles = len(currentState)
    for i in range(n_piles):
        pile_size = currentState[i]
        for j in range(pile_size):
            temp1[i] = j
            nextStates.append(temp1.copy())
            temp1[i] = temp2[i]
    return nextStates

def minimax_number_of_nodes_calculator_helper(state):
    global minimax_number_of_nodes
    minimax_number_of_nodes += 1
    nextStates = generateNextStates(state)
    for nextState in nextStates:
        minimax_number_of_nodes_calculator_helper(nextState)

def minimax_number_of_nodes_calculator(state):
    minimax_number_of_nodes_calculator_helper(state)
    return minimax_number_of_nodes - 1

def build_tree(node):
    if sum(node.piles) == 0:
        if node.node_type == "min":
            node.value = -1
            node.value2 = -1
        elif node.node_type == "max":
            node.value = 1
            node.value2 = 1
    childrenTurn = "max" if node.node_type == "min" else "min"
    childrenPiles = generateNextStates(node.piles)
    for child in childrenPiles:
        unbuiltChild = NodeState(child, childrenTurn, node, None, [])
        node.add_child(build_tree(unbuiltChild))
    node.value = get_node_value(node)
    return node

def get_node_value(node):
    if node.value != None:
        return node.value
    else:
        children_values = []
        for child in node.children:
            children_values.append(get_node_value(child))
        if node.node_type == "max":
            node.value = max(children_values)
        elif node.node_type == "min":
            node.value = min(children_values)
        return node.value

def get_optimal_next_state(node):
    # If there are multiple optimal next state, get the left-most one
    for child in node.children:
        if node.value == child.value:
            return child
        
def minimax(game):
    root = build_tree(NodeState(game, "max", None, None, []))
    next_configuration = get_optimal_next_state(root).piles
    number_of_nodes = minimax_number_of_nodes_calculator(next_configuration)
    return (next_configuration, number_of_nodes)

def max_value_with_pruning(node, alpha, beta):
    global alpha_beta_number_of_nodes
    alpha_beta_number_of_nodes += 1
    if len(node.children) == 0:
        return node.value2
    v = float("-inf")
    for child in node.children:
        v_prime = min_value_with_pruning(child, alpha, beta)
        v = max(v, v_prime)
        if v >= beta:
            return v
        alpha = max(alpha, v)  # Update alpha
        if v == 1:
            return v  # Early termination if max utility is found
    return v

def min_value_with_pruning(node, alpha, beta):
    global alpha_beta_number_of_nodes
    alpha_beta_number_of_nodes += 1
    if len(node.children) == 0:
        return node.value2
    v = float("inf")
    for child in node.children:
        v_prime = max_value_with_pruning(child, alpha, beta)
        v = min(v, v_prime)
        if v <= alpha:
            return v
        beta = min(beta, v)  # Update beta
        if v == -1:
            return v  # Early termination if min utility is found
    return v

def alpha_beta_number_of_nodes_calculator(state):
    min_value_with_pruning(state, float("-inf"), float("inf"))
    return alpha_beta_number_of_nodes - 1

def alpha_beta(game):
    root = build_tree(NodeState(game, "max", None, None, []))
    next_configuration_as_node = get_optimal_next_state(root)
    next_configuration = next_configuration_as_node.piles
    number_of_nodes = alpha_beta_number_of_nodes_calculator(next_configuration_as_node)
    return (next_configuration, number_of_nodes)

def SolveGame(method_name, game):
    if method_name == "Minimax":
        global minimax_number_of_nodes
        result = minimax(game)
        minimax_number_of_nodes = 0
        return result
    elif method_name == "AlphaBeta":
        global alpha_beta_number_of_nodes
        result = alpha_beta(game)
        alpha_beta_number_of_nodes = 0
        return result
