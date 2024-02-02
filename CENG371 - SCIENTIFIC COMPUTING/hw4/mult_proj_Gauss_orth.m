function D = mult_proj_Gauss_orth(A, B, c, f)
    [m, n1] = size(A);
    [n2, p] = size(B);
    
    % Check if the matrices can be multiplied
    if n1 ~= n2
        error('Matrix dimensions are not compatible for multiplication. Inner dimensions must match.');
    end

    % Check if c is positive and less than or equal to n
    if c <= 0 || c > n1
        error('c must be a positive integer less than or equal to the number of columns in A.');
    end

    % Create the random matrix P with elements from N(0, 1)
    P = randn(n1, c);

    % Apply the Gram-Schmidt process to orthogonalize the columns of P
    [Q, RR] = qr(P);
    P = Q;

    % Project matrices A and B onto the lower-dimensional space
    C = f(A, P);
    R = f(P', B);

    % Multiply the projected matrices using the provided function handle
    D = f(C, R);
end