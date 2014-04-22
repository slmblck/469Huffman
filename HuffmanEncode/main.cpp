#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
    //Get the three file names
    if (argc != 4)
    {
        cout << "There must be 3 files listed" << endl;
        exit(-1);
    }
    char *uFaultName = argv[1];
    char *benchName = argv[2];
    char *outputName = argv[3];
    vector<string> faults;
    map<string, int> gateList;
    vector<string> gateTypes;

    //Open the ufaults file
    ifstream uFault;
    uFault.open(uFaultName, ios::out);
    //Check that file is open
    if (uFault.is_open())
    {
        string line;
        while (getline(uFault, line))
        {
            //Find where the slash is
            int pos = line.find("/");
            string withoutSlash = line.substr(0, pos-1);

            //Check if this string has an ->
            pos = withoutSlash.find("->");
            if (pos != -1)
            {
                string firstFault = withoutSlash.substr(0, pos-1);
                string secondFault = withoutSlash.substr(pos+2);
//                if (find(faults.begin(), faults.end(), firstFault) == faults.end())
//                {
//                    faults.push_back(firstFault);
//                }
                if (find(faults.begin(), faults.end(), secondFault) == faults.end())
                {
                    faults.push_back(secondFault);
                }
            }
            else
            {
                if (find(faults.begin(), faults.end(), withoutSlash) == faults.end())
                {
                    faults.push_back(withoutSlash);
                }
            }
        }
    }
    else
    {
        cout << "There was an error opening: " << uFaultName << endl;
    }

    //Close the file
    uFault.close();

    cout << "The faults found are: " << endl;
    for (int i = 0; (unsigned)i < faults.size(); i++)
    {
        cout << faults.at(i) << endl;
    }
    cout << endl;

    //Open the bench file
    ifstream bench;
    bench.open(benchName, ios::out);
    //Check that the file is open
    if (bench.is_open())
    {
        string line;
        while (getline(bench, line))
        {
            for (int i = 0; (unsigned) i < faults.size(); i++)
            {
                string stringToCheck = faults.at(i) + " = ";
                int pos = line.find(stringToCheck);
                if (pos != -1)
                {
                    string fullGate = line.substr(pos+stringToCheck.size());
                    int gatePos = fullGate.find("(");
                    string gate = fullGate.substr(0, gatePos);
                    //Check if gate is in map
                    if (gateList.find(gate) == gateList.end())
                    {
                        gateList[gate] = 1;
                        gateTypes.push_back(gate);
                    }
                    else
                    {
                        int value = gateList.at(gate);
                        gateList[gate] = value+1;
                    }
                }
            }
        }
    }
    else
    {
        cout << "There was an error opening the file " << benchName << endl;
    }

    //Close the file
    bench.close();

    //Output the gates found
    cout << "The Gates found: " << endl;
    for (int i = 0; (unsigned)i < gateTypes.size(); i++)
    {
        cout << gateTypes.at(i) << ": " << gateList[gateTypes.at(i)] << endl;
    }
    cout << endl;

    //Place data in output file
    ofstream output(outputName);
    if (output.is_open())
    {
        output << "The Gates found: \n";
        for (int i = 0; (unsigned)i < gateTypes.size(); i++)
        {
            output << gateTypes.at(i) << ": " << gateList[gateTypes.at(i)] << "\n";
        }
    }
    else
    {
        cout << "There was an error opening the file " << outputName << endl;
    }

    output.close();
    return 0;
}

