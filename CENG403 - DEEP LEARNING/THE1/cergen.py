# CENG403 - Spring 2024 - THE1

import random

# Helpers (functions, classes etc.)
# Much of the error handling is done in the cergen class, here are the helper functions assuming perfect inputs.

def create_tensor(shape, generator, *args): # Creates a tensor (nested array) with the given shape using the given entry generator.
    if len(shape) == 0:
        return generator(*args)
    else:
        return [create_tensor(shape[1:], generator, *args) for _ in range(shape[0])]

def uniform_generator_int(lower_bound, upper_bound): # Generates an integer from the uniform distribution in the interval [lower_bound, upper_bound].
    return random.randint(lower_bound, upper_bound)

def uniform_generator_float(lower_bound, upper_bound): # Generates a real number from the uniform distribution in the interval [lower_bound, upper_bound].
    return random.uniform(lower_bound, upper_bound)

def calculate_shape(tensor): # Given a tensor (nested array), it calculates its shape as a tuple.
    if not isinstance(tensor, list):
        return ()
    shape = [len(tensor)]
    sub_tensor = calculate_shape(tensor[0])
    if sub_tensor:
        shape.extend(sub_tensor)
    return tuple(shape)

def calculate_total_elements(shape): # Given a shape, it calculate how many elements are there in that tensor.
    total = 1
    for dimension in shape:
        total *= dimension
    return total

def fold_vector(arr, new_shape): # Constructs a new tensor with the given shape from the given vector (list).
    old_shape = calculate_shape(arr)
    if (calculate_total_elements(old_shape) != calculate_total_elements(new_shape)):
        raise ValueError(f"Dimension mismatch occured: {old_shape} and {new_shape} do not match!")

    def fold_recursively(arr, new_shape):
        if len(new_shape) == 1:
            return arr
        folded = []
        block_size = len(arr) // new_shape[0]
        for _ in range(new_shape[0]):
            folded.append(fold_recursively(arr[:block_size], new_shape[1:]))
            arr = arr[block_size:]
        return folded

    return fold_recursively(arr, new_shape)

def flatten_tensor(tensor): # Constructs a vector from a tensor (possibly higher dimensional).
    def flatten_recursively(tensor, flattened_vector):
        if isinstance(tensor, list):
            for item in tensor:
                flatten_recursively(item, flattened_vector)
        else:
            flattened_vector.append(tensor)

    flattened_vector = []
    flatten_recursively(tensor, flattened_vector)
    return flattened_vector

def calculate_element_wise_binary(tensor1, tensor2, binary_operation): # Apply a binary operation to two tensor element-wise and returns the resulting tensor.
    shape = calculate_shape(tensor1)
    vector1 = flatten_tensor(tensor1)
    vector2 = flatten_tensor(tensor2)
    result_vector = [binary_operation(x,y) for x, y in zip(vector1, vector2)]
    return fold_vector(result_vector, shape)

def calculate_element_wise_unary(tensor, unary_operation): # Apply a binary operation to a tensor element-wise and returns the resulting the tensor.
    shape = calculate_shape(tensor)
    vector = flatten_tensor(tensor)
    result_vector = [unary_operation(x) for x in vector]
    return fold_vector(result_vector, shape)

def reshape_tensor(tensor, new_shape): # Constructs a new tensor with the given shape.
    vector = flatten_tensor(tensor)
    return fold_vector(vector, new_shape)

def is_scalar(data): # Checks is the given data is a scalar.
    return (isinstance(data, int) or isinstance(data, float))

def tensor2str(tensor, level=0): # Constructs a string from the tensor for printing it.
    if isinstance(tensor[0], list):
        string_rep = "["
        for i, item in enumerate(tensor):
            if i > 0:
                string_rep += "\n" + " " * (level)
            string_rep += tensor2str(item, level + 1)
        string_rep += "]"
        if level > 0:
            string_rep += "\n"
        return string_rep
    else:
        return "[" + " ".join(map(str, tensor)) + "]"

def safe_division(x, y): # Divides x by y and raises an error if y is zero.
    if y == 0:
        raise ZeroDivisionError("attempted to divide by zero !")
    else:
        return float(x) / float(y)

def mul_tuple(arr): # Calculates multiplication of all the elements in a tuple.
    s = 1
    for item in arr:
        s = s * item
    return s

