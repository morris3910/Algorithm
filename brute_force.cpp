#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//structure to represent item
struct Item {
    int weight;
    int value;
};

//calculate the total value
int calculateTotalValue(const vector<Item>& items, const vector<int>& selectedItems) {
    int totalValue = 0;
    for (int i = 0; i < selectedItems.size(); ++i) {
        if (selectedItems[i] == 1) {
            totalValue += items[i].value;
        }
    }
    return totalValue;
}

//find the maximum value
void exhaustiveSearch(const vector<Item>& items, int capacity,string filename) {
    int n = items.size();
    vector<int> bestSolution(n, 0); // To store the best solution
    int maxValue = 0; // To store the maximum value

    //generate all possible
    for (int i = 0; i < (1 << n); ++i) {
        vector<int> selectedItems(n, 0);
        int totalWeight = 0;

        //caculate the weight
        for (int j = 0; j < n; ++j) {
            if ((i & (1 << j)) != 0) {//eg : i=0110 j=0001 0010 0100 1000
                selectedItems[j] = 1;
                totalWeight += items[j].weight;
            }
        }

        // check if the total weight <= capacity
        if (totalWeight <= capacity) {
            int totalValue = calculateTotalValue(items, selectedItems);
            if (totalValue > maxValue) {
                maxValue = totalValue;
                bestSolution = selectedItems;
            }
        }
    }

    //print the answer
	string output_filePath = "ans_" + filename + ".txt";
	ofstream outputfile(output_filePath.c_str());
	outputfile << "max profit: " << maxValue << endl;
    outputfile << "solution : ";
    for (int i = 0; i < n; ++i) {
        outputfile << bestSolution[i];
    }
    outputfile << endl;
}

int main() {
    string filename;
	cout << "Enter the filename: ";
    cin >> filename;
	string filePath = "./dataset/" + filename + "/item.txt";
    ifstream inputFile(filePath.c_str());
    string line;
    vector<Item> data_set;
    Item data;
    int number_of_item = 0, bag_size = 0;
    int i = 0;

    getline(inputFile, line); // Read file
    istringstream tmp_line(line);
    tmp_line >> number_of_item >> bag_size;

    while (getline(inputFile, line)) {
        istringstream tmp_line(line); // Read file and store into vector
        tmp_line >> data.weight >> data.value;
        data_set.push_back(data);
    }
    inputFile.close();

    //solve the knapsack problem
    exhaustiveSearch(data_set, bag_size,filename);

    return 0;
}

