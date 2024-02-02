% Load the grayscale image
image = im2double(imread("./cat.jpg"));

% If the image is not grayscale then make it grayscale
[rows, columns, numberOfColorChannels] = size(image);
if numberOfColorChannels > 1
  disp("Converting rgb image to grayscale image!")
  image = image(:, :, 2); 
end

% ---------------- Part a) ----------------

% Find r
r = min(size(image));

% Calculate Singular Value Decomposition
[U, S, V] = svd(image);

% Preallocate low rank approximations
lowRankApproximations = cell(1, r);

% Calculate all low rank approximations up to rank r
for k = 1:r
    S_k = S(1:k, 1:k);
    U_k = U(:, 1:k);
    V_k = V(:, 1:k);
    I_k = U_k * S_k * transpose(V_k);

    % Save the current low rank approximation
    lowRankApproximations{k} = I_k;
end

% Save original image and low rank approximations
imwrite(image, 'original.jpg');
imwrite(lowRankApproximations{floor(r/2)}, "r_divided_by_2.jpg")
imwrite(lowRankApproximations{r-10}, "r_minus_10.jpg")

% Save these low rank approximations to see differences better
imwrite(lowRankApproximations{floor(r/4)}, "r_divided_by_4.jpg")
imwrite(lowRankApproximations{floor(r/8)}, "r_divided_by_8.jpg")
imwrite(lowRankApproximations{floor(r/16)}, "r_divided_by_16.jpg")
imwrite(lowRankApproximations{floor(r/32)}, "r_divided_by_32.jpg")
imwrite(lowRankApproximations{floor(r/64)}, "r_divided_by_64.jpg")

% ---------------- Part b) ----------------
sigma = flip(diag(S)); % reverse the ordering
k = size(sigma);
frobeniusErrors = ones(k);
sums_of_squares_of_singular_values = ones(k);

% Calculate the Frobenius Errors and S(k)'s
for k = 1:length(sigma)
    Sk = S;
    Sk(end-k+1:end, :) = 0;    
    Ik = U * Sk * transpose(V);
    frobeniusErrors(k) = norm(image - Ik, 'fro');     
    sums_of_squares_of_singular_values(k) = sqrt(sum(sigma(1:k).^2));     
end

% Save log-log graph
loglog(sums_of_squares_of_singular_values, "r-.", "LineWidth", 3); hold on;
loglog(frobeniusErrors, "g-.", "LineWidth", 2); 
legend("S(k)", "|I - Ik|");
title("S(k) and |I - Ik|");
xlabel("k");
ylabel("Value of S(k) and |I - Ik|");
saveas(gcf, 'log_log_graph.png');