def calculate_reduced_shape(old_shape, axis): # Removes the element at the index = axis. For example, if old_shape = (0,1,2,3,4) and axis = 3 it returns (0,1,2,4).
    new_shape = []
    for i in range(len(old_shape)):
        if i != axis:
            new_shape.append(old_shape[i])
    return new_shape

def sum_along_axis(tensor, axis): # Calculates the sum along the axis and returns the tensor reduced by that axis for which sum will be calculated.
    # ---------- Edge Case ----------
    if is_scalar(tensor[0]):
        return sum(tensor)
    # ---------- --------- ----------

    shape = calculate_shape(tensor)

    exclusive_left_factor = mul_tuple(shape[:axis])
    exclusive_right_factor = mul_tuple(shape[axis+1:])
    self_factor = shape[axis]
    inclusive_right_factor = exclusive_right_factor * self_factor

    vector = flatten_tensor(tensor)
    result_vector = []

    star = 0
    for i in range(exclusive_left_factor):
        for j in range(exclusive_right_factor):
            cursor = star + j
            current_element_sum = 0
            for k in range(self_factor):
                current_element_sum += vector[cursor]
                cursor += exclusive_right_factor
            result_vector.append(current_element_sum)
        star += inclusive_right_factor

    new_shape = calculate_reduced_shape(shape, axis)
    return reshape_tensor(result_vector, new_shape)

def average_along_axis(tensor, axis): # Calculates the average along the axis and returns the tensor reduced by that axis for which average will be calculated.
    # ---------- Edge Case ----------
    if is_scalar(tensor[0]):
        return sum(tensor) / len(tensor)
    # ---------- --------- ----------

    shape = calculate_shape(tensor)

    exclusive_left_factor = mul_tuple(shape[:axis])
    exclusive_right_factor = mul_tuple(shape[axis+1:])
    self_factor = shape[axis]
    inclusive_right_factor = exclusive_right_factor * self_factor

    vector = flatten_tensor(tensor)
    result_vector = []

    star = 0
    for i in range(exclusive_left_factor):
        for j in range(exclusive_right_factor):
            cursor = star + j
            current_element_sum = 0
            for k in range(self_factor):
                current_element_sum += vector[cursor]
                cursor += exclusive_right_factor
            result_vector.append(current_element_sum / self_factor)
        star += inclusive_right_factor

    new_shape = calculate_reduced_shape(shape, axis)
    return reshape_tensor(result_vector, new_shape)

def increment_counter(current_counter, bounds): # bounds parameter actually represents a shape. say it is [4,3,2]. and this function increments as follows: [0,0,0] -> [0,0,1] -> [0,1,0] -> [0,1,1] -> [0,2,0] -> .... [3,2,1]
    l = len(current_counter)
    carry = False
    for i in range(1,l+1):
        if i == 1:
            current_counter[-i] = current_counter[-i] + 1
        else:
            current_counter[-i] = current_counter[-i] + int(carry)
            if carry:
                carry = False

        if current_counter[-i] == bounds[-i]:
            current_counter[-i] = 0
            carry = True
    return current_counter

def calculate_digit_values(shape): # Calculates the digit value (basamak değeri in turkish) of the each digit in counter above mentioned in increment counter. For example, if the shape (4,3,2) the digit values are (6,2,1). As another example, if the shape is (5,4,3,2) then digit values are (24,6,2,1)
    digit_values = []
    l = len(shape)
    for i in range(l):
        if i == 0:
            digit_values.append(1)
        else:
            digit_values.append(shape[-i] * digit_values[-1])
    return tuple(digit_values[::-1])

def calculate_linear_index(current_counter, shape): # Transforms the current counter which keeps track of an element in the higher dimensional tensor into an index in its flattened form which is a vector.
    digit_values = calculate_digit_values(shape)
    index = 0
    l = len(shape)
    for i in range(l):
        index += current_counter[i] * digit_values[i]
    return index

