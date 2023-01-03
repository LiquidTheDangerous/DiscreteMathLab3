//
// Created by ASUS on 03.01.2023.
//

#include <EntityBounder.h>
#include <Application.h>

EntityBounder::EntityBounder(Application *app) {
    this->first = nullptr;
    this->second = nullptr;
    this->app = app;
}

void EntityBounder::pushEntity(Entity *e) {
    if (this->first) {
        this->second = e;

        auto iter = std::find_if(app->arrows.begin(), app->arrows.end(), [this](std::shared_ptr<Arrow> &arrow) -> bool {
            auto f = arrow->getStart();
            auto s = arrow->getEnd();
            return f == first && s == second;
        });

        if (iter != app->arrows.end()) {
            (*iter)->markToRemove(true);
            app->createMessage("Edge removed", 0.5f);
        } else {
            auto arrow = std::make_shared<Arrow>(first, second);
            app->arrows.push_back(arrow);
            app->createMessage("Edge created", 0.5f);

        }

        this->first = nullptr;
        this->second = nullptr;
    } else {
        this->first = e;
    }
}

bool EntityBounder::isBounding() const {
    return this->first != nullptr || this->second != nullptr;
}
