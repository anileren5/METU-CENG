function D = mult_proj_Gauss(A, B, c, f)
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

    % Create the random projection matrix P with elements from N(0, 1)
    P = randn(n, c);
    
    % Normalize each column of P to have a norm of 1
    for i = 1:c
        P(:, i) = P(:, i) / norm(P(:, i), 2);
    end
    
    % Project matrices A and B onto the lower-dimensional space
    C = f(A,P);
    R = f(P',B);
    
    % Multiply the projected matrices using the provided function handle
    D = f(C,R);

end