def calculate_transpose(tensor): # Calculates the transpose of a tensor (nested array).
    if is_scalar(tensor):
        return tensor

    vector = flatten_tensor(tensor)
    transpose_vector = len(vector) * [-1]
    shape = calculate_shape(tensor)
    transpose_shape = shape[::-1]

    counter = len(shape) * [0]

    n_entries = calculate_total_elements(shape)

    for i in range(n_entries):
        transpose_counter = counter[::-1]
        index = calculate_linear_index(counter, shape)
        transpose_index = calculate_linear_index(transpose_counter, transpose_shape)
        transpose_vector[transpose_index] = vector[index]
        counter = increment_counter(counter, shape)

    return reshape_tensor(transpose_vector, transpose_shape)

# Operation class implementation:

# I did not utilize the operation class.
class Operation:
    def __call__(self, *operands):
        """
        Makes an instance of the Operation class callable.
        Stores operands and initializes outputs to None.
        Invokes the forward pass of the operation with given operands.

        Parameters:
            *operands: Variable length operand list.

        Returns:
            The result of the forward pass of the operation.
        """
        self.operands = operands
        self.outputs = None
        return self.ileri(*operands)

    def ileri(self, *operands):
        """
        Defines the forward pass of the operation.
        Must be implemented by subclasses to perform the actual operation.

        Parameters:
            *operands: Variable length operand list.

        Raises:
            NotImplementedError: If not overridden in a subclass.
        """
        raise NotImplementedError

import math
from typing import Union

