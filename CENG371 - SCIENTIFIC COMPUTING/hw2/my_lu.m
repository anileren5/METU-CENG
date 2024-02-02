function [L, U] = my_lu(A) 
    [m, n] = size(A); % for square matrices m = n (m: rows, n: columns)
    L = eye(m);
    U = A;

    for i = 1:n % i trace columns
        for j = i+1:m % j trace rows
            L(j, i) = U(j, i) / U(i, i);
            U(j, :) = U(j, :) - L(j, i) * U(i, :);
        end
    end
end



