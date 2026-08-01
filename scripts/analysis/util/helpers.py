def matrix_to_latex_str(matrix):
    """
    Convert a numpy matrix to a LaTeX formatted string.
    """
    if matrix.ndim != 2:
        raise ValueError("Input must be a 2D matrix.")
    
    rows, cols = matrix.shape
    latex_str = "$$\n\\begin{bmatrix}\n"
    
    for i in range(rows):
        row_str = " & ".join(f"{matrix[i, j]:.8f}" for j in range(cols))
        latex_str += row_str + " \\\\\n"
    
    latex_str += "\\end{bmatrix}\n$$"
    return latex_str    

def matrix_to_yaml_str(matrix):
    """
    Convert a numpy matrix to a YAML formatted string.
    """
    if matrix.ndim != 2:
        raise ValueError("Input must be a 2D matrix.")
    
    rows, cols = matrix.shape
    yaml_str = "matrix:\n"
    
    for i in range(rows):
        row_str = "  - [" + ", ".join(f"{matrix[i, j]:.8f}" for j in range(cols)) + "]\n"
        yaml_str += row_str
    
    return yaml_str