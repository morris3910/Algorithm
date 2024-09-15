#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<algorithm>
using namespace std;

//structure to represent item
struct Item {
    int weight;
    int value;
    double CP;
    bool pick;
};


//find the maximum value
void greedySearch(vector<Item>& items, int capacity,string filename) {
    int n = items.size();
    vector<double> CP_set;
    for(int i = 0;i < n;i++){
    	CP_set.push_back(items[i].CP);
    }
    sort(CP_set.rbegin(),CP_set.rend());

    int totalValue = 0,totalWeight = 0; // To store the maximum value
    //generate all possible
    for (int i = 0; i < n; i++) {
		for(int j = 0;j < n;j++){
			if(CP_set[i] == items[j].CP){
				if(items[j].weight + totalWeight <= capacity){
					totalValue += items[j].value;
					totalWeight += items[j].weight;
					items[j].pick = 1;
					break;
				}
			}
		}
		
    }

    //print the answer
	string output_filePath = "ans_" + filename + ".txt";
	ofstream outputfile(output_filePath.c_str());
	outputfile << "max profit: " << totalValue << endl;
    outputfile << "solution : ";
    for (int i = 0; i < n; ++i) {
        outputfile << items[i].pick;
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

    getline(inputFile, line); // Read file
    istringstream tmp_line(line);
    tmp_line >> number_of_item >> bag_size;

    while (getline(inputFile, line)) {
        istringstream tmp_line(line); // Read file and store into vector
        tmp_line >> data.weight >> data.value;
        data.CP = (double)data.value / data.weight;
        data.pick = 0;
        data_set.push_back(data);
    }
    inputFile.close();
    //solve the knapsack problem
    greedySearch(data_set, bag_size,filename);

    return 0;
}

