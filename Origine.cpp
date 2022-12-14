#include<random>
#include<iostream>
#include<cassert>
#include<cmath>

#define MAGIC_NUMBER 19121998 // for random purpouse
#define ask(test, x1, x2) {double _##test##[] = { x1, x2 };\
					feedForward(&nn, _##test##); \
					std::cout<<"Test " << x1 << "," << x2 << " = "  << getOutput(&nn) << std::endl; }
                   
#define IS_INPUT(x) (x == input1 || x == input2  || x == BIAS) 
#define NOT_INPUT(x) (!IS_INPUT(x))
#define IS_OUTPUT(x) (x == output1)
#define NOT_OUTPUT(x) ( !IS_OUTPUT(x))
#define ONE_ENDING(x, y) (IS_INPUT(x) || IS_OUTPUT(y))
#define BOTH_ENDING(x, y) (IS_INPUT(x) && IS_OUTPUT(y))

// you can map wathever values you want
#define FALSE 0
#define TRUE 1

// XOR training dataset
#define TRAINING_DATA 		double training_data[][3] = {\
							{ TRUE, FALSE, TRUE},\
							{ FALSE, TRUE, TRUE },\
							{ TRUE, TRUE, FALSE },\
							{ FALSE, FALSE, FALSE },\
						};
const unsigned DATASET_SIZE = 4; 

// training parameter
const double LEARNING_RATE = 0.0001f;
const unsigned ITERATIONS = 1481;

// Network info
const unsigned SIZE = 6;			// number of nodes in network (with input and output nodes)
const unsigned input1 = 0;			// identify node of first input value
const unsigned input2 = 1;			// identify node of second input value
const unsigned BIAS = 2;			// identify bias node
const unsigned output1 = SIZE - 1;  // identify output node
const double BIAS_VALUE = 0.0001f;	// constant bias output


typedef struct {
	double connections[SIZE][SIZE]; // contains all possible connection of the nodes
	double signals[SIZE];           // signals emitted by each node
} Network;

const char *labels[] = {  // for output purpouses
	"input1",
	"input2",
	"BIAS",
	"h1",
	"h2",	
	"output1",
};

// set all signals to 0  and all connections to a random value (i do not consider)
// loop conections and connections in between nodes of the same layer
void init(Network* nn); 
// feed input to netwrk and generates output signal
void feedForward(Network* nn, double inputs[]);
// generic activation function for input and hidden nodes
double act(double signal);
// acitivation function for output nodes
double act_out(double signal);
double getOutput(Network* nn);
// print all connections of the network eg."from x to y: 0.00"
void printConnections(Network * nn);
// training function
bool exercise(Network* baby, double training_data[][3]);
// correct the wheights in the network(adjust connection value )
void correct(Network* baby, double error);
// get a percentage of how much a specific connection has contributed to get the error
double get_blame(Network* baby, int from, int to);


int main() {
	
	Network nn;
	
	init(&nn);
	
	unsigned long long int x = 0;
	
	// get the training data
	TRAINING_DATA
	
	do{
		x++;
	} while (exercise(&nn, training_data) && x < ITERATIONS);
	std::cout << "Total Iterations: " << x << std::endl;
	
	ask(_10, TRUE, FALSE)
	ask(_01, FALSE, TRUE)
	ask(_00, FALSE, FALSE)
	ask(_11, TRUE, TRUE)

	printConnections(&nn);
	return 0;
}

bool exercise(Network* baby, double training_data[][3])
{
	const auto I1 = 0; // input1 in training_data
	const auto I2 = 1; // input2 in training_data
	const auto DO = 2; // desired output in training_data

	bool exit = false;

	for (int test = 0; test < DATASET_SIZE; test++)
	{
		double error = 0.0f;
		double test_case[] = { training_data[test][I1], training_data[test][I2] };

		feedForward(baby, test_case);

		error = training_data[test][DO] - getOutput(baby);
		// if we get an error for the current test case 
		// we need to adjust the connections
		if (error != 0.0f) {
			correct(baby, error);
			exit = true;
		}
	}
	return exit;
}

void correct(Network *baby, double error)
{
	// correction formula for each weight (connections) 
	// dw = Error * signal[from] * LEARNING_RATE	
	for (int from = 0; from < SIZE; from++)
	{
		if (NOT_OUTPUT(from)) // filter all nodes that are not the output node
		{
			for (int to = 0; to < SIZE; to++)
				if (ONE_ENDING(from, to) && NOT_INPUT(to) && !(IS_INPUT(from) && IS_OUTPUT(to)))
				{
					double blame = get_blame(baby, from, to);
					blame = blame / error;
					double adj = blame * act(baby->signals[from]);

					if (adj == adj && adj != 0) // adj==adj because it might be Nan
						baby->connections[from][to] += adj * LEARNING_RATE;
					else if(adj == 0 && baby->connections[from][to] == 0 && act(baby->signals[from]) != 0)// condition to create a new connection
						baby->connections[from][to] = LEARNING_RATE;
				}
		}
	}
}

double get_blame(Network * baby, int from, int to)
{
	// like the feed_forward formula but for a single connection
	double blame = 100 * (act(baby->signals[from]) * baby->connections[from][to]); 
	if (blame == 0.0f)
		return blame;
	// check if the signal is not Nan and != 0
	else if (baby->signals[to] == baby->signals[to] && baby->signals[to] != 0.0f)
		if ((blame / act(baby->signals[to])) == INFINITY) // it has all the blame so 100%...
			return 100;
		else if ((blame / act(baby->signals[to])) == -INFINITY) // negative 100%
			return -100;
		else
			return blame / act(baby->signals[to]); // calculate the percentage of blame

	return 0.0f;
}

void init(Network* nn) {
	std::srand(MAGIC_NUMBER);
	// Initialize all signals and connections
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
	// reset signals in case of multiple executions
	// like in the training session
	for (int x = 0; x < SIZE; x++)
		nn->signals[x] = 0.0f; 

	// map inputs in the network
	nn->signals[input1] = inputs[0];
	nn->signals[input2] = inputs[1];
	nn->signals[BIAS] = BIAS_VALUE;

	// node activation formula:
	// signals from previous node -> sig[from]
	// wheight between previous and current node -> conn[from][to]
	// sig[current] = sum sig[from] * conn[from][to]
	// for all connections that sends signals to the current node
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
	// TBD: i do not have an actual activation function
	// i just return the raw value calculated in the feed_forward function
	return signal;
}

double act_out(double signal)
{
	// TBD: i noticed that to solve XOR problem
	// the network will get similar values near 0 for the true case
	// eg. 0 xor 1 = 0.08
	// eg. 1 xor 0 = 0.08
	// big values but different sign for the false case
	// eg. 1 xor 1 = + 40.xxx
	// eg. 0 xor 0 = - 40.xxx
	double mid = double(TRUE - FALSE) / 2;
	mid = TRUE - mid;

	double lmid = mid - double(mid - FALSE)/2;
	double hmid = TRUE - double(TRUE - mid)/2;

	return (signal  > lmid && signal < hmid )? TRUE: FALSE;
	//  low   mid    high
	//    lmid   hmid
}

double getOutput(Network* nn)
{
	return act_out(nn->signals[output1]);
}

void printConnections(Network *nn) {
	// print all connections of the network
	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
			std::cout << "from " << labels[x] 
			<< " to " << labels[y] << " : " <<
			nn->connections[x][y] <<std::endl;
		
	}
}
