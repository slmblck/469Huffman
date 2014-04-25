#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

int substring_compare(string x, string y){
    if(x.size() != y.size()){
        return 0;
    } else {
        for(int i = 0; (unsigned)i < x.size(); i++){
            if(x.at(i) != y.at(i)){
                return 0;
            }
        }
    }

    return 1;

}

int char_substring_comp(char *x, char *y){
    if(sizeof(x) != sizeof(y)){
        return 0;
    } else {
        int i = 0;
        while(x[i] != NULL){
            if(x[i] != y[i]){
                return 0;
            }
            i++;
        }
    }

    return 1;
}

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
    vector<string> inputVectorsBroken;
    vector<string> outputVectorsBroken;
    vector<string> inputVectorsUnique;
    vector<int> numInputVectorsUnique;
    vector<string> outputVectorsUnique;
    vector<int> numOutputVectorsUnique;

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
                //string outputVec = inputNOutput.substr(endInput+1, inputNOutput.size());
                string outputVec = inputNOutput.substr(endInput+1);
                size_t outSize = outputVec.size();

                //Add the input and output to the vector
                inputVectors.push_back(inputVec);
                outputVectors.push_back(outputVec);

                inputVec.erase(std::remove(inputVec.begin(), inputVec.end(), '\n'), inputVec.end());
                inputVec.erase(std::remove(inputVec.begin(), inputVec.end(), ' '), inputVec.end());

                //Now break up the vectors into strings of length 4
                for(int i = 0; (unsigned)i < endInput; i = i + 4){
                    string temp = inputVec.substr(i, 4);
                    //removes spaces if they exist (usually does nothing)
                    //temp.erase(end_pos, temp.end());

                    while (temp.size() < 4) {
                        temp.append("0");
                    }

                    inputVectorsBroken.push_back(temp);
                }

                for(int i = 0; (unsigned)i < outSize; i = i + 4){
                    string temp = outputVec.substr(i, 4);

                    while (temp.size() < 4) {
                        temp.append("0");
                    }

                    outputVectorsBroken.push_back(temp);
                }
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
    cout << endl;

    cout << "Broken up input" << endl;
    for (int i = 0; (unsigned)i< inputVectorsBroken.size(); i++){
        cout << inputVectorsBroken.at(i) << endl;
    }
    cout << endl;

    cout << "Broken up output" << endl;
    for(int i = 0; (unsigned)i < outputVectorsBroken.size(); i++){
        cout << outputVectorsBroken.at(i) << endl;
    }
    cout << endl;

    //Now that we have the input and output vectors all broken up
    //Count them up to see if we can compress any
    //This looks bad but has okay runtime
    for(int i = 0; (unsigned)i < inputVectorsBroken.size(); i++){
        string temp = inputVectorsBroken[i];
        inputVectorsUnique.push_back(temp);
        int count = 0;
        //cout << "Got here " << i << endl;
        for(int j = 0; (unsigned)j < inputVectorsBroken.size(); j++){
            string stemp = inputVectorsBroken[j];
            size_t found;
            if((found = temp.find(stemp)) != string::npos){
                cout << temp << "Equivalent to " << stemp << endl;
                count++;
                inputVectorsBroken.erase(inputVectorsBroken.begin() + j);
            }
        }
        //temp.clear();
        numInputVectorsUnique.push_back(count);
    }

    for(int i = 0; (unsigned)i < outputVectorsBroken.size(); i++){
        string temp = outputVectorsBroken[i];
        outputVectorsUnique.push_back(temp);
        int count = 0;
        //cout << "Got here " << i << endl;
        for(int j = 0; (unsigned)j < outputVectorsBroken.size(); j++){
            string stemp = outputVectorsBroken[j];
            size_t found;
            if((found = temp.find(stemp)) != string::npos){
                count++;
                outputVectorsBroken.erase(outputVectorsBroken.begin() + j);
            }
        }
        numOutputVectorsUnique.push_back(count);
    }

    cout << "Counted input" << endl;
    for(int i = 0; (unsigned)i < inputVectorsUnique.size(); i++){
        cout << inputVectorsUnique.at(i) << " : " << numInputVectorsUnique.at(i) << endl;
    }

    cout << "Counted output" << endl;
    for(int i = 0; (unsigned)i < outputVectorsUnique.size(); i++){
        cout << outputVectorsUnique.at(i) << " : " << numOutputVectorsUnique.at(i) << endl;
    }

    return 0;
}

