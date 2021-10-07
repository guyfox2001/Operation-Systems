#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Please input program name \n";
		return 0;
	}
	cout << "Hello, i'm shw module program. My input string: " << argv[1] << ". \n\t\tMy load path:"<< argv[0] << "\n";

	return 0;
}