class gergen:

    __veri = None  # A nested list of numbers representing the data
    D = None       # Transpose of data
    __boyut = None # Dimensions of the derivative (Shape)


    def __init__(self, veri=None):
        self.__veri = veri
        self.__boyut = calculate_shape(self.__veri)
        self.D = calculate_transpose(self.__veri)

    def __getitem__(self, index):
        return gergen(self.__veri[index])

    def __str__(self):
        if is_scalar(self.__veri):
            return "O boyutlu skalar gergen:\n" + str(self.__veri)
        elif len(self.__boyut) == 1 and self.__boyut[0] != 1:
            return f"1x{self.__boyut[0]} boyutlu gergen:\n" + str(self.__veri) 
        else:
            shape = calculate_shape(self.__veri)
            shape = "x".join([str(dim) for dim in shape])
            return shape + " boyutlu gergen:\n" + tensor2str(self.__veri)

    def __mul__(self, other: Union['gergen', int, float]) -> 'gergen':
        if isinstance(other, gergen):
            shape_veri = calculate_shape(self.__veri)
            shape_other = calculate_shape(other.listeye())
            other_veri = other.listeye()

            if (not is_scalar(self.__veri) and not is_scalar(other_veri)) & (shape_veri != shape_other):
                raise ValueError(f"Dimension mismatch occured: {shape_veri} and {shape_other} do not match!")

            if is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(self.__veri * other_veri)
            elif is_scalar(self.__veri) and not is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(other_veri, lambda x: x * self.__veri))
            elif not is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x * other_veri))
            else:
                return gergen(calculate_element_wise_binary(self.__veri, other_veri, lambda x, y : x * y))

        elif is_scalar(other):
            if is_scalar(self.__veri):
                return gergen(self.__veri * other)
            else:
                return gergen(calculate_element_wise_unary(self.__veri, lambda x : other * x))
        else:
            raise TypeError("other parameter must be either a gergen object or a numerical scalar (i.e. int or float) !")

    def __rmul__(self, other: Union['gergen', int, float]) -> 'gergen':
        if isinstance(other, gergen):
            shape_veri = calculate_shape(self.__veri)
            shape_other = calculate_shape(other.listeye())
            other_veri = other.listeye()

            if (not is_scalar(self.__veri) and not is_scalar(other_veri)) & (shape_veri != shape_other):
                raise ValueError(f"Dimension mismatch occured: {shape_veri} and {shape_other} do not match!")

            if is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(self.__veri * other_veri)
            elif is_scalar(self.__veri) and not is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(other_veri, lambda x: x * self.__veri))
            elif not is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x * other_veri))
            else:
                return gergen(calculate_element_wise_binary(self.__veri, other_veri, lambda x, y : x * y))
        elif is_scalar(other):
            if is_scalar(self.__veri):
                return gergen(self.__veri * other)
            else:
                return gergen(calculate_element_wise_unary(self.__veri, lambda x : other * x))
        else:
            raise TypeError("other parameter must be either a gergen object or a numerical scalar (i.e. int or float) !")

    def __truediv__(self, other: Union['gergen', int, float]) -> 'gergen': # Zero division check is handled in safe_division helper function.
        if isinstance(other, gergen):
            shape_veri = calculate_shape(self.__veri)
            shape_other = calculate_shape(other.listeye())
            other_veri = other.listeye()

            if (not is_scalar(self.__veri) and not is_scalar(other_veri)) & (shape_veri != shape_other):
                raise ValueError(f"Dimension mismatch occured: {shape_veri} and {shape_other} do not match!")

            if is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(safe_division(self.__veri, other))
            elif is_scalar(self.__veri) and not is_scalar(other_veri):
                raise TypeError("Attempt to divide a scalar by a non-scalar value!")
            elif not is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: safe_division(x, other_veri)))
            else:
                return gergen(calculate_element_wise_binary(self.__veri, other_veri, lambda x, y : safe_division(x, y)))


        elif is_scalar(other):
            if is_scalar(self.__veri):
                return gergen(safe_division(self.__veri, other))
            else:
                return gergen(calculate_element_wise_unary(self.__veri, lambda x : safe_division(x, other)))
        else:
            raise TypeError("other parameter must be either a gergen object or a numerical scalar (i.e. int or float) !")

    def __add__(self, other: Union['gergen', int, float]) -> 'gergen':
        if isinstance(other, gergen):
            shape_veri = calculate_shape(self.__veri)
            shape_other = calculate_shape(other.listeye())
            other_veri = other.listeye()

            if (not is_scalar(self.__veri) and not is_scalar(other_veri)) & (shape_veri != shape_other):
                raise ValueError(f"Dimension mismatch occured: {shape_veri} and {shape_other} do not match!")

            if is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(self.__veri + other_veri)
            elif is_scalar(self.__veri) and not is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(other_veri, lambda x: x + self.__veri))
            elif not is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x + other_veri))
            else:
                return gergen(calculate_element_wise_binary(self.__veri, other_veri, lambda x, y : x + y))

        elif is_scalar(other):
            if is_scalar(self.__veri):
                return gergen(self.__veri + other)
            else:
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x + other))
        else:
            raise TypeError("other parameter must be either a gergen object or a numerical scalar (i.e. int or float) !")

    def __sub__(self, other: Union['gergen', int, float]) -> 'gergen':
        if isinstance(other, gergen):
            shape_veri = calculate_shape(self.__veri)
            shape_other = calculate_shape(other.listeye())
            other_veri = other.listeye()

            if (not is_scalar(self.__veri) and not is_scalar(other_veri)) & (shape_veri != shape_other):
                raise ValueError(f"Dimension mismatch occured: {shape_veri} and {shape_other} do not match!")

            if is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(self.__veri - other_veri)
            elif is_scalar(self.__veri) and not is_scalar(other_veri):
                raise TypeError("Attempt to subtract a non-scalar value from a scalar-value!")
            elif not is_scalar(self.__veri) and is_scalar(other_veri):
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x - other_veri))
            else:
                return gergen(calculate_element_wise_binary(self.__veri, other_veri, lambda x, y : x - y))

        elif is_scalar(other):
            if is_scalar(self.__veri):
                return gergen(self.__veri - other)
            else:
                return gergen(calculate_element_wise_unary(self.__veri, lambda x: x - other))
        else:
            raise TypeError("other parameter must be either a gergen object or a numerical scalar (i.e. int or float) !")

    def uzunluk(self):
        # Returns the total number of elements in the gergen
        if is_scalar(self.__veri):
            return 1
        else:
            return calculate_total_elements(self.__boyut)

    def boyut(self):
        # Returns the shape of the gergen
        return self.__boyut

    def devrik(self):
        # Returns the transpose of gergen
        return gergen(self.D)

    def sin(self):
        # Calculates the sine of each element in the given `gergen`.
        if is_scalar(self.__veri):
            return gergen(math.sin(self.__veri))
        else:
            return gergen(calculate_element_wise_unary(self.__veri, lambda x: math.sin(x)))

    def cos(self):
        # Calculates the cosine of each element in the given `gergen`.
        if is_scalar(self.__veri):
            return gergen(math.cos(self.__veri))
        else:
            return gergen(calculate_element_wise_unary(self.__veri, lambda x: math.cos(x)))

    def tan(self):
        # Calculates the tangent of each element in the given `gergen`.
        if is_scalar(self.__veri):
            return gergen(math.tan(self.__veri))
        else:
            return gergen(calculate_element_wise_unary(self.__veri, lambda x: math.tan(x)))

    def us(self, n: int):
        # Raises each element of the gergen object to the power 'n'. This is an element-wise operation.
        if n < 0:
            return ValueError("n should be positive!")

        if is_scalar(self.__veri):
            return gergen(self.__veri**n)
        else:
            return gergen(calculate_element_wise_unary(self.__veri, lambda x: x**n))

    def log(self):
        # Applies the logarithm function to each element of the gergen object, using the base 10.
        def safe_log(x):
            if x <= 0:
                raise ValueError(str(x) + " is not in the domain of logarithmic functions since it is not positive!")
            else:
                return math.log10(x)

        if is_scalar(self.__veri):
            return gergen(safe_log(self.__veri))
        else:
            return gergen(calculate_element_wise_unary(self.__veri, safe_log))

    def ln(self):
        # Applies the natural logarithm function to each element of the gergen object.
        def safe_ln(x):
            if x <= 0:
                raise ValueError(str(x) + " is not in the domain of logarithmic (specifically ln) functions since it is not positive!")
            else:
                return math.log(x)

        if is_scalar(self.__veri):
            return gergen(safe_ln(self.__veri))
        else:
            return gergen(calculate_element_wise_unary(self.__veri, safe_ln))

    def L1(self):
        # Calculates and returns the L1 norm
        if is_scalar(self.__veri):
            return abs(self.__veri)
        else:
            sum = 0
            vector = flatten_tensor(self.__veri)
            for entry in vector:
                sum += abs(entry)
            return sum

    def L2(self):
        # Calculates and returns the L2 norm
        if is_scalar(self.__veri):
            return abs(self.__veri)
        else:
            sum = 0
            vector = flatten_tensor(self.__veri)
            for entry in vector:
                sum += abs(entry)**2
            return math.sqrt(sum)

    def Lp(self, p):
        # Calculates and returns the Lp norm, where p should be positive integer
        if p <= 0:
            raise ValueError("p should be positive!")
        if is_scalar(self.__veri):
            return abs(self.__veri)
        else:
            sum = 0
            vector = flatten_tensor(self.__veri)
            for entry in vector:
                sum += abs(entry)**p
            return sum**(1/p)

    def listeye(self):
        # Converts the gergen object into a list or a nested list, depending on its dimensions.
        return self.__veri

    def duzlestir(self):
        # Converts the gergen object's multi-dimensional structure into a 1D structure, effectively 'flattening' the object.
        if is_scalar(self.__veri):
            return gergen(self.__veri)
        else:
            return gergen(flatten_tensor(self.__veri))

    def boyutlandir(self, yeni_boyut):
        # Reshapes the gergen object to a new shape 'yeni_boyut', which is specified as a tuple.
        if not isinstance(yeni_boyut, tuple):
            raise TypeError("yeni_boyut must be of a tuple !")
        else:
            return gergen(reshape_tensor(self.__veri, yeni_boyut)) # Error handling is done in the reshape_tensor function !

    def ic_carpim(self, other):
        # Calculates the inner (dot) product of this gergen object with another.
        def my_sum(arr):
            sum = 0
            for element in arr:
                sum += element
            return sum


        if not isinstance(other, gergen):
            raise TypeError("For ic_carpim operation, both operand must be of type gergen!")

        other_shape = calculate_shape(other.listeye())

        if len(self.__boyut) > 2 or len(other_shape) > 2:
            return ValueError("ic_carpim is defined only for 1-D and 2-D gergens!")

        if is_scalar(self.__veri) and is_scalar(other.listeye()): # They are scalar gergens.
            return gergen(self.__veri * other.listeye())

        elif len(self.__boyut) == 1 and len(other_shape) == 1: # Both are horizontal vectors.
            if self.__boyut[0] != other_shape[0]:
                raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match!")
            else:
                return my_sum([x * y for x, y in zip(self.__veri, other.listeye())])

        elif len(self.__boyut) == 2 and len(other_shape) == 2 and self.__boyut[1] == 1 and self.__boyut[1] == 1: # Both are vertical vectors.
            raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match for ic_carpim!")

        elif len(self.__boyut) == 1 and len(other_shape) == 2 and other_shape[1] == 1: # self is a horizontal vector and other is a vertical vector
            if self.__boyut[0] != other_shape[0]:
                raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match!")
            else:
                return [my_sum([x * y[0] for x, y in zip(self.__veri, other.listeye())])]

        elif len(self.__boyut) == 2 and len(other_shape) == 1 and self.__boyut[1] == 1: # self is a vertical vector and other is a horizontal vector
            raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match for ic_carpim!")

        elif len(self.__boyut) == 2 and len(other_shape) == 2 and ((self.__boyut[0] >= 2 and self.__boyut[1] >= 2) or (other_shape[0] >= 2 and other_shape[1] >= 2)) : # They are matrices
            if self.__boyut[1] != other_shape[0]:
                raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match! Matrix multiplication cannot be performed!")
            else:
                matrix_multiplication = []
                for i in range(self.__boyut[0]):
                    row = self.__veri[i]
                    temp = []
                    for j in range(other_shape[1]):
                        column = [row[j] for row in other]
                        sum = 0
                        for k in range(self.__boyut[1]):
                            sum += (row[k] * column[k].listeye())
                        temp.append(sum)
                    matrix_multiplication.append(temp)
                return gergen(matrix_multiplication)

        else:
            raise ValueError(f"Dimension mismatch occured: {self.__boyut} and {other_shape} do not match!")

    def dis_carpim(self, other):
        #Calculates the outer product of this gergen object with another.
        if not isinstance(other, gergen):
            raise TypeError("For ic_carpim operation, both operand must be of type gergen!")

        other_shape = calculate_shape(other.listeye())

        vector1 = None
        vector2 = None

        if len(self.__boyut) == 1 and len(other_shape) == 1: # Both are horizontal vectors.
            vector1 = self.__veri
            vector2 = other.listeye()

        elif len(self.__boyut) == 2 and len(other_shape) == 2 and self.__boyut[1] == 1 and self.__boyut[1] == 1: # Both are vertical vectors.
            vector1 = [x[0] for x in self.__veri]
            vector2 = [x[0] for x in other.listeye()]

        elif len(self.__boyut) == 1 and len(other_shape) == 2 and other_shape[1] == 1: # self is a horizontal vector and other is a vertical vector
            vector1 = self.__veri
            vector2 = [x[0] for x in other.listeye()]

        elif len(self.__boyut) == 2 and len(other_shape) == 1 and self.__boyut[1] == 1: # self is a vertical vector and other is a horizontal vector
            vector1 = [x[0] for x in self.__veri]
            vector2 = other.listeye()

        else:
            raise ValueError("Both operands must be a vector to perform dis_carpim!")

        result = []
        for x_i in vector1:
            row_result = []
            for y_i in vector2:
                row_result.append(x_i * y_i)
            result.append(row_result)

        return gergen(result)

    def topla(self, eksen=None):
        #Sums up the elements of the gergen object, optionally along a specified axis 'eksen'.
        if not isinstance(eksen, int) and eksen != None:
            return TypeError("eksen must be an integer or None!")

        if eksen == None: # Sum all elements
            vector = flatten_tensor(self.__veri)
            return sum(vector)

        else:
            if eksen >= len(calculate_shape(self.__veri)):
                raise ValueError("eksen exceeds the number of dimensions!")
            else:
                return gergen(sum_along_axis(self.__veri, eksen))

    def ortalama(self, eksen=None):
        #Calculates the average of the elements of the gergen object, optionally along a specified axis 'eksen'.
        if not isinstance(eksen, int) and eksen != None:
            return TypeError("eksen must be an integer or None!")

        if eksen == None: # Sum all elements
            vector = flatten_tensor(self.__veri)
            return sum(vector) / len(vector)

        else:
            if eksen >= len(calculate_shape(self.__veri)):
                raise ValueError("eksen exceeds the number of dimensions!")
            else:
                return gergen(average_along_axis(self.__veri, eksen))

