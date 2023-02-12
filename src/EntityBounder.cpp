//
// Created by ASUS on 03.01.2023.
//

#include <EntityBounder.h>
#include <Application.h>
#include <iostream>
#include <cctype>

EntityBounder::EntityBounder(Application *app) {
    this->first = nullptr;
    this->second = nullptr;
    this->app = app;
}

void EntityBounder::pushEntity(Entity *e) {
    if (this->first) {
        this->second = e;

        auto iter = std::find_if(app->arrowHolder->getEntities().begin(), app->arrowHolder->getEntities().end(),
                                 [this](std::shared_ptr<Entity> &arrow) -> bool {
                                     auto f = (std::static_pointer_cast<Arrow>(arrow))->getStart();
                                     auto s = (std::static_pointer_cast<Arrow>(arrow))->getEnd();
                                     return f == first && s == second;
                                 });
        auto transIter = std::find_if(app->arrowHolder->getEntities().begin(), app->arrowHolder->getEntities().end(),
                                      [this](std::shared_ptr<Entity> &arrow) -> bool {
                                          auto f = (std::static_pointer_cast<Arrow>(arrow))->getStart();
                                          auto s = (std::static_pointer_cast<Arrow>(arrow))->getEnd();
                                          return f == second && s == first;
                                      });

//        auto firstName = app->graph.

        if (iter != app->arrowHolder->getEntities().end() && transIter == app->arrowHolder->getEntities().end()) {
            (*iter)->markToRemove(true);
            app->graph.disconnect(first->getName(), second->getName());
            app->createMessage("Edge removed", 0.5f);
        } else {
            if (transIter != app->arrowHolder->getEntities().end() && iter == app->arrowHolder->getEntities().end()) {
                auto p = std::static_pointer_cast<Arrow>(*transIter);
                p->setIsOriented(false);
                auto arrow = std::make_shared<Arrow>(first, second, app->font, app->view);
                arrow->setShowText(false);
                arrow->setIsOriented(false);
                arrow->setW((p->getW()));
                this->initArrowSignals(arrow.get());
                app->arrowHolder->getEntities().push_back(arrow);
                app->graph.setW(first->getName(), second->getName(),p->getW());
                app->createMessage("Unoriented Edge created", 0.5f);

            } else if (transIter != app->arrowHolder->getEntities().end() &&
                       iter != app->arrowHolder->getEntities().end()) {
                auto p = std::static_pointer_cast<Arrow>(*transIter);
                p->setIsOriented(true);
                p->setShowText(true);
                (*iter)->markToRemove(true);
                app->graph.disconnect(first->getName(), second->getName());
                app->createMessage("Edge removed", 0.5f);
            } else if (transIter == app->arrowHolder->getEntities().end() &&
                       iter == app->arrowHolder->getEntities().end()) {
                auto arrow = std::make_shared<Arrow>(first, second, app->font, app->view);
                this->initArrowSignals(arrow.get());
                app->arrowHolder->getEntities().push_back(arrow);
                app->graph.connect(first->getName(), second->getName());
                app->createMessage("Edge created", 0.5f);
            }
        }
        app->colorizeVertices();
        this->first = nullptr;
        this->second = nullptr;
    } else {
        this->first = e;
    }
}

bool EntityBounder::isBounding() const {
    return this->first != nullptr || this->second != nullptr;
}

void EntityBounder::initArrowSignals(Arrow *arrow) {
    arrow->setTextValidator([](const char &symbol) -> bool {
        if (symbol == '\r') {
            return true;
        }
        if (symbol == '\b') {
            return true;
        }
        if (std::isalpha(symbol)) {
            return false;
        }
        if (std::isspace(symbol)) {
            return false;
        }
        if (!std::isdigit(symbol)) {
            return false;
        }
        return true;
    });
    arrow->getSignal(signals::onEndEditingText).addSlot([arrow, this](void *) {
        if (arrow->getLabelTextSize() == 0) {
            arrow->setLabelString(std::to_string(arrow->getW()));
            app->createMessage("Bad mark size", 0.5f);
            return;
        }
        try {
            auto len = std::stoi(arrow->getString());
            auto isConnect = app->graph.setW(arrow->getSourceName(), arrow->getDestinationName(), len);
            if (isConnect && !arrow->isOriented()) {
                app->graph.setW(arrow->getDestinationName(), arrow->getSourceName(), len);
                arrow->setW(len);
                auto iter = std::find_if(app->arrowHolder->getEntities().begin(), app->arrowHolder->getEntities().end(),
                                         [arrow](const std::shared_ptr<Entity> &e) -> bool {
                                             auto p = std::static_pointer_cast<Arrow>(e);

                                             return p->getStart() == arrow->getEnd() &&
                                                    p->getEnd() == arrow->getStart();
                                         });
                if (iter != app->arrowHolder->getEntities().end()) {
                    auto p = std::static_pointer_cast<Arrow>(*iter);
                    p->setW(len);
                }
                char format[255]{};
                std::sprintf(format, "from %s to %s and from %s to %s = %d", arrow->getSourceName().c_str(),
                             arrow->getDestinationName().c_str(), arrow->getDestinationName().c_str(),
                             arrow->getSourceName().c_str(), len);
                app->createMessage(format, 0.5f);

            } else if (isConnect) {
                arrow->setW(len);
                char format[255]{};
                std::sprintf(format, "from %s to %s = %d", arrow->getSourceName().c_str(),
                             arrow->getDestinationName().c_str(), len);
                app->createMessage(format, 0.5f);
            }

        } catch (std::invalid_argument &ex) {
            app->createMessage("can't bound", 0.5f);
        }
    });
}
