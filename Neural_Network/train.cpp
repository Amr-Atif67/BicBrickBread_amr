#include "Include/NeuralNetwork.h"
#include "Large_Tic_Tac_Toe.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>

using namespace std;

// ------------------ Activation functions ------------------
auto sigmoid = [](double x){ return 1.0 / (1.0 + exp(-x)); };
auto sigmoidDerivative = [](double x){ double s = 1.0 / (1.0 + exp(-x)); return s*(1-s); };

// ------------------ Encode Board ------------------
vector<double> encodeBoard(Large_XO_Board* board, char ai) {
    vector<double> input(25);
    for(int r=0;r<5;r++)
        for(int c=0;c<5;c++){
            char cell = board->get_cell(r,c);
            input[r*5+c] = (cell==ai ? 1.0 : (cell=='.' ? 0.0 : -1.0));
        }
    return input;
}

// ------------------ Pick move from NN safely ------------------
pair<int,int> pickMoveFromNN(NeuralNetwork& nn, Large_XO_Board* board, char ai, mt19937& rng){
    vector<double> inputVec = encodeBoard(board, ai);
    Matrix input(inputVec, 25, 1);
    Matrix output = nn.predict(input);

    double bestScore = -1e9;
    pair<int,int> move = {-1,-1};
    for(int r=0;r<5;r++)
        for(int c=0;c<5;c++){
            int index = r*5+c;
            if(board->get_cell(r,c)=='.' && output(index,0) > bestScore){
                bestScore = output(index,0);
                move = {r,c};
            }
        }

    // Fallback to random empty cell if NN fails
    if(move.first==-1){
        vector<pair<int,int>> emptyCells;
        for(int r=0;r<5;r++)
            for(int c=0;c<5;c++)
                if(board->get_cell(r,c)=='.')
                    emptyCells.push_back({r,c});
        uniform_int_distribution<int> dist(0, emptyCells.size()-1);
        move = emptyCells[dist(rng)];
    }

    return move;
}

// ------------------ Play full game ------------------
char playFullGame(NeuralNetwork NN1, NeuralNetwork NN2){
    Large_XO_Board board;
    char currentPlayer='X';
    mt19937 rng(random_device{}());

    while(!board.game_is_over(nullptr)){
        pair<int,int> move;
        if(currentPlayer=='X') move = pickMoveFromNN(NN1, &board, 'X', rng);
        else move = pickMoveFromNN(NN2, &board, 'O', rng);

        Move<char> m(move.first, move.second, currentPlayer);
        board.update_board(&m);
        currentPlayer = (currentPlayer=='X'?'O':'X');
    }

    int xScore = board.countWin('X');
    int oScore = board.countWin('O');
    if(xScore>oScore) return 'X';
    else if(oScore>xScore) return 'O';
    else return 'D';
}

// ------------------ Mutate NN safely ------------------
void mutateNetwork(NeuralNetwork& nn, double mutationRate, mt19937& rng){
    uniform_real_distribution<double> dist(-1.0,1.0);
    for(auto& layer : nn.layers){
        for(int i=0;i<layer.W.rows;i++)
            for(int j=0;j<layer.W.cols;j++)
                layer.W(i,j) += dist(rng) * mutationRate;
        for(int i=0;i<layer.B.rows;i++)
            for(int j=0;j<layer.B.cols;j++)
                layer.B(i,j) += dist(rng) * mutationRate;
    }
}

// ------------------ Evaluate network safely ------------------
double evaluateNetworkSafe(NeuralNetwork nn, NeuralNetwork opponent, int games, mt19937& rng){
    uniform_int_distribution<int> coin(0,1);
    double score = 0;
    for(int i=0;i<games;i++){
        char winner;
        if(coin(rng)==0) winner = playFullGame(nn, opponent);
        else winner = playFullGame(opponent, nn);

        if(winner=='X') score+=1.0;
        else if(winner=='D') score+=0.5;
    }
    return score;
}

