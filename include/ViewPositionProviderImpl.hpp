//
// Created by ASUS on 05.02.2023.
//

#ifndef DISCRETEMATHLAB3_VIEWPOSITIONPROVIDERIMPL_HPP
#define DISCRETEMATHLAB3_VIEWPOSITIONPROVIDERIMPL_HPP
#include <ViewPositionProvider.hpp>

class ViewPositionProviderImpl : public ViewPositionProvider{
public:
    std::pair<float, float> getPositionInView(int x, int y, int w, int h) override;

};


#endif //DISCRETEMATHLAB3_VIEWPOSITIONPROVIDERIMPL_HPP
