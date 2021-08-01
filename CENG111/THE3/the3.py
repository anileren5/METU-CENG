#ANIL EREN GÖÇER
#CENG111-THE3
#2448397

def is_basic(part):
    """This function checks if the part is basic part or not. Actually, the function checks if the node is leaf or not."""
    if type(part[-1])==float:
        return True
    return False
def price(tree):
    """This function takes a tree which is processed by mytree function and calculates the price of node."""
    if type(tree[-1])==float:
        return tree[0]*tree[-1]
    else:
        sum=0
        children=tree[2:]
        for child in children:
            sum+=price(child)
        return sum*tree[0]
def mytree(part_list):
    """This function convert the given input into the tree format to be used in other functions."""
    basics=[]
    for i in part_list:
        if is_basic(i):
            basics.append(i)
    for i in basics:
        part_list.remove(i)

    for basic in basics:
        for part in part_list:
            for i in range(1,len(part)):
                if part[i][1]==basic[0]:
                    part[i]=part[i]+tuple([basic[1]])
    next=[]
    while len(part_list)>1:
        for part in part_list:
            state=True
            for subpart in part[1:]:
                if len(subpart)<3:
                    state=False
            if state==True:
                next.append(part)
        for i in next:
            part_list.remove(i)
        for item in next:
            for part in part_list:
                for i in range(1,len(part)):
                    if item[0]==part[i][1]:
                        part[i]=part[i]+tuple(item[1:])
        next=[]
    return (1,)+tuple(part_list[0])
def mycalculate_price(part_list):
    """This function takes the given input and calculates its price except one node trees."""
    tree=mytree(part_list)
    return price(tree)
def quantities_of_basics(tree):
    """This function takes a tree which is result of mytree and returns the quantities of the basic parts as a list."""
    results=[]
    tree=list(tree)
    for i in range(2,len(tree)):
        if type(tree[i][-1])==float:
            tree[i]=tuple([tree[0]*tree[i][0]])+tree[i][1:]
            results.append(tree[i][:-1])
        else:
            tree[i] = tuple([tree[0] * tree[i][0]]) + tree[i][1:]

            results=results+quantities_of_basics(tree[i])
    return results
def myrequired_parts(part_list):
    """This function takes the given input and return quantities of basic parts except one node trees."""
    tree=mytree(part_list)
    return quantities_of_basics(tree)
def mystock_check(part_list,stock_list):
    """This function checks the stocks via myrequired_parts and returns the quantities of short parts as a list except one node trees."""
    needed_list=myrequired_parts(part_list)
    results=[]
    for needed_part in needed_list:
        for stocked_part in stock_list:
            if needed_part[1]==stocked_part[1]:
                if needed_part[0]>stocked_part[0]:
                    results.append((needed_part[1],needed_part[0]-stocked_part[0]))
    needded_names=[]
    needed_quantity=[]
    stock_names=[]
    for (quantity,name) in needed_list:
        needded_names.append(name)
        needed_quantity.append(quantity)
    for (quantity,name) in stock_list:
        stock_names.append(name)
    for name in needded_names:
        if name not in stock_names:
            results.append((name,needed_quantity[needded_names.index(name)]))
    return results

def calculate_price(part_list):
    """These functions can also process one node trees."""
    if len(part_list)==1 and type(part_list[0][-1])==float:
        return part_list[0][1]
    return mycalculate_price(part_list)
def required_parts(part_list):
    """These functions can also process one node trees"""
    if len(part_list)==1 and type(part_list[0][-1])==float :
        name=part_list[0][0]
        return [(1,name)]
    return myrequired_parts(part_list)
def stock_check(part_list,stock_list):
    """These functions can also process one node trees."""
    if len(part_list)==1 and type(part_list[0][-1])==float:
        part=part_list[0][0]
        names=[]
        for (n,name) in stock_list:
            names.append(name)
        if part in names:
            return []
        if part not in names:
            return [(part,1)]

    return mystock_check(part_list,stock_list)

