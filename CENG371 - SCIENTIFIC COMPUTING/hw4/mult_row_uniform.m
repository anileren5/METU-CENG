function D = mult_row_uniform(A, B, c, f)
    [m,n1] = size(A);
    [n2,p] = size(B);
    
    % Check if the matrices can be multiplied
    if n1 ~= n2
        error('Matrix dimensions are not compatible for multiplication. Inner dimensions must match.');
    end

    % Check if c is positive
    if c <= 0
        error('c is not positive.');
    end

    n = n1;
    
    probabilities = ones(1, n) / n; % Uniform probability distribution
    
    % C will have c columns and R will have c rows
    C = zeros(m, c);
    R = zeros(c, p);

    for t = 1:c
        i_t = randsample(1:n, 1, true, probabilities);
        C(:, t) = A(:, i_t) / sqrt(c * probabilities(i_t));
        R(t, :) = B(i_t, :) / sqrt(c * probabilities(i_t));
    end

    D = f(C,R);
end    


    


