WP_ERRORS = [];
PP_ERRORS = [];
CP_ERRORS = [];
WP_RUNTIMES = [];
PP_RUNTIMES = [];
CP_RUNTIMES = [];

for i = 2:300
    A = hilb(i);
    
    tic;
    [L1,U1] = my_lu(A);
    WP_RUNTIMES(end+1) = toc;
    WP_ERRORS(end+1) = norm(A - L1*U1, 2) / norm(A, 2);

    tic;
    [L2,U2,P2] = my_lu_pp(A);
    PP_RUNTIMES(end+1) = toc;
    PP_ERRORS(end+1) = norm(P2*A - L2*U2, 2) / norm(P2*A, 2);

    tic;
    [L3,U3,P3,Q3] = my_lu_cp(A);
    CP_RUNTIMES(end+1) = toc;
    CP_ERRORS(end+1) = norm(P3*A*Q3 - L3*U3, 2) / norm(P3*A*Q3, 2);
end

figure;
plot(2:300, WP_RUNTIMES, 'r', 'LineWidth', 2, 'DisplayName', 'LU without pivoting');
hold on;
plot(2:300, PP_RUNTIMES, 'g', 'LineWidth', 2, 'DisplayName', 'LU with partial pivoting');
plot(2:300, CP_RUNTIMES, 'b', 'LineWidth', 2, 'DisplayName', 'LU with complete pivoting');
hold off;

title('LU Decomposition Runtimes');
xlabel('Matrix Size');
ylabel('Runtime (seconds)');
legend('show');
grid on;
set(gcf, 'PaperPosition', [0 0 8 6]); 
saveas(gcf, 'LU_Runtimes.pdf');

figure;
semilogy(2:300, WP_ERRORS, 'r', 'LineWidth', 2, 'DisplayName', 'LU without pivoting');
hold on;
semilogy(2:300, PP_ERRORS, 'g', 'LineWidth', 2, 'DisplayName', 'LU with partial pivoting');
semilogy(2:300, CP_ERRORS, 'b', 'LineWidth', 2, 'DisplayName', 'LU with complete pivoting');
hold off;

title('LU Decomposition Relative Errors');
xlabel('Matrix Size');
ylabel('Relative Error');
legend('show');
grid on;
set(gcf, 'PaperPosition', [0 0 8 6]); % [left bottom width height] in inches
saveas(gcf, 'LU_RelativeErrors.pdf');

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



