#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    //Get the files
    if (argc != 3)
    {
        cout << "There should be a bench file and test file specified" << endl;
        exit(-1);
    }
    char *benchName = argv[1];
    char *testFile = argv[2];
    int numInputs = 0;
    vector<string> inputVectors;
    vector<string> outputVectors;

    //Open the bench file
    ifstream bench;
    bench.open(benchName, ios::out);
    //Check that file is open
    if (bench.is_open())
    {
        cout << "Counting the inputs in the file " << benchName << endl;
        string line;
        while (getline(bench, line))
        {
            //Find where the slash is
            size_t pos = line.find("INPUT");

            //If position != -1 then "INPUT" was found
            //Increment counter
            if (pos != string::npos)
            {
                numInputs++;
            }
        }
    }
    else
    {
        cout << "There was an error opening: " << benchName << endl;
    }

    //Close the file
    bench.close();

    //Print out the number of inputs
    cout << "The number of inputs for " << benchName << " is " << numInputs << endl;
    cout << endl;

    //Get the test vectors for the specified file
    //Open the test file
    ifstream test;
    test.open(testFile, ios::out);
    //Check that the file is open
    if (test.is_open())
    {
        cout << "Getting the test vectors for the file " << testFile << endl;
        string line;
        while (getline(test, line))
        {
            //Find if there is a test vector on this line
            size_t positionOfVector = -1;
            bool foundVector = false;
            if (line.find(": 0") != string::npos)
            {
                positionOfVector = line.find(": 0") + 2;
                foundVector = true;
            }
            else if (line.find(": 1") != string::npos)
            {
                positionOfVector = line.find(": 1") + 2;
                foundVector = true;
            }

            if (foundVector == true)
            {
                //Get the input and output vector
                string inputNOutput = line.substr(positionOfVector);

                //Split the input and output vector
                size_t endInput = inputNOutput.find(" ");
                string inputVec = inputNOutput.substr(0, endInput);
                string outputVec = inputNOutput.substr(endInput+1);

                //Add the input and output to the vector
                inputVectors.push_back(inputVec);
                outputVectors.push_back(outputVec);
            }
        }
    }

    //Close the file
    test.close();

    //Output the input and output vectors
    cout << "The input test vectors for " << benchName << " are:" << endl;
    for (int i = 0; (unsigned)i < inputVectors.size(); i++)
    {
        cout << inputVectors.at(i) << endl;
    }
    cout << endl;

    cout << "The output test vectors for " << benchName << " are:" << endl;
    for (int i = 0; (unsigned)i < outputVectors.size(); i++)
    {
        cout << outputVectors.at(i) << endl;
    }

    return 0;
}

