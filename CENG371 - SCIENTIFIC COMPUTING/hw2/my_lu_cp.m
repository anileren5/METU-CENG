function [L,U,P,Q] = my_lu_cp(A)
    [m, n] = size(A); 
    P = eye(m);
    Q = eye(m);

    for i = 1:m-1
        [~, ind] = max(abs(A(i:m, i:m)), [], 'all');
        x = mod(ind - 1, m - i + 1) + i;
        y = floor((ind - 1) / (m - i + 1)) + i;
        A([i, x], :) = A([x, i], :);
        A(:, [i, y]) = A(:, [y, i]);
        P([i, x], :) = P([x, i], :);
        Q(:, [i, y]) = Q(:, [y, i]);
        A(i+1:m, i) = A(i+1:m, i) / A(i, i);
        A(i+1:m, i+1:m) = A(i+1:m, i+1:m) - A(i+1:m, i) * A(i, i+1:m);
    end

    L = eye(m);
    U = zeros(m);
    for i = 1:m
        L(i+1:m, i) = A(i+1:m, i);
        U(i, i:m) = A(i, i:m);
    end
end