# Fundamental Operations:
# Random number generation:

import random

def cekirdek(sayi: int):
    random.seed(sayi)

def rastgele_dogal(boyut, aralik=(0,100), dagilim='uniform'):
    if dagilim == "uniform":
        if len(boyut) == 1 and boyut[0] == 1: # Scalar case
            return gergen(uniform_generator_int(aralik[0], aralik[1]))
        elif len(boyut) == 1 and boyut[0] != 1: # Error: The boyut parameter is one-dimensional but has value different than (1,)
            raise ValueError("The parameter must have value have (1,) if it has one dimension!")
        else:
            return gergen(create_tensor(boyut, uniform_generator_int, aralik[0], aralik[1]))
    else:
        raise ValueError("Possible values for the parameter dagilim is uniform!!")

def rastgele_gercek(boyut, aralik=(0.0, 1.0), dagilim='uniform'):
    if dagilim == "uniform":
        if len(boyut) == 1 and boyut[0] == 1: # Scalar case
            return gergen(uniform_generator_float(aralik[0], aralik[1]))
        elif len(boyut) == 1 and boyut[0] != 1: # Error: The boyut parameter is one-dimensional but has value different than (1,)
            raise ValueError("The parameter must have value have (1,) if it has one dimension!")
        else:
            return gergen(create_tensor(boyut, uniform_generator_float, aralik[0], aralik[1]))
    else:
        raise ValueError("Possible values for the parameter dagilim is uniform!")

