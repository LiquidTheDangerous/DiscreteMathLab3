//
// Created by ASUS on 05.02.2023.
//
#include <ViewPositionProvider.hpp>
#include <cmath>
ViewPositionProvider::ViewPositionProvider(sf::View &view) : view(&view) {

}

sf::View *ViewPositionProvider::getView() {
    return this->view;
}

void ViewPositionProvider::setView(sf::View &view) {
    this->view = &view;
}

sf::IntRect ViewPositionProvider::getViewPort(int w, int h) {
    auto width = static_cast<float>(w);
    auto height = static_cast<float>(h);
    const sf::FloatRect &viewport = view->getViewport();
    return {std::lround(width * viewport.left),
                   std::lround(height * viewport.top),
                   std::lround(width * viewport.width),
                   std::lround(height * viewport.height)};
}
