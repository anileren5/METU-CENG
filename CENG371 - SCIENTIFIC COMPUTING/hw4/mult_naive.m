function C = mult_naive(A, B)
    [m,n1] = size(A);
    [n2,p] = size(B);
    
    % Check if the matrices can be multiplied
    if n1 ~= n2
        error('Matrix dimensions are not compatible for multiplication. Inner dimensions must match.');
    end

    n = n1;

    AB = zeros(m,p);

    for i = 1:m
        for j = 1:p
            AB(i,j) = 0;
            for k = 1:n
                AB(i,j) = AB(i,j) + (A(i,k) * B(k,j));
            end
        end
    end

    C = AB;
end
