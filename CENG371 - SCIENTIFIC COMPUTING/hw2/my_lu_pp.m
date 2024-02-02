function [L, U, P] = my_lu_pp(A)
    [m, n] = size(A); 
    L = eye(m);
    U = A;
    P = eye(m);

    for i = 1:n 
        [~, pivot] = max(abs(U(i:end, i)));
        pivot = pivot + i - 1;

        U([i, pivot], :) = U([pivot, i], :);
        L([i, pivot], 1:i-1) = L([pivot, i], 1:i-1);
        P([i, pivot], :) = P([pivot, i], :);

        for j = i+1:m 
            L(j, i) = U(j, i) / U(i, i);
            U(j, :) = U(j, :) - L(j, i) * U(i, :);
        end
    end
end

