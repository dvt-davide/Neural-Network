#include<random>
#include<iostream>
#include<cassert>
#include<cmath>

#define ask(test, x1, x2) {double _##test##[] = { x1, x2 };\
					feedForward(&nn, _##test##); \
					std::cout<<"Test " << x1 << "," << x2 << " = "  << getOutput(&nn) << std::endl; }
                   
#define IS_INPUT(x) (x == input1 || x == input2  || x == BIAS) 
#define NOT_INPUT(x) (!IS_INPUT(x))
#define IS_OUTPUT(x) (x == output1)
#define NOT_OUTPUT(x) ( !IS_OUTPUT(x))
#define ONE_ENDING(x, y) (IS_INPUT(x) || IS_OUTPUT(y))
#define BOTH_ENDING(x, y) (IS_INPUT(x) && IS_OUTPUT(y))
#define FALSE 0
#define TRUE 1

#define TRAINING_DATA 		double training_data[][3] = {\
							{ TRUE, FALSE, TRUE},\
							{ FALSE, TRUE, TRUE },\
							{ TRUE, TRUE, FALSE },\
							{ FALSE, FALSE, FALSE },\
						};


#define LEARNING_RATE  0.0001f

const auto GENERAZIONI = 1481;
const auto SIZE = 6;
const auto DATASET_SIZE = 3;
const auto input1 = 0;
const auto input2 = 1;
const auto BIAS = 2;
const auto BIAS_VALUE = 0.0001f;
const auto output1 = SIZE-1;


typedef struct {
	double connections[SIZE][SIZE];
	double signals[SIZE];
} Network;

const char *etichette[] = {
	"input1",
	"input2",
	"BIAS",
	"h1",
	"h2",	
	"output1",
	"h3",
	"h4",
};

void init(Network* nn);
void feedForward(Network* nn, double inputs[]);
double getOutput(Network* nn);
void printConnections(Network * nn);
bool exercise(Network* baby, double training_data[][3]);
void correct(Network* baby, double error);
double get_blame(Network* baby, int from, int to);
double act(double signal);
//void rec_adjustment(Network* baby, int to, double error, bool* not_working);

int main() {
	
	Network nn;
	init(&nn);
	unsigned long long int perc = 0;
	unsigned long long int old_perc = -1;
	unsigned long long int x = 0;
	//for (unsigned long long int x = 0; x < GENERAZIONI; x++)
	TRAINING_DATA
	do{
		// 100 : ? = 999999 : x
		//perc = x * 100 / GENERAZIONI;
		//if (perc != old_perc) {
		//	std::cout << "Completamento: " << perc << "%" << std::endl;
		//	old_perc = perc;
		//}
		x++;
	} while (exercise(&nn, training_data));
	std::cout << "Iterazioni totali necessarie: " << x << std::endl;
	ask(_10, TRUE, FALSE)
	ask(_01, FALSE, TRUE)
	ask(_00, FALSE, FALSE)
	ask(_11, TRUE, TRUE)

	printConnections(&nn);
	return 0;
}

bool exercise(Network* baby, double training_data[][3])
{
	const auto TSET_SIZE = DATASET_SIZE;
	const auto I1 = 0;
	const auto I2 = 1;
	const auto DO = 2;
	bool exit = false;
	for (int test = 0; test < TSET_SIZE; test++)
	{
		double error = 0.0f;
		//do {
		double test_case[] = { training_data[test][I1], training_data[test][I2] };
		feedForward(baby, test_case);
		error = training_data[test][DO] - getOutput(baby);
		if (error != 0.0f) {
			correct(baby, error);
			exit = true;
		}
		//} while (ERROR_LIMIT);
	}
	return exit;
}

void correct(Network *baby, double error)
{
	// Aw = E * signal[from] * LR
	
	for (int from = 0; from < SIZE; from++)
	{
		if (NOT_OUTPUT(from))
		{
			for (int to = 0; to < SIZE; to++)
				if (ONE_ENDING(from, to) && NOT_INPUT(to) && !(IS_INPUT(from) && IS_OUTPUT(to)))
				{
					double blame = get_blame(baby, from, to);
					// 100: blame = error : x
					double blame2 = blame / error;
					double adj = blame2 * act(baby->signals[from]);

					if (adj == adj && adj != 0)
						baby->connections[from][to] += adj * LEARNING_RATE;
					else if(adj == 0 && baby->connections[from][to] == 0 && act(baby->signals[from]) != 0)
						baby->connections[from][to] = LEARNING_RATE;
					//if (baby->connections[from][to] == INFINITY)
					//	baby->connections[from][to] = 1.0f;
					//else if (act(baby->signals[from]) != 0.0f)
					//	baby->connections[from][to] += LEARNING_RATE;
				}
		}
	}
}

double get_blame(Network * baby, int from, int to)
{
	double blame = 100 * (act(baby->signals[from]) * baby->connections[from][to]);
	if (blame == 0.0f)
		return 0.0f;
	else if (baby->signals[to] == baby->signals[to] && baby->signals[to] != 0.0f)
		if ((blame / act(baby->signals[to])) == INFINITY)
			return 100;
		else if ((blame / act(baby->signals[to])) == -INFINITY)
			return -100;
		else
			return blame / act(baby->signals[to]);

	return 0.0f;
}

void init(Network* nn) {
	std::srand(19121998);

	for (int to = 0; to < SIZE; to++)
	{
		nn->signals[to] = 0.0f;

		for (int from = 0; from < SIZE; from++)
				nn->connections[from][to] = 0.0f;
		if(NOT_INPUT(to) && NOT_OUTPUT(to))
			nn->connections[BIAS][to] = (double(std::rand() % 201) / 100) - 1.0f;
	}
}

void feedForward(Network* nn, double inputs[])
{
	for (int x = 0; x < SIZE; x++)
		nn->signals[x] = 0.0f;

	nn->signals[input1] = inputs[0];
	nn->signals[input2] = inputs[1];
	nn->signals[BIAS] = BIAS_VALUE;

	for (int to = 0; to < SIZE; to++)
	{
		if (NOT_INPUT(to))
		{
			for (int from = 0; from < SIZE; from++)
				if(NOT_OUTPUT(from) && ONE_ENDING(from, to) && !BOTH_ENDING(from, to))
					nn->signals[to] += act(nn->signals[from]) * nn->connections[from][to];
		}


	}
}

double act(double signal)
{
	//return tanh(signal);
	return signal;
}

double act_out(double signal)
{
	//return tanh(signal);
	double mid = double(TRUE - FALSE) / 2;
	mid = TRUE - mid;

	double lmid = mid - double(mid - FALSE)/2;
	double hmid = TRUE - double(TRUE - mid)/2;
	//std::cout << "Activation signal " << signal << std::endl;
	//std::cout << "mid " << mid << std::endl;
	//std::cout << "low mid " << lmid << std::endl;
	//std::cout << "high mid " << hmid << std::endl;
	return (signal  > lmid && signal < hmid )? TRUE: FALSE;
	//  low mid high
	//    lm   lh
}

double getOutput(Network* nn)
{
	return act_out(nn->signals[output1]);
}

void printConnections(Network *nn) {
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
			std::cout << "from " << etichette[x] 
			<< " to " << etichette[y] << " : " <<
			nn->connections[x][y] <<std::endl;
		
	}
}