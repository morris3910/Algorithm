//delete repeat + uniform(keep parent) + mutation (keep parent)
//need to put dataset file in same path
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>
#define selection 8
#define crossover 1
#define mutation 0.27
using namespace std;

// Structure to represent item
struct Item {
    int weight;
    int value;
};

int main() {
    string filename;
    cout << "Enter the filename(ex:dt01): ";
    cin >> filename;
    string filePath = "./dataset/" + filename + "/item.txt";
    ifstream inputFile(filePath.c_str());
	string output_filePath = "ans_" + filename + ".txt";
	ofstream outputfile(output_filePath.c_str());

    // Check if the file opened successfully
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
	
    string line;
    vector<Item> data_set;
    long long int count = 0;
    int run = 1;
    Item data;
    int number_of_item = 0, capacity = 0;
    int evaluation_max = filename == "dt01" ? 1000 : (filename == "dt02" ? 10000 : 500000);
    int population = filename == "dt01" ? 100 : (filename == "dt02" ? 200 : 4850);
    
    // Read number of items and capacity
    getline(inputFile, line);
    istringstream tmp_line(line);
    tmp_line >> number_of_item >> capacity;

    // Read items data
    while (getline(inputFile, line)) {
        istringstream tmp_line(line);
        tmp_line >> data.weight >> data.value;
        data_set.push_back(data);
    }
    inputFile.close();
	while(run <= 30)
	{
		cout<<"Round : "<<run<<endl;
		// Initialize population
		vector<int> ObjectiveValue;
		vector<string> pick;
		int eva = 0;
		
		srand(time(NULL));
		for (int i = 0; i < population; i++) {
			string str(number_of_item, '0');
		    for (int j = 0; j < number_of_item; j++) {
		    	if(rand() % 2)	str[j] = '1';
		        else	str[j] = '0';
		    }
		    pick.push_back(str);
		}

		// Evaluation
		for (int i = 0; i < population; i++) {
		    int ObjectiveWeight = 0, tmp = 0;
		    eva++;
		    for (int j = 0; j < number_of_item; j++) {
		        if (pick[i][j] == '1') {
		            tmp += data_set[j].value;
		            ObjectiveWeight += data_set[j].weight;
		        }
		        if (ObjectiveWeight > capacity) {
		        	pick[i][j] = '0';
					tmp -= data_set[j].value;
					ObjectiveWeight -= data_set[j].weight;
				}
		    }
		    ObjectiveValue.push_back(tmp);
		}
		for(int i = 0;i < ObjectiveValue.size();i++){
			for(int j = i+1;j < ObjectiveValue.size();){
				if(ObjectiveValue[i] == ObjectiveValue[j]){
					ObjectiveValue.erase(ObjectiveValue.begin()+j);
					pick.erase(pick.begin()+j);
				}
				else
					++j;
			}
		}
		// Main loop for genetic algorithm
		while(eva < evaluation_max) {
		    if (ObjectiveValue.size() == 1) break;
		    vector<int> NewObjectiveValue;
		    vector<string> Newpick;

		    // Selection
		    for (int i = 0; i < ObjectiveValue.size(); i++) {
		        vector<pair<int, int>> selected;
		        for (int j = 0; j < selection; j++) {
		            int select = rand() % ObjectiveValue.size();
		            selected.push_back(make_pair(ObjectiveValue[select], select));
		        }
		        sort(selected.rbegin(), selected.rend());
		        NewObjectiveValue.push_back(selected[0].first);
		        Newpick.push_back(pick[selected[0].second]);
		    }

		    ObjectiveValue = NewObjectiveValue;
		    pick = Newpick;

		    // Crossover
			const int mid = ObjectiveValue.size() / 2;
		    for (int i = 0; i < mid; i++) {
		        float cross_rate = static_cast<float>(rand()) / RAND_MAX;
		        if (cross_rate <= crossover) {
		        	eva++;
		            string str1 = pick[i],str2 = pick[mid+i];
		            for (int j = 0; j < number_of_item; j++) {
		            	if(rand() % 2){	
				        	char tmp = str1[j];
				        	str1[j] = str2[j];
				        	str2[j] = tmp;
				        }
		            }
		            if(filename == "dt01"){
		            	pick.push_back(str1);
		            	pick.push_back(str2);
		            }
		            else if(filename == "dt02")
		            {
		            	pick.push_back(str1);
		            }
		            else{
		            	if(pick.size() < 5)
		            	{
		            		pick.push_back(str1);
		            		pick.push_back(str2);
		            	}
		            	else
		            		pick.push_back(str2);
		            }
		        }
		    }

		    // Mutation
		    int size = pick.size();
		    for (size_t i = 0; i < size; i++) {
		        float mutation_rate = static_cast<float>(rand()) / RAND_MAX;
		        if (mutation_rate <= mutation) {
		        	pick.push_back(pick[i]);
		            int mutation_point = rand() % number_of_item;
		            if(pick[i][mutation_point] == '1')	pick[i][mutation_point] = '0';
		            else pick[i][mutation_point] = '1';
		        }
		    }

		    // Clear previous values
		    ObjectiveValue.clear();

		    // Re-evaluation
		    for (size_t i = 0; i < pick.size(); i++) {
		        int ObjectiveWeight = 0, tmp = 0;
		        eva++;
		        for (int j = 0; j < number_of_item; j++) {
		            if (pick[i][j] == '1') {
		                tmp += data_set[j].value;
		                ObjectiveWeight += data_set[j].weight;
		                if (ObjectiveWeight > capacity) {
		                    pick[i][j] = '0';
		                    tmp -= data_set[j].value;
		                    ObjectiveWeight -= data_set[j].weight;
		                }
		            }
		        }
		        ObjectiveValue.push_back(tmp);
		    }
		    //delete repeat
		    for(int i = 0;i < ObjectiveValue.size();i++){
				for(int j = i+1;j < ObjectiveValue.size();){
					if(ObjectiveValue[i] == ObjectiveValue[j]){
						ObjectiveValue.erase(ObjectiveValue.begin()+j);
						pick.erase(pick.begin()+j);
					}
					else
						++j;
				}
			}
		}
		// Print answer
		auto maxElementIndex = max_element(ObjectiveValue.begin(), ObjectiveValue.end()) - ObjectiveValue.begin();
		count += ObjectiveValue[maxElementIndex];
		cout << pick[maxElementIndex];
		cout << endl << "Max profit: " << ObjectiveValue[maxElementIndex] << endl << "eva :" << eva << endl << endl;
		outputfile << "Max profit: " << ObjectiveValue[maxElementIndex] << endl;
		outputfile << "solution : ";
		outputfile << pick[maxElementIndex] << endl << endl;
		run++;
	}
	cout<<"Avg profit of 30 Round : "<<count/30<<endl;
	outputfile<<"Avg profit of 30 Round : "<<count/30<<endl;
    return 0;
}
