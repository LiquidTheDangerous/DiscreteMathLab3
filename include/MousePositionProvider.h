//
// Created by ASUS on 31.12.2022.
//

#ifndef DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDER_H
#define DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDER_H


#include <utility>

class MousePositionProvider {
public:
    [[nodiscard]] virtual std::pair<float,float> getMousePosition() const = 0;
};


#endif //DISCRETEMATHLAB3_MOUSEPOSITIONPROVIDER_H
