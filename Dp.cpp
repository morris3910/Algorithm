//need to put dataset file in same path
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
};

int main(){
	string filename;
	cout << "Enter the filename(ex:dt01): ";
    cin >> filename;
	string filePath = "./dataset/" + filename + "/item.txt";
    ifstream inputFile(filePath.c_str());
    string line;
    vector<Item> data_set;
    Item data;
    int number_of_item = 0, capacity = 0;

    getline(inputFile, line); // Read file
    istringstream tmp_line(line);
    tmp_line >> number_of_item >> capacity;

    while (getline(inputFile, line)) {
        istringstream tmp_line(line); // Read file and store into vector
        tmp_line >> data.weight >> data.value;
        data_set.push_back(data);
    }
    inputFile.close();

	vector<vector<int> > dp(number_of_item+1,vector<int>(capacity+1,0));//use vector to store dp array
	for(int i = 0;i < number_of_item+1;i++){//down traversal
		for(int j = 0;j < capacity+1;j++){//right traversal
			if(i == 0 || j == 0)//capacity == 0 || item == 0 --> dp = 0
				dp[i][j] = 0;
			else if(i > 0 && data_set[i-1].weight > j)//this item's weight > capacity && item > 0 --> dp = last dp
				dp[i][j] = dp[i-1][j];
			else if(i > 0 && data_set[i-1].weight <= j){//this item's weight < capacity && item > 0 -->
				if(dp[i-1][j] >= data_set[i-1].value + dp[i-1][j-data_set[i-1].weight])//last dp > this item's value + dp[capacity-this item's weight] --> dp = last dp
					dp[i][j] = dp[i-1][j];
				else
					dp[i][j] = data_set[i-1].value + dp[i-1][j-data_set[i-1].weight];
				//dp[i][j] = max{dp[i-1][j],data_set[i].value + dp[i-1][j-data_set[i].weight]};
			}
		}
	}
	int total_value = dp[number_of_item][capacity],tmp_item = number_of_item,tmp_capacity = capacity;
	string ans = "";
	while(total_value > 0 && number_of_item > 0){
		if(total_value == dp[number_of_item-1][capacity]){//didn't pick this item
			number_of_item--;
			ans += "0";
		}
		else{//pick this item
			total_value -= data_set[number_of_item-1].value;
			capacity -= data_set[number_of_item-1].weight;
			number_of_item--;
			ans += "1";
		}	
	}
	string output_filePath = "ans_" + filename + ".txt";
	ofstream outputfile(output_filePath.c_str());
	outputfile << "max profit: " << dp[tmp_item][tmp_capacity] << endl;
    outputfile << "solution : ";
    for (int i = ans.length()-1; i >= 0; i--) {//reverse output
        outputfile << ans[i];
    }
    outputfile << endl;
	return 0;
}

