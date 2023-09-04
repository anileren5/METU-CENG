N = 4145; % size of the Monte Carlo simulation with alpha = 0.01 and epsilon = 0.02

% Parameters 
n_lambda_automobile = 50; % lambda of the number of automobiles passing over
n_lambda_truck = 10; % lambda of the number of trucks passing over
w_alpha_automobile = 190; % alpha of the weight of each automobile
w_lambda_automobile = 0.15; % lambda of the weight of each automobile
w_alpha_truck = 110; % alpha of the weight of each truck
w_lambda_truck = 0.01; % lambda of the weight of each truck

% A vector that keeps the total weight of vehicles passing over the bridge for each run of Monte Carlo Simulation
TotalWeight = zeros(N,1);

for k=1:N;
    % generate n_automobile by sampling from the Poisson Distribution
    U = rand;
    i = 0;
    F = exp(-n_lambda_automobile);
    while (U >= F);
        i = i + 1;
        F = F + exp(-n_lambda_automobile) * n_lambda_automobile^i/gamma(i+1);
    end;
    n_automobile = i;

    % generate n_truck by sampling from the Poisson Distribution
    U = rand;
    i = 0;
    F = exp(-n_lambda_truck);
    while (U >= F);
        i = i + 1;
        F = F + exp(-n_lambda_truck) * n_lambda_truck^i/gamma(i+1);
    end;
    n_truck = i;

    % generate w_automobile from the Gamma Distribution
    w_automobile = 0;
    for j=1:n_automobile;
        w_automobile = w_automobile + sum(-1/w_lambda_automobile * log(rand(w_alpha_automobile,1)));
    end;

    % generate w_truck from the Gamma Distribution
    w_truck = 0;
    for j=1:n_truck;
        w_truck = w_truck + sum(-1/w_lambda_truck * log(rand(w_alpha_truck,1)));
    end;

    TotalWeight(k) = w_automobile + w_truck;
end;
p_est = mean(TotalWeight > 200000);
expectedWeight = mean(TotalWeight);
stdWeight = std(TotalWeight);
fprintf('Estimated probability = %f\n',p_est);
fprintf('Expected weight = %f\n',expectedWeight);
fprintf('Standard deviation = %f\n',stdWeight);