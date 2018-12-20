#include "date.hpp"

int main() {
    Date dt;
     std::cin >> dt;
    Date dt2;
    dt.setFormat(Format::mmddyyyy);
    std::cin >> dt2;
    int temp = dt2 - dt;
    std::cout << temp << '\n';
    dt.applyDelta(temp);
    std::cout << dt << '\n';

    return 0;
}
