#include <iostream>
#include <memory>
#include <string>

#include "CApplication.h"

using namespace std;

int main() {
    try {
        auto interface = make_shared<CInterface>(cin, cout);

        CApplication application(interface);
        application.Run();

        return 0;
    }
    catch (const ios::failure &exception) {
        if (cin.eof()) {
            return 0;
        }

        cerr << "Unexpected problem with input and output." << endl;
        cerr << "Error: " << exception.what() << endl;

        return 1;
    }
    catch (const runtime_error &exception) {
        cerr << "Error: " << exception.what() << endl;

        return 2;
    }
}