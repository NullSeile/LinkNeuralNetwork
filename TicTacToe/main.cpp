#include <LinkNeuralNetwork.h>
#include <User Interface.h>
#include <utility.h>

#define bot 1

#define gSize 3

enum Result
{
	None = -1,
	Draw,
	Win
};

inline int GetIndex(const uint& x, const uint& y)
{
	return y * gSize + x;
}

void DrawGrid(const std::array<int, gSize * gSize>& grid)
{
	for (uint j = 0; j < gSize; j++)
	{
		for (uint i = 0; i < gSize; i++)
		{
			std::cout << grid[GetIndex(i, j)] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void ResetGrid(std::array<int, gSize * gSize>& grid)
{
	for (uint i = 0; i < gSize * gSize; i++)
	{
		grid[i] = 0;
	}
}

std::array<double, gSize * gSize> GetInputs(const std::array<int, gSize * gSize>& grid)
{
	std::array<double, gSize * gSize> inputs;
	for (uint i = 0; i < gSize * gSize; i++)
	{
		switch (grid[i])
		{
		case 0:
			inputs[i] = 0;
			break;

		case 1:
			inputs[i] = -1;
			break;

		case 2:
			inputs[i] = 1;
			break;
		}
	}
	return inputs;
}

std::array<double, gSize * gSize> GetOutput(const uint& play)
{
	std::array<double, gSize * gSize> outputs;
	outputs.fill(0);
	outputs[play] = 1;

	return outputs;
}

int CheckResult(const std::array<int, gSize * gSize>& grid, const int& player)
{
	uint sum;

	// Vertical
	for (uint i = 0; i < gSize; i++)
	{
		sum = 0;
		for (uint j = 0; j < gSize; j++)
		{
			if (grid[GetIndex(i, j)] == player)
				sum++;
		}
		if (sum == gSize)
			return Win;
	}

	// Horitzontal
	for (uint j = 0; j < gSize; j++)
	{
		sum = 0;
		for (uint i = 0; i < gSize; i++)
		{
			if (grid[GetIndex(i, j)] == player)
				sum++;
		}
		if (sum == gSize)
			return Win;
	}

	// Diagonal 1
	sum = 0;
	for (uint i = 0; i < gSize; i++)
	{
		if (grid[GetIndex(i, i)] == player)
			sum++;
	}
	if (sum == gSize)
		return Win;

	// Diagonal 2
	sum = 0;
	for (uint i = 0; i < gSize; i++)
	{
		if (grid[GetIndex(i, gSize - i - 1)] == player)
			sum++;
	}
	if (sum == gSize)
		return Win;

	// Draw
	sum = 0;
	for (uint i = 0; i < gSize; i++)
	{
		for (uint j = 0; j < gSize; j++)
		{
			if (grid[GetIndex(i, j)] != 0)
				sum++;
		}
	}
	if (sum == gSize * gSize)
		return Draw;

	// None
	return None;
}

void SaveMove(const std::array<int, gSize * gSize>& grid, const uint& play)
{
	std::fstream file("C:/Users/elies/Desktop/save.txt", std::fstream::out | std::fstream::app);

	for (uint i = 0; i < gSize * gSize; i++)
	{
		file << grid[i] << "\n";
	}
	file << play << "\n";

	file.close();
}

int main()
{
	std::array<int, gSize * gSize> grid;

	ResetGrid(grid);

#if bot == 1

	nn::NeuralNetwork<gSize * gSize, 3, gSize * gSize> nn({ 10, 10, 10 }, nn::Activation::hyperbolicTangent, { -5, 5 });

	std::fstream file("C:/Users/elies/Desktop/save.txt");

	std::vector<std::pair<std::array<int, gSize * gSize>, uint>> data; // all the data in the document
	std::string str; // Stores the current file line as text
	int count = 0; // Index on the array
	std::array<int, gSize * gSize> rInputs; // Raw inputs from file
	uint rOutput; // Raw output from file
	while (file >> str)
	{
		if (count < gSize * gSize)
		{
			rInputs[count] = std::stoi(str);
			count++;
		}
		else
		{
			rOutput = std::stoi(str);
			data.push_back(std::pair<std::array<int, gSize * gSize>, uint>(rInputs, rOutput));
			count = 0;
		}
	}

	file.close();

	for (uint i = 0; i < 5000; i++)
	{
		std::random_shuffle(data.begin(), data.end());
		for (auto& d : data)
		{
			nn.Train(GetInputs(d.first), GetOutput(d.second), 0.1, 0.0);
		}
	}

	//system("cls");

#endif

	while (true)
	{
		// Inputs
		uint x1, y1;
		do
		{
			DrawGrid(grid);
			std::cout << "Player 1\n";
			std::cout << "x: ";
			std::cin >> x1;
			x1--;
			std::cout << "y: ";
			std::cin >> y1;
			y1--;
			system("cls");
		} while (x1 < 0 || x1 >= gSize || y1 < 0 || y1 >= gSize || grid[GetIndex(x1, y1)] != 0);

		//TrainNN(nn, grid, GetIndex(x1, y1));

		SaveMove(grid, GetIndex(x1, y1));

		grid[GetIndex(x1, y1)] = 1;

		switch (CheckResult(grid, 1))
		{
		case Win:
			system("cls");
			DrawGrid(grid);
			std::cout << "PLAYER 1 WINS!!!";
			WaitFor(5s);
			system("cls");
			ResetGrid(grid);
			continue;

		case Draw:
			system("cls");
			DrawGrid(grid);
			std::cout << "DRAW!!";
			WaitFor(5s);
			system("cls");
			ResetGrid(grid);
			continue;
		}

#if bot == 0
		uint x2, y2;
		do
		{
			DrawGrid(grid);
			std::cout << "Player 2\n";
			std::cout << "x: ";
			std::cin >> x2;
			x2--;
			std::cout << "y: ";
			std::cin >> y2;
			y2--;
			system("cls");
		} while (x2 < 0 || x2 >= gSize || y2 < 0 || y2 >= gSize || grid[GetIndex(x2, y2)] != 0);

		//TrainNN(nn, grid, GetIndex(x2, y2));
		
		SaveMove(grid, GetIndex(x2, y2));

		grid[GetIndex(x2, y2)] = 2;

#else
		// Inputs
		std::array<double, gSize * gSize> inputs = GetInputs(grid);

		// NN Output
		std::array<double, gSize * gSize> output = nn.Calculate(inputs);

		std::vector<nn::Vec2<double>> outputV;
		for (uint i = 0; i < gSize * gSize; i++)
		{
			outputV.push_back({ (double)i, output[i] });
		}

		uint nnOutput;
		while (true)
		{
			nn::Vec2<double> max = { 0, 0 };
			uint index;
			for (uint i = 0; i < outputV.size(); i++)
			{
				if (outputV[i].y > max.y)
				{
					max = outputV[i];
					index = i;
				}
			}

			if (grid[max.x] == 0)
			{
				nnOutput = max.x;
				break;
			}
			else
				outputV.erase(outputV.begin() + index);
		}

		grid[nnOutput] = 2;
#endif

		switch (CheckResult(grid, 2))
		{
		case Win:
			system("cls");
			DrawGrid(grid);
			std::cout << "PLAYER 2 WINS!!!";
			WaitFor(5s);
			system("cls");
			ResetGrid(grid);
			continue;

		case Draw:
			system("cls");
			DrawGrid(grid);
			std::cout << "DRAW!!";
			WaitFor(5s);
			system("cls");
			ResetGrid(grid);
			continue;
		}
	}
}