# 2 Compare with NumPy

import numpy as np              # NumPy, for working with arrays/tensors
import time                     # For measuring time

def example_1():
    #Example 1
    boyut = (64,64)
    g1 = rastgele_gercek(boyut)
    g2 = rastgele_gercek(boyut)

    start = time.time()
    #TODO
    #Apply given equation
    my_result = g1.ic_carpim(g2)
    end = time.time()

    start_np = time.time()
    #TODO
    #Apply the same equation for NumPy equivalent
    numpy_result = np.inner(np.array(g1.listeye()), np.array(g2.listeye()))
    end_np = time.time()

    #TODO:
    #Compare if the two results are the same
    flag = True
    for i in range(64):
        for j in range(64):
            if abs(my_result[i][j].listeye() - numpy_result[i][j]) > 0.000000001: # Here, 0.000000001 is an epsilon for compensating precision loss.
                flag = False

    if flag == False:
        print("Results are different!")
    else:
        print("Results are same!")


    #Report the time difference
    #TODO
    print("Time taken for gergen:", end-start)
    print("Time taken for numpy:", end_np-start_np)

def example_2():
    #Example 2
    #TODO:
    boyut = (4,16,16,16)
    a = rastgele_dogal(boyut)
    b = rastgele_dogal(boyut)
    c = rastgele_dogal(boyut)

    start = time.time()
    my_result = (a*b + a*c + b*c).ortalama()
    end = time.time()

    start_np = time.time()
    numpy_result = np.average(np.add(np.add(np.multiply(np.array(a.listeye()), np.array(b.listeye())), np.multiply(np.array(a.listeye()), np.array(c.listeye()))), np.multiply(np.array(b.listeye()), np.array(c.listeye()))))
    end_np = time.time()

    #TODO:
    #Compare if the two results are the same
    if abs(my_result -  numpy_result) > 0.000000001:
        print("Results are different!")
    else:
        print("Results are same!")

    #Report the time difference
    #TODO
    print("Time taken for gergen:", end-start)
    print("Time taken for numpy:", end_np-start_np)

def example_3():
    #Example 3
    #TODO:
    boyut = (3,64,64)
    a = rastgele_dogal(boyut)
    b = rastgele_dogal(boyut)

    start = time.time()
    my_result = (a.sin() + b.cos()).us(2).ln()
    end = time.time()

    start_np = time.time()
    numpy_result = np.log(np.square(np.add(np.sin(np.array(a.listeye())), np.cos(np.array(b.listeye())))))
    end_np = time.time()

    flag = True
    for i in range(3):
        for j in range(64):
                for k in range(64):
                    if abs(my_result[i][j][k].listeye() - numpy_result[i][j][k]) > 0.000000001:
                        flag = False

    if flag == False:
        print("Results are different!")
    else:
        print("Results are same!")


    #Report the time difference
    #TODO
    print("Time taken for gergen:", end-start)
    print("Time taken for numpy:", end_np-start_np)
