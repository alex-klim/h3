#include "date.hpp"

int main() {
    Date dt;
    std::cin >> dt;
    std::cout << "dt: " << dt << '\n';
    Date dt2("1212315.04.2542asdasd");
    std::cout << "dt2: " << dt2 << '\n';
    std::cout << "changing format of dt to mmddyyyy:\n";
    dt.setFormat(Format::mmddyyyy);
    std::cout << dt << '\n';
    std::cout << "**********************************************\n";
    int temp = dt2 - dt;
    std::cout << "dt2 - dt = " << temp << '\n';
    std::cout << "**********************************************\n";
    dt.applyDelta(temp);
    std::cout << "dt.applyDelta(" << temp << "):\n" << dt << '\n';

    return 0;
}
