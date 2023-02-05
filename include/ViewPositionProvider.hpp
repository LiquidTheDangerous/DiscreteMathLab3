//
// Created by ASUS on 05.02.2023.
//

#ifndef DISCRETEMATHLAB3_VIEWPOSITIONPROVIDER_HPP
#define DISCRETEMATHLAB3_VIEWPOSITIONPROVIDER_HPP

#include <SFML/Graphics/View.hpp>

class ViewPositionProvider{
public:
    explicit ViewPositionProvider(sf::View &view);
    virtual std::pair<float, float> getPositionInView(int x, int y, int w, int h) = 0;
    sf::IntRect getViewPort(int w, int h);
    sf::View* getView();
    void setView(sf::View& view);
private:
    sf::View* view;
};


#endif //DISCRETEMATHLAB3_VIEWPOSITIONPROVIDER_HPP
