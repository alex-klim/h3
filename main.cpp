#include "date.hpp"

int main() {
    Date dt;
    std::cin >> dt;
    Date dt2("1212315.04.2542asdasd");
    std::cout << "dt2: " << dt2 << '\n';
    std::cout << "changing format of dt1 to mmddyyyy\n";
    dt.setFormat(Format::mmddyyyy);
    std::cout << "**********************************************\n";
    int temp = dt2 - dt;
    std::cout << "dt2 - dt1 = " << temp << '\n';
    std::cout << "**********************************************\n";
    dt.applyDelta(temp);
    std::cout << "dt.applyDelta:\n" << dt << '\n';

    return 0;
}