// ------------------ All-vs-All Evolutionary Training ------------------
void evolutionaryTrainingAllVsAll(int generations, int populationSize=20, int gamesPerGen=5, int eliteCount=5) {
    vector<int> layerSizes={25,32,25};
    vector<NeuralNetwork> population;
    for(int i=0;i<populationSize;i++)
        population.emplace_back(layerSizes, sigmoid, sigmoidDerivative);

    int numThreads = thread::hardware_concurrency();
    if(numThreads==0) numThreads=4;
    cout << "Using " << numThreads << " threads for evaluation\n";

    for(int g=0; g<generations; g++) {
        vector<pair<int,double>> fitnessScores(populationSize);
        mutex fitnessMutex;

        vector<thread> threads;
        int perThread = (populationSize + numThreads - 1) / numThreads;

        for(int t=0; t<numThreads; t++) {
            threads.emplace_back([&,t]() {
                mt19937 rng(random_device{}() + t*1000); 
                int start = t*perThread;
                int end = min(start + perThread, populationSize);

                for(int i=start; i<end; i++) {
                    double score = 0.0;
                    // All-vs-all
                    for(int j=0; j<populationSize; j++){
                        if(i==j) continue;
                        score += evaluateNetworkSafe(population[i], population[j], gamesPerGen, rng);
                    }

                    lock_guard<mutex> lock(fitnessMutex);
                    fitnessScores[i] = {i, score};
                }
            });
        }

        for(auto& th: threads) th.join();

        // Sort descending
        sort(fitnessScores.begin(), fitnessScores.end(), [](auto &a, auto &b){ return a.second > b.second; });
        cout << "Generation " << g << " best score: " << fitnessScores[0].second << endl;

        // Elite selection
        vector<NeuralNetwork> newPopulation;
        for(int i=0;i<eliteCount;i++)
            newPopulation.push_back(population[fitnessScores[i].first]);

        // Adaptive mutation
        double baseMutation = 0.2;
        double finalMutation = 0.01;
        double mutationRate = baseMutation - (baseMutation-finalMutation)*(g/double(generations));

        mt19937 rng_main(random_device{}());
        while(newPopulation.size() < populationSize) {
            int parentIndex = rng_main()%eliteCount;
            NeuralNetwork child = newPopulation[parentIndex];
            mutateNetwork(child, mutationRate, rng_main);
            newPopulation.push_back(child);
        }

        population = newPopulation;

        if(g % 100 == 0)
            population[0].save("bestNN.dat");
    }

    population[0].save("bestNN.dat");
    cout << "Training complete. Best network saved to bestNN.dat\n";
}

// ------------------ Evolutionary Training ------------------
void evolutionaryTraining(int generations, int populationSize=50, int gamesPerGen=20, int eliteCount=10){
    srand(time(0));
    vector<int> layerSizes={25,32,25};
    vector<NeuralNetwork> population;
    for(int i=0;i<populationSize;i++)
        population.emplace_back(layerSizes, sigmoid, sigmoidDerivative);

    int numThreads = thread::hardware_concurrency();
    if(numThreads==0) numThreads=4;
    cout << "Using " << numThreads << " threads\n";

    mutex fitnessMutex;

    for(int g=0; g<generations; g++){
        vector<pair<int,double>> fitnessScores(populationSize);

        vector<thread> threads;
        int perThread = (populationSize + numThreads -1)/numThreads;

        for(int t=0;t<numThreads;t++){
            threads.emplace_back([&,t](){
                mt19937 rng(time(0)+t*1000); // thread-local RNG
                int start = t*perThread;
                int end = min(start+perThread, (int)populationSize);
                for(int i=start;i<end;i++){
                    NeuralNetwork nnCopy = population[i];
                    int oppIndex;
                    do {
                        oppIndex = rng() % populationSize;
                    } while (oppIndex == i);
                    NeuralNetwork oppCopy = population[oppIndex];
                    double score = evaluateNetworkSafe(nnCopy, oppCopy, gamesPerGen, rng);
                    lock_guard<mutex> lock(fitnessMutex);
                    fitnessScores[i]={i,score};
                }
            });
        }
        for(auto& th: threads) th.join();

        sort(fitnessScores.begin(), fitnessScores.end(), [](auto&a, auto&b){return a.second>b.second;});

        cout << "Generation " << g << " best score: " << fitnessScores[0].second << endl;

        // Create new population
        vector<NeuralNetwork> newPopulation;
        for(int i=0;i<eliteCount;i++)
            newPopulation.push_back(population[fitnessScores[i].first]);

        // Adaptive mutation rate
        double baseMutation = 0.2;
        double finalMutation = 0.01;
        double mutationRate = baseMutation - (baseMutation-finalMutation)*(g/double(generations));

        mt19937 rng_main(random_device{}());
        while(newPopulation.size()<populationSize){
            int parentIndex = rand()%eliteCount;
            NeuralNetwork child = newPopulation[parentIndex];
            mutateNetwork(child, mutationRate, rng_main);
            newPopulation.push_back(child);
        }

        population = newPopulation;

        // Optional: save every 1000 generations
        if(g%1000==0)
            population[0].save("bestNN.dat");
    }

    // Final save
    population[0].save("bestNN.dat");
    cout << "Training complete. Best network saved to bestNN.dat\n";
}

// ------------------ Main ------------------
int main(){
    int generations = 5000;
    int populationSize = 20;   
    int gamesPerGen = 20;      
    int eliteCount = 5;

    evolutionaryTrainingAllVsAll(generations, populationSize, gamesPerGen, eliteCount);
    return 0;
}