//
// Created by ASUS on 05.02.2023.
//

#include "ViewPositionProviderImpl.hpp"



std::pair<float, float> ViewPositionProviderImpl::getPositionInView(int x, int y, int w, int h) {
    sf::Vector2f normalized;
    sf::IntRect viewport = this->getViewPort(w,h);
    normalized.x = -1.f + 2.f * static_cast<float>(x - viewport.left) / static_cast<float>(viewport.width);
    normalized.y =  1.f - 2.f * static_cast<float>(y - viewport.top)  / static_cast<float>(viewport.height);
    auto pos = this->getView()->getInverseTransform().transformPoint(normalized);
    return {pos.x,pos.y};
}
